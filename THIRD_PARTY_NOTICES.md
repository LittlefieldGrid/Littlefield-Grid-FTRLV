# Third-party notices and separate publication scopes

**PUBLIC SOURCE: PASS. WINDOWS INSTALLER: FAIL / NOT CLEARED.**
The detailed review, evidence and corrective actions are in
[doc/PUBLICATION_CLEARANCE.md](doc/PUBLICATION_CLEARANCE.md).

Keep LICENSE (LGPL 2.1), doc/LGPL-license.txt, doc/LICENSE-source.txt,
doc/LICENSE-logos.txt, doc/contributions.txt and every per-file contributor notice.
Preserve Linden Lab, Kokua, Marine Kelley/RLV, Firestorm and third-party credits.
Do not apply a blanket MIT/GPLv3 or proprietary license to the whole source tree.
The previously added dated modification notices remain intact.

The designated notices for all 43 packages and the notice collection remain in
doc/dependency-licenses/. Package labels are historical metadata, not grants that
override actual text. Dullahan's exact license matches its MIT notice despite the
stale MPL label; NVAPI's recorded library notice grants MIT rights; the JPEG encoder
has a BSD-style notice despite label NONE. WebRTC has component-specific terms.

The staged LFGrid logo/icon are owner-authored CC BY-SA 3.0. The staged WebRTC PNG
has an official pinned origin and retained website notice. See ARTWORK_PROVENANCE.md.
The unused legacy helper is excluded rather than asserted cleared. These decisions
remain unchanged. The original installer raster has a separate qualified review.

VLC's 365 plugin DLLs and two core DLLs match official 3.0.21 binaries. Actual LGPL
core and GPL/plugin/transitive license duties must be handled separately. Exact
source anchors are in doc/CORRESPONDING_SOURCE.md; the full source/relink set is not
yet delivered. Notices/source URLs alone cannot discharge every binary obligation.

CEF's license is verified against its exact revision. The actual distributed
resources.pak contains a 344-entry Chromium Credits page, now extracted verbatim
under doc/publication-audit/. The earlier missing-Chromium-notices hold is corrected;
LGPL-containing component source/relink fulfillment remains separately incomplete.

Microsoft's release runtime families can be redistributed by eligible licensed
users under applicable terms. The exact older shipped copies' authority and
downstream terms still need a documented route. See doc/MICROSOFT_REDISTRIBUTION.md.

Prepared additional notice material is doc/INSTALLER_ACCOMPANYING_NOTICES.md. It is
not an issued offer, signed agreement, or permission to release the binary now.
Source-only publication need not wait for binary-only holds, provided the installer
and dependency binaries remain excluded and the source/artwork notices are kept.
