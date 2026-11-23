# LGPL 2.1 compliance plan

This is a distribution checklist, not an issued offer or a blanket license for
every dependency. The controlling license is the individual file/component's
license. Full LGPL 2.1 and GPL 2 texts are retained in publication-audit and the
original project license files remain unchanged.

## Source-only repository

Retain copyright, license/disclaimer notices and license copies. Identify modified
files and dates, and provide covered modifications under the applicable LGPL
terms. The completed snapshot does this. Do not relabel the whole repository MIT,
GPLv3, or a Littlefield proprietary license. Artwork and per-file exceptions retain
their own terms. No dependency DLLs, objects or installer accompany source hosting.

## Binary viewer and libraries

1. Provide prominent identification of LGPL-covered components and their license
   copies. The installer includes Kokua-LICENSE.txt and package notices, but its
   inherited licenses.txt is not an accurate shipped-component inventory: it still
   describes disabled Vivox/other historical components. Preserve valid notices;
   accompany the binary with the actual inventory and this compliance plan.
2. For distributing an LGPL library itself, supply its complete preferred source,
   modifications and scripts controlling compilation/installation under section 4.
   Equivalent source access at the designated binary download place is a route.
3. For linked combinations, section 6 requires applicable modification and reverse-
   engineering rights and one valid mechanism. With static linking, furnish the
   library source and the caller object/source materials needed to rebuild/relink
   with a modified library. Full usable caller source can satisfy that materials
   route; a proprietary compiled import library alone cannot.
4. For compatible DLL replacement, section 6(b) can avoid delivering caller relink
   objects if the mechanism genuinely allows a user-supplied interface-compatible
   modified library. It does not erase section 4 duties for the library DLL being
   distributed. No install-time signature lock was established here; actual
   replacement/relink testing was not performed and is not claimed.
5. Do not impose an EULA prohibition on modifying or reverse engineering the
   LGPL-covered work for debugging library changes. Scope third-party Microsoft
   terms to Microsoft components; review any interaction with statutory/copyleft
   rights before adopting download terms.

## Actual component distinctions

- Viewer/core: LGPL 2.1 principally, with per-file exceptions. Supply the exact
  release version's source and build/install scripts, including all eight changes.
- Hunspell 1.7.2: supplied notice offers MPL 1.1/GPL 2/LGPL 2.1 alternatives. Record
  the selected compliance route; under LGPL, include exact source/patches and a
  relink route for the static library used by the viewer.
- Physics extensions stub: LGPL 2.1 source notice; metadata 'internal' is not a
  license. Verify the publicly downloaded archive's actual stub sources/build
  files are complete and provide them, not an inaccessible private repository URL.
- OpenAL Soft and freealut: packaged texts say GNU Library GPL version 2, not 2.1.
  Apply their actual version/upgrade grants and source/relink conditions. Record
  any later-version election explicitly; do not assume it. Provide both DLL
  sources and build/patch material. DLL replacement alone is insufficient source
  delivery for the DLLs themselves.
- VLC/libVLC: individual LGPL core and GPL modules differ; plugin transitive code
  must be accounted for. GPL plugin presence does not by itself prove the separate
  viewer executable must be relicensed. Any combined-work boundary/compatibility
  issue not resolved by the component notices needs qualified review.
- CEF-contained FFmpeg: embedded credits identify LGPL 2.1+ and optional other
  licensed code; use the actual configuration and exact Chromium dependency pin.
  CEF's BSD license alone is insufficient for all contained code.

## Offers versus direct source access

Prefer completed source bundles, recipes and source access alongside the exact
binary. A pinned third-party URL is useful provenance but does not prove the
distributor has fulfilled every required delivery step.

GPL 2 section 3(b) offers run at least three years and reach any third party, at
no more than physical source-distribution cost. Section 3(c)'s pass-through route
is limited to its stated noncommercial circumstances and requires the received
offer; it is not a general shortcut for downloaded binaries. LGPL 2.1 section 6(c)
offers cover the same user and section 6(a) materials for at least three years.
Keep those different scopes distinct. Library-only section 4 duties must also be
addressed. Do not copy a generic Vivox request paragraph and call it an LFGrid offer.

No offer is issued, no postal/email fulfillment service is promised, and no third
party was contacted by this task. A future offer needs actual complete materials,
a responsible distributor, contact/delivery method, date/version, retention and
fulfillment capacity. Contact email alone cannot establish those conditions.

Authorities: https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html and
https://www.gnu.org/licenses/old-licenses/gpl-2.0.html .
