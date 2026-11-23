> Clearance update (2026-11-22): **PUBLIC SOURCE PASS; WINDOWS INSTALLER FAIL / NOT CLEARED.**
> Source-only hosting is independent of binary distribution. The earlier overall
> hold is superseded for source hosting. See [publication clearance](doc/PUBLICATION_CLEARANCE.md)
> for updated evidence and remaining installer actions. No build or static suite was repeated.

# Littlefield Grid FTRLV 7.2.0.61057

Local public-source staging snapshot for the proposed repository
`LittlefieldGrid/Littlefield-Grid-FTRLV`. No GitHub repository or release was created
by preparing this directory. **This curated source snapshot is cleared for source-only publication; the Windows installer is not cleared.**
See [publication status](doc/PUBLICATION_STATUS.md) for the separate installer holds.

This independent Littlefield Grid viewer is based on Kokua, Linden Lab viewer code,
and the credited third-party contributors. It preserves the existing FTRLV/RLV
implementation (no new RLVa integration), WebRTC-only WolfVoice client integration,
NVAPI, Hypergrid, PBR, LFGrid Search, profiles and settings. No grid server source or
production server configuration is needed or included.

The core source retains its original per-file licensing, predominantly LGPL 2.1
only; this is not a claim that all bundled dependencies or artwork share one license.
Keep [LICENSE](LICENSE), [source licensing](doc/LICENSE-source.txt),
[artwork notice](doc/LICENSE-logos.txt), [third-party notices](THIRD_PARTY_NOTICES.md),
and contributor credits. The owner-authored Littlefield logo/icon use CC BY-SA 3.0; the staged WebRTC PNG is a traceable official press-kit asset. See doc/ARTWORK_PROVENANCE.md for attribution and limits.

- [Windows build procedure](BUILD_WINDOWS.md)
- [43-package dependency inventory](DEPENDENCIES.md)
- [Upstream and release provenance](PROVENANCE.md)
- [Littlefield changes, including all eight corrections](CHANGES-FTRLV.md)
- [Portable source validation](scripts/validate-public-source.py)

The established executable remains `LittlefieldGridFTRLV.exe`; the installer default
remains `C:\Program Files\LFGFTRLV` with separate Desktop and Start Menu options.
Installers, extracted payloads, dependency caches, logs, private configuration and
the original Git metadata are intentionally absent from this source directory.

This is a Windows source-preparation result, not a claim that macOS/Linux builds or
live-grid runtime tests passed. The old upstream build/support README is preserved
unchanged at [doc/UPSTREAM-README.md](doc/UPSTREAM-README.md).
