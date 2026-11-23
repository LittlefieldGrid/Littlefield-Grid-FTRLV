# Littlefield Grid FTRLV changes

This is a modified Kokua-based viewer, not a claim of original authorship of upstream
code. Public snapshot/date of modification notices: 2026-11-22. Original copyright,
licensing, contributor credits and notices are retained.

## Established Littlefield baseline

The development snapshot includes LFGrid login/profile identity, Littlefield branding,
OpenSim asset/appearance/profile/group/teleport compatibility and diagnostics, the
existing FTRLV/RLV implementation, WebRTC-only WolfVoice, NVAPI and theme work.
Hypergrid, PBR, Search, profiles and settings are preserved. It does not add RLVa.
The installer retains LittlefieldGridFTRLV.exe, Program Files\LFGFTRLV, and both
shortcut options. No portal, ROBUST or region-server configuration is included.

## Completed corrections 1-8

1. Grid Status uses viewer-known region connection state, explicitly says grid-wide
   operational status is unavailable, and offers the existing LFGrid welcome page.
   It does not navigate to or poll Second Life grid-status feeds.
2. One LFG Search command remains, still opening search_legacy. Saved legacy_search
   placements migrate and deduplicate without resetting unrelated toolbar settings.
3. Build permits editing an existing modifiable selection on no-rez land, retaining
   permanent-object and existing RLV restrictions. New creation uses the original
   parcel permission gate; simulator restrictions are not bypassed.
4. Shop and Buy OS$ top-interface controls and relevant menu entries were removed;
   balance display and unrelated wallet functions were preserved.
5. Marketplace toolbar/menu entries were removed with null-safe former lookups;
   general browser functionality remains.
6. Report Abuse toolbar/menu entries and exclusive registrations were removed;
   unrelated Help, blocking, bug reporting and safety functions remain.
7. Expanded generated chat headers use Littlefield Grid for the configured LFGrid
   key. Other grids retain the old prefix. Sender names/text, compact chat, logging
   and RLV privacy exceptions are unchanged. Translator scripts were not modified.
8. Standard object purchases on LFGrid reject nonzero quoted prices before funding,
   dialog side effects and ObjectBuy dispatch. Zero-price and other-grid handling
   remain unchanged. The exact Purchase Unavailable notice has only an OK button;
   prices are never converted to zero. Scripted vendors and Pay are out of scope.

## Limits retained from the original correction reports

No verified LFGrid-wide incident/status feed is configured. Chat script contents and
live packets were not independently audited. Purchase policy identifies the selected/
login grid, so Hypergrid contexts still need runtime verification. Build enablement,
permissions, UI behavior, voice and graphics need normal live-grid test coverage.
Unrelated pre-existing twitter/flickr default-toolbar references were not changed.

The raw development validation reports and installed-file inventories are deliberately
not copied into this public package. The portable validator covers the staged source
without relying on private paths, old temporary backups or an installed viewer.

## Publication build fix

A fresh generation failed because the project listed res/ll_icon.ico unconditionally, although LFGrid configures viewerRes.rc with littlefield.ico. Only that stale HEADER_FILE_ONLY project entry was removed in staging; no icon was substituted and no runtime viewer code changed. The original development tree and final installer remain unchanged.

## Staged artwork provenance cleanup

Owner-authored LFGrid logo/icon are unchanged and licensed CC BY-SA 3.0 by owner declaration. Only the public-stage WebRTC raster was replaced with an unchanged official press-kit PNG with a pinned source/hash and BSD-style website notice. Its filename and WebRTC indicator role are unchanged. No original development asset or final installer was modified.

## Public package metadata

Only the top-level autobuild package name, description, prospective canonical repository URL, and incorrect docs/LICENSE-source.txt path were corrected for this public snapshot. The actual license path is doc/LICENSE-source.txt. Upstream copyright metadata, installable dependencies, archive pins and build variants were retained unchanged. The GitHub URL is a proposed publication target, not a claim that a repository was created.
