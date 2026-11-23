> Clearance update (2026-11-22): **PUBLIC SOURCE PASS; WINDOWS INSTALLER FAIL / NOT CLEARED.**
> Source-only hosting is independent of binary distribution. The earlier overall
> hold is superseded for source hosting. See [publication clearance](doc/PUBLICATION_CLEARANCE.md)
> for updated evidence and remaining installer actions. No build or static suite was repeated.

# Provenance

Prepared 2026-11-22 from the completed Littlefield Grid FTRLV development worktree.
The development worktree, its Git history, correction reports, final installer and
installed viewer were not changed by this source preparation.

## Upstream

- Kokua source tag: `Kokua_Release_RLV_7.2.0.61057`.
- Exact source commit: `0389bdfa8907386fd0a9bccd10ec0c2d79ce010b`.
- Upstream project: https://bitbucket.org/kokua/kokua.git
- The local development repository was shallow. Only one upstream commit was
  reachable; this snapshot does not represent a complete historical audit or clone.
- All 621 tracked working-tree modifications and all ten required new Littlefield
  files were included before adding publication notices/documentation.
- The new source tree contains no original `.git` or Mercurial metadata and no
  public Git history has been initialized or rewritten.

## Build variables and actual compiler settings

The established build used https://bitbucket.org/kokua/viewer-build-variables.git
at `11e3b347e5f17214db9c6cf9f0f69f346e631e43` (clean checkout).
The public build helper records the effective Windows Release compiler switch
expansion instead of silently depending on that external local checkout.
It explicitly reproduces the successful build's `/Zc:wchar_t-` ABI override; the
pinned upstream variables use `/Zc:wchar_t`. See scripts/windows-public-variables.
This file is a minimal, documented project build recipe, not a copy of the entire
external variables repository, whose independent licensing was not established.

The build ID is explicitly `revision=61057`; it must not be recomputed from a new
public repository's initial commit count. The channel remains
`Littlefield Grid Kokua FTRLV Edition`; runtime branding remains Littlefield Grid FTRLV.

## Immutable binary reference

- Installer: `Littlefield-Grid-FTRLV-7.2.0.61057-x64-Setup.exe`.
- Size: 166,910,702 bytes.
- SHA-256: `E69392E0D16091ED9E2BD9C2B8904B0FD949FC8C6A2863E3607FAAAB5903C3DB`.
- This installer is not included, modified, installed or uploaded by this preparation.
- Its redistribution is NOT cleared merely because the viewer source was staged.

## Publication preparation changes

Only the staged copy gained dated modification comments, public documentation,
sanitized manifests, independent validation/build helpers and an appropriate ignore
file. Original upstream copyright and license headers remain. A private validation
manifest beside this source tree reversibly checks each added notice against the
development copy. Functional viewer code and the eight corrections are unchanged. A staged-only build fix removes the stale unconditional res/ll_icon.ico HEADER_FILE_ONLY project entry; the LFGrid resource already uses littlefield.ico directly. The first clean generation exposed this cached-resource dependency.

Upstream CI, local Git/Mercurial metadata and the legacy compiled VSTool.exe were
excluded. The entire unused historical VSTool automation helper is omitted from this curated snapshot, including its C# project/solution and borrowed-code sources. The documented modern VS2022 CMake build does not reference it. Original development files remain untouched; omitted files are listed in doc/modified-source.json.

## Artwork clarification

The owner declared the LFGrid logo/icon self-created and authorized CC BY-SA 3.0. The staged webrtc.png is now a byte-exact official press-kit PNG with pinned origin and notice. This documented staging-only artwork replacement leaves the original source icon and final installer untouched; see doc/ARTWORK_PROVENANCE.md.

## Public package metadata

Only the top-level autobuild package name, description, prospective canonical repository URL, and incorrect docs/LICENSE-source.txt path were corrected for this public snapshot. The actual license path is doc/LICENSE-source.txt. Upstream copyright metadata, installable dependencies, archive pins and build variants were retained unchanged. The GitHub URL is a proposed publication target, not a claim that a repository was created.

## Build-generated copies excluded

The 19 font, emoji and icon assets generated/copied during clean CMake validation were removed from the public snapshot and listed exactly in .gitignore and doc/modified-source.json. No maintained original source/resource was removed. Fresh builds regenerate these files.
