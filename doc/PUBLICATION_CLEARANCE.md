# Final publication clearance — 2026-11-22

| Distribution being assessed | Result | Scope |
|---|---|---|
| PUBLIC SOURCE | **PASS** | The curated source/resources/notices in this directory may be published independently. No installer or dependency binaries are included. |
| WINDOWS INSTALLER | **FAIL — NOT CLEARED** | Complete covered corresponding-source/relinking delivery and specific Microsoft provenance/terms evidence remain incomplete; original raster provenance assessment remains qualified. |

This is an evidence-based publication review, not a court ruling or an attorney's
opinion. Uncertain conclusions are identified below. PASS applies to source hosting
with its existing licenses/notices, not to attaching the installer to a GitHub
release or claiming complete compliance for every binary dependency.

## Why source publication can proceed

The staged source preserves the core LGPL 2.1 license, per-file exceptions,
copyright/credit notices and dated modification notices. Owner-authored LFGrid
artwork is CC BY-SA 3.0; the staged WebRTC raster has a pinned official origin and
retained notice. The unverified optional legacy helper and active upstream Actions
were excluded during completed preparation. That completed scope is unchanged.

LGPL 2.1 sections 1–2 govern source copying/modification. Object-code and linked-work
delivery duties in sections 4 and 6 do not require a source-only repository to
include the installer or copies of every prebuilt dependency. References to public
dependency download locations are not redistribution of those binaries by this
repository. The PASS depends on continuing to exclude them and retaining notices.
The source repository may be published before the installer is cleared. A future
installer upload requires its own completed compliance package and decision.

Authority: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html . The retrieved
unaltered license is also retained in publication-audit/LGPL-2.1-authoritative.txt.
No source-publication requirement for a new written offer was identified when the
complete covered source itself is being provided. No offer has been issued here.

## VLC and 365 plugins

All **367 DLLs** (365 plugins plus libvlc/libvlccore) match both the pinned vendor
Git blobs and the official VideoLAN 3.0.21 Windows ZIP byte for byte. Both official
binary ZIP and source archive SHA-256 match VideoLAN's published checksum files.
See publication-audit/VLC-COMPONENT-AUDIT.json and RETRIEVAL.json.

This resolves binary-origin uncertainty. Official VLC 3.0.21 source exists, including
module sources, build infrastructure, contrib recipes/patches and GPL/LGPL texts.
Actual FFmpeg strings identify 4.4.4 and static dependency build flags. However,
this task has not assembled the preferred sources of every transitive plugin
dependency, their exact patches/configuration, or a usable source/relinking delivery
set. The 3p-vlc-bin wrapper copies vendor binaries; it is not that source set.

The installer contains LICENSES/vlc.txt (GPL 2). That does not make all libVLC code
GPL-only: the official source separately supplies COPYING.LIB (LGPL 2.1). Nor does
it grant all transitive libraries the same license. The official VLC source notice
and additional COPYING.LIB have been retained in publication-audit. Exact routes
and remaining materials are listed in CORRESPONDING_SOURCE.md.

**Required action before binary publication:** collect the matching VLC/contrib
sources, patches and build/install scripts; cover the complete plugin inventory;
provide them with/equivalently alongside the binary using a valid license route.
Alternatively issue an appropriately fulfilled written offer only after the full
materials and operational capacity exist. A generic upstream URL or the inherited
Vivox source-request paragraph is not an LFGrid source offer. No upstream contact
has been made and no future fulfillment promise is created by this review.

## LGPL viewer and libraries

See LGPL_COMPLIANCE.md for the source, static-link, DLL-replacement, notice and
offer distinctions. Publishing this source snapshot is a permitted source-only
activity. For the installer, the viewer's full source is available in staging but
the complete LGPL dependency source/relinking materials have not all been mapped
and delivered. DLL use can satisfy a linked-work mechanism condition without
eliminating the separate source obligation for distributing the library DLL.

## CEF/Chromium: the missing-notices claim is corrected

The bundled CEF notice matches the exact CEF source license after line-ending
normalization. More importantly, actual distributed resources.pak contains Brotli
resource 37031: a **9,121,546-byte Credits HTML page with 344 component entries**,
including FFmpeg, ANGLE, ICU, V8/other Chromium components and license texts.
Its extracted SHA-256 is
`912385ff7b47e4bfe5d6f1b52372d71b4c7ab40806c041434fc48c39134fe533`.

The exact page, component index, resource hashes and other embedded notice
resources are retained in publication-audit. This is stronger evidence than a
generic current web license list. The earlier assertion that only CEF's short BSD
notice was bundled was incomplete. **Absence of Chromium third-party notices is
not a demonstrated blocker.** Providing the extracted page as accompanying
materials improves visibility without changing the installer. No live browser
accessibility test was performed or is claimed.

The generated page is not proof of source/relinking fulfillment. The actual CEF
line is 118.4.1+g3dd6078 / Chromium 118.0.5993.54. Its wrapper enables proprietary
codecs; the credits include FFmpeg's LGPL notice. Exact source pins and the wrapper
are identified in CORRESPONDING_SOURCE.md. Complete LGPL-containing CEF dependency
materials/rebuild or relink route remain to be delivered. A BSD top-level CEF
license cannot override those component conditions. No separate patent clearance
is inferred from a copyright license or the codec flag; jurisdiction/use-specific
questions, if material to the distribution, need qualified review rather than an
automatic claim that a paid license is required.

## Microsoft redistribution

Microsoft's VS2022 Distributable Code list permits unmodified release VC runtime
redistribution by validly licensed Visual Studio users, subject to its terms.
The Community terms allow individual development and OSI-licensed development by
organizations. The v14 family spans VS2017–2026. No separate fee requirement for
these eligible runtime files was identified.

All **11 shipped VC runtime locations** (8 distinct filenames, 3 duplicate copies)
report version **14.50.35719.0**, with valid Microsoft Authenticode signatures.
They differ from the installed VS2022 redist (14.44) and the current official
download (14.52.37029.0). A version difference is not proof of tampering or a license
violation. It also does not establish that the exact copies came through an
authorized redistribution route. The inherited build permits a System32 fallback.

**Remaining evidence/terms action:** identify an official matching 14.50 release
or otherwise document the specific copies' redistribution authorization and comply
with Microsoft component protection/end-user terms. If that route cannot be
documented, use verified permitted runtime copies in a separately generated future
installer. This usually requires repackaging, not viewer recompilation. Do not
apply Microsoft restrictions to the LGPL viewer or remove its reverse-engineering
rights. The SDK D3D compiler DLL is a separate component; its redist family is
permitted for desktop apps, but its older exact-copy provenance also remains to
be documented. See MICROSOFT_REDISTRIBUTION.md and the hash/signature inventories.

## Original WebRTC raster

The original 1280x239 raster visibly depicts the official horizontal WebRTC design
without an apparent added design. Commons attributes its SVG to the official press
kit under a BSD notice, and that Commons SVG is byte-identical to the pinned
official press-kit SVG. The original raster is not byte- or pixel-identical to
the currently generated Commons thumbnail, including after alpha compositing.
It plausibly reflects another mechanical rendering; exact creation history remains
unverified. See WEBRTC_RASTER_REVIEW.md.

BSD permits modified derivatives with its notice conditions, so different hashes
alone do not require a replacement. The official press page expressly permits
logo use in communications. These support a derivative assessment, not a proved
chain of creation for this exact PNG. The existing binary also contains the WebRTC
authors' BSD-style software notice, but it does not explicitly identify the logo.

**Disposition:** no demonstrated requirement to rebuild solely for this image,
and no unconditional legal clearance for the untraced raster. Before binary
publication, accept the documented derivative/notice assessment through qualified
review, obtain origin/permission confirmation, or replace it with the verified
staged asset in a new installer. Only the replacement option necessarily needs a
new installer; a source compile may be unnecessary for an asset-only repack.

## Blockers, gaps and optional improvements

| Item | Classification | Exact corrective action |
|---|---|---|
| Covered binary source/relinking set not complete or delivered | Installer compliance blocker | Finish the component-specific source/patch/build ledger and supply the materials using the applicable GPL/LGPL route. |
| Microsoft exact-copy authority and downstream terms | Installer evidence/terms hold, not proof of forbidden runtime use | Match official older release copies or get authority confirmation; document terms, or repackage verified redist files. |
| Original raster creation history | Qualified provenance hold | Review licensed-derivative assessment, obtain confirmation, or use verified replacement in a separate installer. |
| Chromium notices allegedly missing | Resolved audit finding | Retain the actual embedded page; extracted accompanying HTML is prepared. |
| Hosting the curated source independently | Cleared | Publish the staged source with its notices, without installer/dependency binaries. No push is authorized now. |
| A new written offer | Optional alternative, not mandatory | Prefer completed same-place source access. Do not issue an unsupported promise. |
| Rebuilding every dependency solely to publish source | Optional improvement | Not required for this source-only repository. |
| Repackaging the immutable installer | Conditional remedy | Necessary only if chosen fixes must change its contents; do not overwrite it. |

No build, static correction suite, secret scan, installation or packaging was
repeated during clearance. Only notices/documentation and their file manifest were
updated. No GitHub repository was created or pushed. The earlier DRAFT ZIP is
unchanged and predates this documentation; the staging directory is authoritative.
