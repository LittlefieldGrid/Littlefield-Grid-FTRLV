# Clean public-source build: PASS

On 2026-11-22 the staged Windows x64 Release viewer configured and compiled with
Visual Studio 2022 / MSVC 14.44.35207, SDK 10.0.26100.0 and the documented helper.
Both final configure and compile exit codes were zero. Version remains 7.2.0.61057.
The viewer executable and llwebrtc.dll exist and are nonempty outside this source
package; hashes and sizes are recorded in CLEAN_BUILD_EVIDENCE.json.

The attempt started with empty build/download directories. All 43 pinned dependency
archives were downloaded anonymously, with no development dependency cache or private
credentials supplied. Archive hashes and versions were verified. This is a clean
viewer build against public prebuilt dependencies, not a build of all dependencies
from corresponding source and not a fresh OS/toolchain provisioning test.

The first configuration exposed an inherited unused res/ll_icon.ico entry in the
HEADER_FILE_ONLY project list. The actual LFGrid Windows resource already uses
littlefield.ico. Removing only the stale list entry in staging allowed a successful
full compile. The same attempt's public downloads were reused after that fix. A
subsequent incremental configure/build passed after publication-only top-level
autobuild metadata edits; earlier failed and full-build logs are preserved outside
the public snapshot. No original cache was substituted.

CMake copies 19 font, emoji and icon assets into the source directory during a
build. Those generated copies were excluded from this curated package after build
validation and have exact .gitignore entries. Maintained inputs remain intact;
future builds regenerate the copies from pinned dependencies/maintained artwork.

Nonfatal diagnostics include inherited CMake deprecation/developer messages and
the linker /LTCG-without-code-generation advisory. No compilation errors remain.
Packaging, installation and interactive grid/voice/graphics tests were not run.
Static validation covers the eight corrections; runtime behavior still needs
interactive testing. Technical build success does not resolve binary licensing.
