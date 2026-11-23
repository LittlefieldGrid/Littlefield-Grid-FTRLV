# Corresponding-source locations and delivery plan

This is a provenance/delivery ledger, not a claim that all materials have been supplied.
The completed dependency download/build verification was not repeated. A wrapper
repository revision does not automatically identify all vendored or fetched preferred source.

## Known exact anchors

| Component | Exact source location | Still needed for installer clearance |
|---|---|---|
| Viewer 7.2.0.61057 | This complete staged source; Kokua base `0389bdfa8907386fd0a9bccd10ec0c2d79ce010b` | Publish/tag immutable release source only when authorized; identify staged notice/artwork-only differences from the binary source. Retain eight fixes and build/install scripts. |
| VLC 3.0.21 | https://download.videolan.org/pub/videolan/vlc/3.0.21/vlc-3.0.21.tar.xz | Archive hash verified. Deliver its source plus the plugin transitive sources, patches, configuration and install scripts. |
| VLC packaging wrapper | https://github.com/secondlife/3p-vlc-bin/tree/e60ee26d93fb3ac18282534a355645f02369dc43 | Its vendor DLL blobs match all 367 shipped VLC DLLs; wrapper alone is not complete source. |
| VLC-contrib sources | https://download.videolan.org/pub/contrib/ and the pinned source archive contrib/src rules/patches | Match every applicable plugin to exact source versions/patches; preserve checksum files. Directory availability alone is insufficient. |
| FFmpeg in VLC | https://ffmpeg.org/releases/ffmpeg-4.4.4.tar.xz | Actual plugin strings say 4.4.4; pinned VLC rule/checksum retained. Include VLC contrib patches, build flags and statically linked dependency materials. |
| x264 in VLC | Pinned source archive contrib/src/x264 rules; https://download.videolan.org/pub/videolan/x264/snapshots/x264-snapshot-20180324-2245.tar.bz2 | This is the declared fallback snapshot, not proof the DLL used it. Establish actual checkout/revision and flags. |
| x265 in VLC | Pinned source archive contrib/src/x265 rules; https://bitbucket.org/multicoreware/x265_git/downloads/x265_2.9.tar.gz | Declared recipe version 2.9; verify actual linked source and patches rather than assume. |
| Dullahan | https://github.com/secondlife/dullahan/tree/ead65e7ba782ccd4c9e1bb1e6247484ffbe980b1 | MIT notice matches packaged notice. Preserve attribution; CEF source is separate. |
| CEF | https://github.com/chromiumembedded/cef/tree/3dd607849786b271d95e1a3e8013f59572fa9779 | Complete Chromium checkout and applicable CEF patches/configuration. Exact short revision resolved authoritatively. |
| CEF packaging/build wrapper | https://github.com/secondlife/cef/tree/9522e46a765a5eedc4bbe238896da5f7845bbf72 | Dullahan manifest pins this package; wrapper enables proprietary codecs. Preserve Windows build recipe and effective GN/toolchain args; moving branch alone is insufficient. |
| Chromium | https://chromium.googlesource.com/chromium/src/+/refs/tags/118.0.5993.54 | Complete pinned DEPS dependency sources and build materials, not merely top-level source. |
| Chromium FFmpeg | https://chromium.googlesource.com/chromium/third_party/ffmpeg/+/0ba37733400593b162e5ae9ff26b384cff49c250 | Revision from exact Chromium DEPS; include generated config/patches and usable LGPL relink/rebuild materials. |
| Microsoft DLLs | Official release/SDK redistribution channel under applicable terms | Proprietary components: no corresponding-source obligation was identified. Exact older-copy authority and downstream terms still need documentation; signatures alone are not license grants. |

VLC source checksum and official Windows ZIP URL/hash are retained in publication-audit/RETRIEVAL.json.
No complete installer source bundle or operational written offer is asserted.

## All 43 package source references

These are exact package-level references from the audited lock inventory. They are
not an assertion that every package ships or that every wrapper contains all sources.
Missing revision data stays explicit. In particular, private stub-repo metadata must
be replaced by evidence of the actual sources/build files in its public archive.

| Package | Version | Package-level source | Revision |
|---|---|---|---|
| apr_suite | 1.7.4-10338381102 | https://github.com/secondlife/3p-apr_suite | `945d742f59a908ec7fac83c3144dc6fd1a1f4cbe` |
| boost | 1.86 | https://github.com/secondlife/3p-boost | `e2bee1e1b4c8a101134df6863a491d54bd933025` |
| colladadom | 2.3.0-r8 | https://github.com/secondlife/3p-colladadom | `60a1fc06fdb7da878ccd1b10a4950d55802b3673` |
| cubemaptoequirectangular | 1.1.0 | https://github.com/secondlife/3p-cubemap_to_eqr_js | `d7afe27f1184445fb2a6f6ce98d65ccb93f8ac4c` |
| curl | 7.54.1-10342910827 | https://github.com/secondlife/3p-curl | `4dd010c35e56b5b36e47a0aa2646b72a380a5794` |
| dictionaries | 1.a01bb6c | https://github.com/secondlife/3p-dictionaries-git | `a01bb6c48927c44e138382a608ad805c790819a9` |
| dullahan | 1.14.0.202408091639_118.4.1_g3dd6078_chromium-118.0.5993.54 | https://github.com/secondlife/dullahan | `ead65e7ba782ccd4c9e1bb1e6247484ffbe980b1` |
| emoji_shortcodes | 15.3.2.10207138275 | https://github.com/secondlife/3p-emoji-shortcodes | `35e785db417237795d49582aa8462e9647b97f1d` |
| expat | 2.6.2-r5 | https://github.com/secondlife/3p-expat | `73123f6b06c94246395e80c4c8c395846896f442` |
| freetype | 2.13.3-cb2e120 | https://github.com/secondlife/3p-freetype | `cb2e120693c93e30537a1728b8a18cacd52dccb7` |
| glext | 68 | https://github.com/secondlife/3p-glext | `685b36e812a9aabf3b52ec548515251553539f67` |
| glh_linear | 1.0.1-dev4 | https://github.com/secondlife/3p-glh_linear | `984c3971bd7b442e37d20b0fb85b4df3e1c9ee31` |
| glm | v1.0.1 | https://github.com/secondlife/3p-glm | `399cd5ba57a9267a560ce07e50a0f8c5fe3dc66f` |
| jpegencoderbasic | 1.0 | https://github.com/secondlife/3p-jpeg_encoder_js | `9165e472422d94ee196b35ec9384982703eb17c0` |
| libhunspell | 1.7.2.10207243663 | https://github.com/secondlife/3p-libhunspell | `ebe43929028a244c1e2ff84b407b65ffc7e346ef` |
| libjpeg-turbo | 3.0.3-r2 | https://github.com/secondlife/3p-libjpeg-turbo | `02d1aa1af062afe5356569639b9dcc928f2a011c` |
| libndofdev | 0.1.8e9edc7 | https://github.com/secondlife/3p-libndofdev | `8e9edc71886af143e0446da12273b2aa21baefb1` |
| libpng | 1.6.43-r2 | https://github.com/secondlife/3p-libpng | `5de9b9bdb4a4e353ce83e09af82a5c94f487cfcd` |
| libxml2 | 2.13.3-r1 | https://github.com/secondlife/3p-libxml2 | `8c41b2cba1c2e849a58ab7c380c0096eecc17a1b` |
| llca | 202407221423.0 | https://github.com/secondlife/llca | `a0fd5b9f0041b7d1319d5736f5ee2ee0ab0e87a0` |
| llphysicsextensions_stub | 1.0.542456 | https://bitbucket.org/lindenlabinternal/llphysicsextensions_stub | `Not supplied` |
| meshoptimizer | 210.0.0-r2 | https://github.com/secondlife/3p-meshoptimizer | `0750291cae00d2c15a11c475ae80197fb9430dee` |
| mikktspace | 1 | https://github.com/secondlife/3p-mikktspace | `e967e1bc8eb2642d33137525fdb51368080a61e4` |
| minizip-ng | 4.0.7-r1 | https://github.com/secondlife/3p-minizip-ng | `9fc435a59e7b609ad0b541d236a01d5aac874908` |
| nanosvg | 2022.09.27 | https://github.com/secondlife/3p-nanosvg | `Not supplied` |
| nghttp2 | 1.62.1 | https://github.com/secondlife/3p-nghttp2 | `caeded2ca8d7f56399609a327901fd387369daa8` |
| nvapi | 560.0.0-r1 | https://github.com/secondlife/3p-nvapi | `575aae9c470a10cce8549954b731c6fff44f40b6` |
| ogg_vorbis | 1.3.5-1.3.7.10341271136 | https://github.com/secondlife/3p-ogg_vorbis | `8ceaf768c922b9af65c2f46b689f69a25391433f` |
| openal | 1.24.2-r1 | https://github.com/secondlife/3p-openal-soft | `e33e940c00e8447bcf164dc6df3aa6096b86946b` |
| openjpeg | 2.5.2.10604495243 | https://github.com/secondlife/3p-openjpeg | `37fac3d38377f84d7ea13843d378d23db4ba5065` |
| openssl | 1.1.1w | https://github.com/secondlife/3p-openssl | `6f9998d618a3cfa411b476635ae875f261615574` |
| openxr | 1.1.40-r1 | https://github.com/secondlife/3p-openxr | `279266ba4ff6f6545fb20e86c0146f1c6ffa1712` |
| threejs | 0.132.2 | https://github.com/secondlife/3p-three_js | `b8f6746f40efadcead303158636632fb918e6da2` |
| tinyexr | 1.0.9-5e8947c | https://github.com/secondlife/3p-tinyexr | `5e8947cb6780eab69742b9cd1c46850db58ab247` |
| tinygltf | 2.9.3-r1 | https://github.com/secondlife/3p-tinygltf | `c1c79870f656fcffc30ec3c3f5927b6bc997db83` |
| tut | 2008.11.30 | https://github.com/secondlife/3p-tut | `409bce514cc8d89dde2842c78088a4fde0d3d666` |
| viewer-fonts | 1.0.0.10204976553 | https://github.com/secondlife/3p-viewer-fonts | `7a412d5adefd67b3b34ec50a2ace274d6ecb03bd` |
| viewer-manager | 3.0-f14b5ec | https://github.com/secondlife/viewer-manager | `f14b5ecad00301c8297bd474c550c62260a169cc` |
| vlc-bin | 3.0.21.10218721728 | https://github.com/secondlife/3p-vlc-bin | `e60ee26d93fb3ac18282534a355645f02369dc43` |
| vulkan_gltf | 1.0.0 | https://github.com/secondlife/3p-vulkan-gltf-pbr | `d7c372f8a7483510396f5dd5a9e0369c5db1d855` |
| webrtc | m114.5735.08.72-test.10444682919 | https://github.com/secondlife/3p-webrtc-build | `cd42b814538ef81899d9ed3513ceebd6555cdc79` |
| xxhash | 0.8.2-10285735820 | https://github.com/secondlife/3p-xxhash | `27b1ae758dbc6a79960a23e80fba546f43dc0e22` |
| zlib-ng | 2.2.1-r2 | https://github.com/secondlife/3p-zlib-ng | `69f639218cde9f38a7dc8df458084f2bb0ba06cb` |

## Delivery route before binary publication

Prefer complete versioned source/patch/build bundles accessible alongside the binary,
with hashes and a per-component license/relink ledger. For GPL 2 section 3(a), source
must cover the program and required build/install material; equivalent same-place
source download can qualify as stated in the license. For LGPL 2.1 sections 4/6,
meet both library-source and linked-work conditions. See LGPL_COMPLIANCE.md.
A source offer is an alternative with actual obligations, not a documentation shortcut.
Do not issue one before the complete materials and fulfillment capacity exist.
The inherited third-party Vivox paragraph and generic upstream links are insufficient.
No email was sent, no offer issued and no source-hosting service created by this task.
