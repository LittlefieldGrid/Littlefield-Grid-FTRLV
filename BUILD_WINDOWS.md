# Windows 11 / Visual Studio 2022 build

This procedure reproduces the established Windows x64 Release configuration from
the staged source. A separate clean attempt and its actual result are recorded in
doc/CLEAN_BUILD_RESULT.md. Do not claim a clean build succeeded unless that record
shows configure and compile exit code zero.

## Toolchain

The successful original release used Visual Studio 2022 Community, MSBuild
17.14.60, MSVC 14.44.35207, Windows SDK 10.0.26100.0, CMake 4.5.0, Python 3.13.17,
autobuild 3.10.2 and NSIS 3.13 for separate packaging. Install the VS Desktop C++
workload and a Windows SDK. Use a supported x64 environment; no original build cache,
installed viewer, GitHub personal token, signing identity or server credential is needed.

Use your own Python environment; do not copy the development machine's environment:

```powershell
py -3.13 -m venv ..\ftrlv-tools
..\ftrlv-tools\Scripts\python.exe -m pip install autobuild==3.10.2
```

Put Git and CMake on PATH. The helper can also locate autobuild.exe beside the Python
interpreter. CMake uses the Visual Studio 17 2022 generator and x64 platform.

## Configure and build from an empty directory

From this source root:

```powershell
..\ftrlv-tools\Scripts\python.exe scripts\build-public-windows.py `
  --build-dir ..\ftrlv-clean-build --cache-dir ..\ftrlv-clean-downloads
```

The default includes Release compilation after successful configuration. For a
configure/download-only diagnostic use --configure-only. The build and installable
cache must start empty. This is a fresh viewer build using publicly downloaded,
manifest-pinned prebuilt dependencies, not a claim to rebuild all third-party
libraries from preferred source. The helper refuses the original development tree
as its source/build/cache location and does not install a viewer.

The helper documents/applies these settings:

- revision=61057 and AUTOBUILD_BUILD_ID=61057; never use a new public commit count.
- LL_BUILD from scripts/windows-public-variables, explicitly including the successful
  /Zc:wchar_t- ABI override. Original variables revision is in PROVENANCE.md.
- CMAKE_BUILD_TYPE=Release; ADDRESS_SIZE=64; Visual Studio 17 2022 / x64.
- VIEWER_CHANNEL=Littlefield Grid Kokua FTRLV Edition; runtime app identity unchanged.
- NVAPI=ON, RLV_ALWAYS_ON=ON, USE_OPENAL=ON, ENABLE_MEDIA_PLUGINS=ON.
- USE_VIVOX, USE_FMODSTUDIO, USE_KDU, USE_BUGSPLAT, USE_TRACY, USE_LTO,
  HAVOK, HAVOK_TPV, INSTALL_PROPRIETARY, PACKAGE, LL_TESTS, ENABLE_SIGNING=OFF.
- No inherited credentials, old AUTOBUILD cache, prefix path or private package dir.
- AUTOBUILD_INSTALLABLE_CACHE points only to the explicit empty new download cache.
- GIT_TERMINAL_PROMPT=0; Git credential helpers/global configuration disabled for
  this subprocess; common token/secret/service-account environment names removed.
- cmake --build <new-build> --config Release --target kokua-bin -- /m:2 /nologo
  /verbosity:minimal; appended _CL_=/MP2 bounds compiler workers without changing
  optimization/features. The full commands and public dependency failures are logged.

Expected output after successful compilation is newview\Release\kokua-bin.exe and
llwebrtc\Release\llwebrtc.dll in the new build directory. Packaging renames the
viewer to LittlefieldGridFTRLV.exe. Validate version 7.2.0.61057 before packaging.

## Separate packaging, only after license clearance

Packaging is not performed by the clean build helper. The established process runs
indra/newview/viewer_manifest.py with --actions="copy package", --arch=x86_64,
--buildtype=Release, --configuration=Release, --vivox=OFF, --openal=ON,
--fmodstudio=OFF, --tracy=OFF, the established channel and the new build's
newview/viewer_version.txt. Supply absolute source/artwork/build/dest paths.
Set MAKENSIS to your NSIS 3.13 makensis.exe. Run from an isolated working directory
because the manifest writes build_data.json to its working-directory parent.
Use a new destination and refuse to overwrite an existing installer.

The existing littlefield_installer.nsi retains C:\Program Files\LFGFTRLV and separate
Desktop/Start Menu options. Do not run setup automatically. The final historical
installer is not input to the build and must not be overwritten. Binary redistribution
is still pending the source/notice/artwork checks in THIRD_PARTY_NOTICES.md.

## Verification

```powershell
..\ftrlv-tools\Scripts\python.exe scripts\validate-public-source.py
```

Do not silently replace an unavailable dependency with an old local archive. Record
the exact public URL and failure. Updating a URL requires evidence that its content
matches the pinned package hash. No private package token is an acceptable workaround
for this public-source validation. Live-grid, voice, graphics, permissions, RLV,
Hypergrid and purchase behavior need separate interactive validation.

## Resuming the recorded clean attempt

After a documented source/build fix, --resume-public-attempt can reuse only the explicitly named cache/install tree from this helper's recorded fresh public attempt (or incremental revalidation of a completed attempt). It preserves the earlier logs and records the reuse. This is not permission to copy dependencies from the development cache. A new independent checkout should still use empty build/cache directories.

The inherited general autobuild configurations include proprietary variants and are not this public Release recipe. Use the documented helper and its explicit feature settings.

CMake generates source-directory font/emoji/icon copies during configuration. Their exact paths are ignored, and they are excluded from the curated source manifest. Run the manifest-based source validator on a pristine/extracted snapshot, not on a post-build directory containing these generated files.
