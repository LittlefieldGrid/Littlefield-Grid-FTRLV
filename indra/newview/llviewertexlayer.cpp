/*
Littlefield Grid FTRLV modified version, 2026-11-22.
Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
Existing upstream copyright and license notices are retained.
*/
/**
 * @file llviewertexlayer.cpp
 * @brief Viewer texture layer. Used for avatars.
 *
 * $LicenseInfo:firstyear=2012&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#include "llviewerprecompiledheaders.h"

#include "llviewertexlayer.h"

#include "llagent.h"
#include "llappviewer.h"
#include "llmd5.h"
#include "lltimer.h"
#include "message.h"
#include "llagentwearables.h"
#include "llassetstorage.h"
#include "llcallbacklist.h"
#include "llcoproceduremanager.h"
#include "llfilesystem.h"
#include "lluri.h"
#include "llviewerwearable.h"
#include "llviewertexture.h"
#include "llimagej2c.h"
#include "llnotificationsutil.h"
#include "llviewerregion.h"
#include "llglslshader.h"
#include "llvoavatarself.h"
#include "pipeline.h"
#include "llviewercontrol.h"


namespace
{
using namespace LLAvatarAppearanceDefines;

struct LegacyBakeBatch
{
    LLUUID region;
    LLVOAvatarSelf* avatar = nullptr;
    // Each batch is a new outfit generation. A null result reserves a slot.
    std::map<ETextureIndex, LLUUID> results;
    std::vector<LLUUID> wearable_assets;
    std::vector<ETextureIndex> eligible_slots;
    bool supports_bom = false;
    bool publication_queued = false;
    bool published = false;
};

std::shared_ptr<LegacyBakeBatch> active_bake_batch;
std::shared_ptr<LegacyBakeBatch> published_bake_batch;
U32 legacy_appearance_serial = 0;
constexpr U64 LEGACY_BAKE_BOM_PROTOCOL = U64(1) << 63;

std::vector<LLUUID> getBakeWearableAssets()
{
    std::vector<LLUUID> assets;
    for (S32 value = 0; value < LLWearableType::WT_COUNT; ++value)
    {
        const auto type = static_cast<LLWearableType::EType>(value);
        const U32 count = gAgentWearables.getWearableCount(type);
        for (U32 index = 0; index < count; ++index)
        {
            const LLViewerWearable* wearable = gAgentWearables.getViewerWearable(type, index);
            assets.push_back(wearable ? wearable->getAssetID() : LLUUID::null);
        }
        assets.push_back(LLUUID::null);
    }
    return assets;
}

bool isCurrentBakeBatch(const std::shared_ptr<LegacyBakeBatch>& batch)
{
    LLViewerRegion* region = gAgent.getRegion();
    return batch && batch == active_bake_batch && isAgentAvatarValid() &&
           batch->avatar == gAgentAvatarp.get() && region && region == gAgentAvatarp->getRegion() &&
           region->getRegionID() == batch->region && region->getCentralBakeVersion() == 0 &&
           batch->supports_bom == ((region->getRegionProtocols() & LEGACY_BAKE_BOM_PROTOCOL) != 0) &&
           batch->wearable_assets == getBakeWearableAssets();
}


LLUUID getLegacyBakeCacheHash(const LLAvatarAppearanceDictionary::BakedEntry& baked)
{
    LLMD5 hash;
    bool has_assets = false;
    for (const auto type : baked.mWearables)
    {
        for (U32 index = 0; index < gAgentWearables.getWearableCount(type); ++index)
        {
            if (const auto* wearable = gAgentWearables.getViewerWearable(type, index))
            {
                hash.update(wearable->getAssetID().mData, UUID_BYTES);
                has_assets = true;
            }
        }
    }

    LLUUID result;
    if (has_assets)
    {
        hash.update(baked.mWearablesHashID.mData, UUID_BYTES);
        hash.finalize();
        hash.raw_digest(result.mData);
    }
    return result;
}

void publishLegacyBake(const std::shared_ptr<LegacyBakeBatch>& batch)
{
    if (!isCurrentBakeBatch(batch) || batch->published || batch->eligible_slots.empty() ||
        !gMessageSystem || gDisconnected)
    {
        return;
    }

    for (const auto slot : batch->eligible_slots)
    {
        const auto found = batch->results.find(slot);
        if (found == batch->results.end() || found->second.isNull())
        {
            return;
        }
    }

    LLVOAvatarSelf& avatar = *batch->avatar;
    LLPrimitive packet_textures;
    const U8 texture_count = batch->supports_bom ? TEX_NUM_INDICES : TEX_HEAD_UNIVERSAL_TATTOO;
    packet_textures.setNumTEs(texture_count);
    const auto* dictionary = LLAvatarAppearance::getDictionary();
    for (U8 index = 0; index < texture_count; ++index)
    {
        LLTextureEntry entry = avatar.getTEref(index);
        const auto bake = batch->results.find(static_cast<ETextureIndex>(index));
        entry.setID(bake == batch->results.end() ? IMG_DEFAULT_AVATAR : bake->second);
        packet_textures.setTE(index, entry);
    }

    LLMessageSystem* msg = gMessageSystem;
    msg->newMessageFast(_PREHASH_AgentSetAppearance);
    msg->nextBlockFast(_PREHASH_AgentData);
    msg->addUUIDFast(_PREHASH_AgentID, gAgent.getID());
    msg->addUUIDFast(_PREHASH_SessionID, gAgent.getSessionID());
    msg->addU32Fast(_PREHASH_SerialNum, ++legacy_appearance_serial);
    msg->addVector3Fast(_PREHASH_Size, avatar.mBodySize);

    for (const auto slot : batch->eligible_slots)
    {
        const auto* texture = dictionary->getTexture(slot);
        const auto* baked = dictionary->getBakedTexture(texture->mBakedTextureIndex);
        const LLUUID hash = getLegacyBakeCacheHash(*baked);
        if (hash.notNull())
        {
            msg->nextBlockFast(_PREHASH_WearableData);
            msg->addUUIDFast(_PREHASH_CacheID, hash);
            msg->addU8Fast(_PREHASH_TextureIndex, static_cast<U8>(slot));
        }
    }

    msg->nextBlockFast(_PREHASH_ObjectData);
    if (!packet_textures.packTEMessage(msg))
    {
        msg->clearMessage();
        LL_WARNS("Outfit") << "Could not pack the legacy appearance texture entries" << LL_ENDL;
        return;
    }

    for (LLVisualParam* param = avatar.getFirstVisualParam(); param; param = avatar.getNextVisualParam())
    {
        if (param->getGroup() == VISUAL_PARAM_GROUP_TWEAKABLE ||
            param->getGroup() == VISUAL_PARAM_GROUP_TRANSMIT_NOT_TWEAKABLE)
        {
            msg->nextBlockFast(_PREHASH_VisualParam);
            msg->addU8Fast(_PREHASH_ParamValue,
                           F32_to_U8(param->getWeight(), param->getMinWeight(), param->getMaxWeight()));
        }
    }

    if (msg->sendReliable(gAgent.getRegionHost()) <= 0)
    {
        LL_WARNS("Outfit") << "Could not send the legacy appearance message" << LL_ENDL;
        return;
    }

    batch->published = true;
    published_bake_batch = batch;
    for (const auto& entry : dictionary->getTextures())
    {
        const auto slot = entry.first;
        if (entry.second->mIsBakedTexture)
        {
            const auto bake = batch->results.find(slot);
            avatar.setTETexture(slot, bake == batch->results.end() ? IMG_DEFAULT_AVATAR : bake->second);
        }
    }
    avatar.updateMeshTextures();
    avatar.dirtyMesh();
    LL_INFOS("Outfit") << "Published complete legacy appearance with " << batch->eligible_slots.size()
                       << " bake slots" << LL_ENDL;
}

void queueLegacyAppearancePublication(const std::shared_ptr<LegacyBakeBatch>& batch)
{
    if (batch->publication_queued || batch->published || batch->eligible_slots.empty() ||
        batch->results.size() != batch->eligible_slots.size() ||
        std::any_of(batch->results.begin(), batch->results.end(),
                    [](const auto& result) { return result.second.isNull(); }))
    {
        return;
    }

    batch->publication_queued = true;
    // Leave the render target and any upload callback before touching avatar texture entries.
    doOnIdleRepeating([batch, wait = LLTimer()]()
    {
        if (!isCurrentBakeBatch(batch) || gDisconnected)
        {
            return true;
        }
        if (batch->avatar->isEditingAppearance() || batch->avatar->getIsAppearanceAnimating() ||
            !gAgentWearables.areWearablesLoaded())
        {
            if (wait.getElapsedTimeF32() < 15.f)
            {
                return false;
            }
            LL_WARNS("Outfit") << "Legacy appearance publication timed out waiting for the avatar" << LL_ENDL;
            return true;
        }

        publishLegacyBake(batch);
        return true;
    });
}

void completeLegacyBake(const std::shared_ptr<LegacyBakeBatch>& batch, ETextureIndex slot,
                        const LLUUID& asset)
{
    if (!isCurrentBakeBatch(batch))
    {
        return;
    }
    if (asset.isNull())
    {
        LL_WARNS("Outfit") << "Legacy bake upload failed for texture slot " << slot << LL_ENDL;
        return;
    }

    batch->results.at(slot) = asset;
    LL_INFOS("Outfit") << "Legacy bake upload completed for texture slot " << slot << LL_ENDL;
    queueLegacyAppearancePublication(batch);
}

void uploadLegacyBakeAsset(const std::shared_ptr<LegacyBakeBatch>& batch, ETextureIndex slot,
                           const LLTransactionID& transaction)
{
    if (!isCurrentBakeBatch(batch))
    {
        return;
    }
    if (!gAssetStorage)
    {
        completeLegacyBake(batch, slot, LLUUID::null);
        return;
    }

    gAssetStorage->storeAssetData(transaction, LLAssetType::AT_TEXTURE,
        [batch, slot](const LLUUID& asset, void*, S32 status, LLExtStat)
        {
            completeLegacyBake(batch, slot, status == 0 ? asset : LLUUID::null);
        },
        nullptr, true, true, true);
}

void queueLegacyBake(const std::shared_ptr<LegacyBakeBatch>& batch, ETextureIndex slot,
                     LLImageJ2C& image)
{
    LLTransactionID transaction;
    transaction.generate();
    const LLUUID asset = transaction.makeAssetID(gAgent.getSecureSessionID());
    LLFileSystem file(asset, LLAssetType::AT_TEXTURE, LLFileSystem::WRITE);
    if (!file.write(image.getData(), image.getDataSize()))
    {
        completeLegacyBake(batch, slot, LLUUID::null);
        return;
    }

    const std::string capability = gAgent.getRegionCapability("UploadBakedTexture");
    if (capability.empty())
    {
        uploadLegacyBakeAsset(batch, slot, transaction);
        return;
    }

    using Adapter = LLCoreHttpUtil::HttpCoroutineAdapter;
    const LLUUID queued = LLCoprocedureManager::instance().enqueueCoprocedure("Upload", "LegacyAvatarBake",
        [batch, slot, capability, transaction, asset](Adapter::ptr_t& adapter, const LLUUID&)
        {
            if (!isCurrentBakeBatch(batch))
            {
                return;
            }

            LLCore::HttpRequest::ptr_t request(new LLCore::HttpRequest);
            LLCore::HttpOptions::ptr_t options(new LLCore::HttpOptions);
            options->setRetries(0);
            options->setTimeout(60);
            const LLSD handshake = adapter->postAndSuspend(request, capability, LLSD::emptyMap(), options);
            if (!isCurrentBakeBatch(batch))
            {
                return;
            }

            const auto status = Adapter::getStatusFromLLSD(handshake[Adapter::HTTP_RESULTS]);
            const std::string uploader = handshake["uploader"].asString();
            const LLURI upload_uri(uploader);
            if (status && (upload_uri.scheme() == "http" || upload_uri.scheme() == "https") &&
                !upload_uri.hostName().empty() && upload_uri.userName().empty() &&
                upload_uri.password().empty() &&
                (LLURI(capability).scheme() != "https" || upload_uri.scheme() == "https"))
            {
                const LLSD response = adapter->postFileAndSuspend(
                    request, uploader, asset, LLAssetType::AT_TEXTURE, options);
                const auto upload_status = Adapter::getStatusFromLLSD(response[Adapter::HTTP_RESULTS]);
                const LLUUID uploaded = response["new_asset"].asUUID();
                if (upload_status && response["state"].asString() == "complete" && uploaded.notNull())
                {
                    completeLegacyBake(batch, slot, uploaded);
                    return;
                }
            }

            if (isCurrentBakeBatch(batch))
            {
                LL_WARNS("Outfit") << "Legacy bake capability upload failed for texture slot " << slot
                                   << "; trying temporary asset upload once" << LL_ENDL;
                doAfterInterval([batch, slot, transaction]()
                {
                    uploadLegacyBakeAsset(batch, slot, transaction);
                }, 1.f);
            }
        });
    if (queued.isNull())
    {
        LL_WARNS("Outfit") << "Could not queue legacy bake capability upload for texture slot " << slot
                           << "; trying temporary asset upload once" << LL_ENDL;
        doAfterInterval([batch, slot, transaction]()
        {
            uploadLegacyBakeAsset(batch, slot, transaction);
        }, 1.f);
    }
}
} // namespace

void beginLegacyBakeUploadBatch(LLVOAvatarSelf& avatar, LLViewerRegion& region,
                                const std::vector<LLAvatarAppearanceDefines::ETextureIndex>& slots)
{
    static const auto region_connection = gAgent.addRegionChangedCallback([]
    {
        active_bake_batch.reset();
        published_bake_batch.reset();
    });
    auto batch = std::make_shared<LegacyBakeBatch>();
    batch->region = region.getRegionID();
    batch->eligible_slots = slots;
    batch->supports_bom = (region.getRegionProtocols() & LEGACY_BAKE_BOM_PROTOCOL) != 0;
    batch->avatar = &avatar;
    batch->wearable_assets = getBakeWearableAssets();
    active_bake_batch = std::move(batch);
}


bool isObsoleteLegacySelfAppearance(const LLVOAvatar& avatar, const LLHost& sender,
                                    const LLTEContents& textures)
{
    LLViewerRegion* region = gAgent.getRegion();
    const auto batch = published_bake_batch;
    if (!batch || !region || region->getCentralBakeVersion() != 0 ||
        batch->avatar != &avatar || avatar.getRegion() != region || batch->region != region->getRegionID())
    {
        return false;
    }

    if (sender != region->getHost() ||
        (isCurrentBakeBatch(active_bake_batch) && active_bake_batch != batch))
    {
        return true;
    }

    return std::any_of(batch->results.begin(), batch->results.end(), [&](const auto& bake)
    {
        return static_cast<U32>(bake.first) >= textures.face_count ||
               textures.image_data[bake.first] != bake.second;
    });
}

// runway consolidate
extern std::string self_av_string();

//-----------------------------------------------------------------------------
// LLViewerTexLayerSetBuffer
// The composite image that a LLViewerTexLayerSet writes to.  Each LLViewerTexLayerSet has one.
//-----------------------------------------------------------------------------

// static
S32 LLViewerTexLayerSetBuffer::sGLByteCount = 0;

LLViewerTexLayerSetBuffer::LLViewerTexLayerSetBuffer(LLTexLayerSet* const owner,
                                         S32 width, S32 height) :
    // ORDER_LAST => must render these after the hints are created.
    LLTexLayerSetBuffer(owner),
    LLViewerDynamicTexture(width, height, 4, LLViewerDynamicTexture::ORDER_LAST, false),
    mNeedsUpdate(true),
    mNumLowresUpdates(0)
{
    mGLTexturep->setNeedsAlphaAndPickMask(false);

    LLViewerTexLayerSetBuffer::sGLByteCount += getSize();
    mNeedsUpdateTimer.start();
}

LLViewerTexLayerSetBuffer::~LLViewerTexLayerSetBuffer()
{
    LLViewerTexLayerSetBuffer::sGLByteCount -= getSize();
    destroyGLTexture();
    for( S32 order = 0; order < ORDER_COUNT; order++ )
    {
        LLViewerDynamicTexture::sInstances[order].erase(this);  // will fail in all but one case.
    }
}

//virtual
S8 LLViewerTexLayerSetBuffer::getType() const
{
    return LLViewerDynamicTexture::LL_TEX_LAYER_SET_BUFFER ;
}

//virtual
void LLViewerTexLayerSetBuffer::restoreGLTexture()
{
    LLViewerDynamicTexture::restoreGLTexture() ;
}

//virtual
void LLViewerTexLayerSetBuffer::destroyGLTexture()
{
    LLViewerDynamicTexture::destroyGLTexture() ;
}

// static
void LLViewerTexLayerSetBuffer::dumpTotalByteCount()
{
    LL_INFOS() << "Composite System GL Buffers: " << (LLViewerTexLayerSetBuffer::sGLByteCount/1024) << "KB" << LL_ENDL;
}

void LLViewerTexLayerSetBuffer::requestUpdate()
{
    restartUpdateTimer();
    mNeedsUpdate = true;
    mNumLowresUpdates = 0;
}

void LLViewerTexLayerSetBuffer::restartUpdateTimer()
{
    mNeedsUpdateTimer.reset();
    mNeedsUpdateTimer.start();
}

// virtual
bool LLViewerTexLayerSetBuffer::needsRender()
{
    llassert(mTexLayerSet->getAvatarAppearance() == gAgentAvatarp);
    if (!isAgentAvatarValid()) return false;

    const bool update_now = mNeedsUpdate && isReadyToUpdate();

    // Don't render if we don't want to (or aren't ready to) update.
    if (!update_now)
    {
        return false;
    }

    // Don't render if we're animating our appearance.
    if (gAgentAvatarp->getIsAppearanceAnimating())
    {
        return false;
    }

    // Don't render if we are trying to create a skirt texture but aren't wearing a skirt.
    if (gAgentAvatarp->getBakedTE(getViewerTexLayerSet()) == LLAvatarAppearanceDefines::TEX_SKIRT_BAKED &&
        !gAgentAvatarp->isWearingWearableType(LLWearableType::WT_SKIRT))
    {
        return false;
    }

    // Render if we have at least minimal level of detail for each local texture.
    return getViewerTexLayerSet()->isLocalTextureDataAvailable();
}

// virtual
void LLViewerTexLayerSetBuffer::preRenderTexLayerSet()
{
    LLTexLayerSetBuffer::preRenderTexLayerSet();

    // keep depth buffer, we don't need to clear it
    LLViewerDynamicTexture::preRender(false);
}

// virtual
void LLViewerTexLayerSetBuffer::postRenderTexLayerSet(bool success)
{

    LLTexLayerSetBuffer::postRenderTexLayerSet(success);
    LLViewerDynamicTexture::postRender(success);
}

// virtual
void LLViewerTexLayerSetBuffer::midRenderTexLayerSet(bool success)
{
    using namespace LLAvatarAppearanceDefines;
    const bool update_now = mNeedsUpdate && isReadyToUpdate();
    const auto batch = active_bake_batch;
    if (update_now && isCurrentBakeBatch(batch) &&
        getViewerTexLayerSet()->isLocalTextureDataFinal())
    {
        const auto slot = gAgentAvatarp->getBakedTE(getViewerTexLayerSet());
        const auto& slots = batch->eligible_slots;
        if (std::find(slots.begin(), slots.end(), slot) != slots.end() &&
            batch->results.emplace(slot, LLUUID::null).second)
        {
            if (!success || !mBoundTarget)
            {
                LL_WARNS("Outfit") << "Legacy bake composite unavailable for texture slot " << slot << LL_ENDL;
            }
            else if (!getViewerTexLayerSet()->isVisible())
            {
                // An entirely hidden slot requires the invisible placeholder, not an upload.
                batch->results.at(slot) = IMG_INVISIBLE;
                queueLegacyAppearancePublication(batch);
            }
            else
            {
                const S32 width = getCompositeWidth();
                const S32 height = getCompositeHeight();
                LLPointer<LLImageRaw> colors = new LLImageRaw(width, height, 4);
                LLPointer<LLImageRaw> mask = new LLImageRaw(width, height, 1);
                LLPointer<LLImageRaw> pixels = new LLImageRaw(width, height, 5);
                if (colors->getData() && mask->getData() && pixels->getData())
                {
                    glReadPixels(getCompositeOriginX(), getCompositeOriginY(), width, height,
                                 GL_RGBA, GL_UNSIGNED_BYTE, colors->getData());
                    stop_glerror();
                    getViewerTexLayerSet()->gatherMorphMaskAlpha(mask->getData(),
                        getCompositeOriginX(), getCompositeOriginY(), width, height, mBoundTarget);

                    const U8* rgba = colors->getData();
                    const U8* alpha = mask->getData();
                    U8* bake = pixels->getData();
                    for (S32 pixel = 0; pixel < width * height; ++pixel, rgba += 4, bake += 5)
                    {
                        std::copy_n(rgba, 4, bake);
                        bake[4] = alpha[pixel];
                    }

                    LLPointer<LLImageJ2C> image = new LLImageJ2C;
                    if (image->encode(pixels, LINDEN_J2C_COMMENT_PREFIX "RGBHM"))
                    {
                        queueLegacyBake(batch, slot, *image);
                    }
                    else
                    {
                        LL_WARNS("Outfit") << "Legacy bake encoding failed for texture slot " << slot << LL_ENDL;
                    }
                }
                else
                {
                    LL_WARNS("Outfit") << "Legacy bake readback allocation failed for texture slot " << slot
                                       << LL_ENDL;
                }
            }
        }
    }
    if (update_now)
    {
        doUpdate();
    }

    // *TODO: Old logic does not check success before setGLTextureCreated
    // we have valid texture data now
    mGLTexturep->setGLTextureCreated(true);
}

bool LLViewerTexLayerSetBuffer::isInitialized(void) const
{
    return mGLTexturep.notNull() && mGLTexturep->isGLTextureCreated();
}

bool LLViewerTexLayerSetBuffer::isReadyToUpdate() const
{
    // If we requested an update and have the final LOD ready, then update.
    if (getViewerTexLayerSet()->isLocalTextureDataFinal()) return true;

    // If we haven't done an update yet, then just do one now regardless of state of textures.
    if (mNumLowresUpdates == 0) return true;

    // Update if we've hit a timeout.  Unlike for uploads, we can make this timeout fairly small
    // since render unnecessarily doesn't cost much.
    const U32 TEXTURE_TIMEOUT = 10;
    if (TEXTURE_TIMEOUT != 0)
    {
        // If we hit our timeout and have textures available at even lower resolution, then update.
        const bool is_update_textures_timeout = mNeedsUpdateTimer.getElapsedTimeF32() >= TEXTURE_TIMEOUT;
        const bool has_lower_lod = getViewerTexLayerSet()->isLocalTextureDataAvailable();
        if (has_lower_lod && is_update_textures_timeout) return true;
    }

    return false;
}

bool LLViewerTexLayerSetBuffer::requestUpdateImmediate()
{
    mNeedsUpdate = true;
    bool result = false;

    if (needsRender())
    {
        preRender(false);
        result = render();
        postRender(result);
    }

    return result;
}

// Mostly bookkeeping; don't need to actually "do" anything since
// render() will actually do the update.
void LLViewerTexLayerSetBuffer::doUpdate()
{
    LLViewerTexLayerSet* layer_set = getViewerTexLayerSet();
    const bool highest_lod = layer_set->isLocalTextureDataFinal();
    if (highest_lod)
    {
        mNeedsUpdate = false;
    }
    else
    {
        mNumLowresUpdates++;
    }

    restartUpdateTimer();

    // need to switch to using this layerset if this is the first update
    // after getting the lowest LOD
    layer_set->getAvatar()->updateMeshTextures();

    // Print out notification that we updated this texture.
    if (gSavedSettings.getBOOL("DebugAvatarRezTime"))
    {
        const bool highest_lod = layer_set->isLocalTextureDataFinal();
        const std::string lod_str = highest_lod ? "HighRes" : "LowRes";
        LLSD args;
        args["EXISTENCE"] = llformat("%d",(U32)layer_set->getAvatar()->debugGetExistenceTimeElapsedF32());
        args["TIME"] = llformat("%d",(U32)mNeedsUpdateTimer.getElapsedTimeF32());
        args["BODYREGION"] = layer_set->getBodyRegionName();
        args["RESOLUTION"] = lod_str;
        LLNotificationsUtil::add("AvatarRezSelfBakedTextureUpdateNotification",args);
        LL_DEBUGS("Avatar") << self_av_string() << "Locally updating [ name: " << layer_set->getBodyRegionName() << " res:" << lod_str << " time:" << (U32)mNeedsUpdateTimer.getElapsedTimeF32() << " ]" << LL_ENDL;
    }
}

//-----------------------------------------------------------------------------
// LLViewerTexLayerSet
// An ordered set of texture layers that get composited into a single texture.
//-----------------------------------------------------------------------------

LLViewerTexLayerSet::LLViewerTexLayerSet(LLAvatarAppearance* const appearance) :
    LLTexLayerSet(appearance),
    mUpdatesEnabled( false )
{
}

// virtual
LLViewerTexLayerSet::~LLViewerTexLayerSet()
{
}

// Returns true if at least one packet of data has been received for each of the textures that this layerset depends on.
bool LLViewerTexLayerSet::isLocalTextureDataAvailable() const
{
    if (!mAvatarAppearance->isSelf()) return false;
    return getAvatar()->isLocalTextureDataAvailable(this);
}


// Returns true if all of the data for the textures that this layerset depends on have arrived.
bool LLViewerTexLayerSet::isLocalTextureDataFinal() const
{
    if (!mAvatarAppearance->isSelf()) return false;
    return getAvatar()->isLocalTextureDataFinal(this);
}

// virtual
void LLViewerTexLayerSet::requestUpdate()
{
    if( mUpdatesEnabled )
    {
        createComposite();
        getViewerComposite()->requestUpdate();
    }
}

void LLViewerTexLayerSet::updateComposite()
{
    createComposite();
    getViewerComposite()->requestUpdateImmediate();
}

// virtual
void LLViewerTexLayerSet::createComposite()
{
    if(!mComposite)
    {
        S32 width = mInfo->getWidth();
        S32 height = mInfo->getHeight();
        // Composite other avatars at reduced resolution
        if( !mAvatarAppearance->isSelf() )
        {
            LL_ERRS() << "composites should not be created for non-self avatars!" << LL_ENDL;
        }
        mComposite = new LLViewerTexLayerSetBuffer( this, width, height );
    }
}

void LLViewerTexLayerSet::setUpdatesEnabled( bool b )
{
    mUpdatesEnabled = b;
}

LLVOAvatarSelf* LLViewerTexLayerSet::getAvatar()
{
    return dynamic_cast<LLVOAvatarSelf*> (mAvatarAppearance);
}

const LLVOAvatarSelf* LLViewerTexLayerSet::getAvatar() const
{
    return dynamic_cast<const LLVOAvatarSelf*> (mAvatarAppearance);
}

LLViewerTexLayerSetBuffer* LLViewerTexLayerSet::getViewerComposite()
{
    return dynamic_cast<LLViewerTexLayerSetBuffer*> (getComposite());
}

const LLViewerTexLayerSetBuffer* LLViewerTexLayerSet::getViewerComposite() const
{
    return dynamic_cast<const LLViewerTexLayerSetBuffer*> (getComposite());
}


const std::string LLViewerTexLayerSetBuffer::dumpTextureInfo() const
{
    if (!isAgentAvatarValid()) return "";

    const bool is_high_res = true;
    const U32 num_low_res = 0;
    const std::string local_texture_info = gAgentAvatarp->debugDumpLocalTextureDataInfo(getViewerTexLayerSet());

    std::string text = llformat("[HiRes:%d LoRes:%d] %s",
                                is_high_res, num_low_res,
                                local_texture_info.c_str());
    return text;
}
