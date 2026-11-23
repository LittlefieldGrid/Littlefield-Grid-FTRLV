# Existing-installer WebRTC raster review

Original installer asset: skins/default/textures/3p_icons/webrtc.png, 1280x239,
10,228 bytes, SHA-256
`2ab489e11fcefe1bed4f36e4619b59f78f97b21f20dca97f471e1ce57eb320f7`.

Evidence goes beyond public availability:

- Official press page permits logo images for web/print communications:
  https://webrtc.github.io/webrtc-org/press/ .
- Official pinned site license is retained in artwork-licenses/WebRTC-site-BSD.txt;
  it permits modified derivatives subject to its BSD conditions. It does not grant
  an endorsement or unrestricted trademark use.
- Commons' 2017 horizontal SVG is attributed to the official press kit with a BSD
  notice. Its SVG bytes match the pinned official SVG. Description revision:
  https://commons.wikimedia.org/w/index.php?title=File:WebRTC_Logo.svg&oldid=1133668220 .
- The original PNG depicts that same recognizable design; a visual inspection found
  no apparent added design. That supports, but does not prove, a mechanical-rendering
  derivation. Current Commons 1280px thumbnail bytes/pixels differ; alpha-composited
  mean RGB differences are approximately 1–1.34 on a 0–255 scale. Such similarity
  is not a forensic chain of authorship or permission for arbitrary web images.
- The binary contains a WebRTC-authors BSD notice for software components. It does
  not explicitly map that notice to the logo or establish the raster's origin.

**Conclusion:** BSD's permission to modify means nonidentical PNG bytes do not
automatically require rebuilding an installer. A verified notice plus a defensible
licensed-derivative assessment could permit retaining this raster, but its exact
creation history remains unverified. This review does not give it unconditional
legal clearance. There is no demonstrated infringement finding either.

Before installer publication choose one evidence-backed route: obtain provenance/
permission confirmation; accept the documented derivative assessment after qualified
review and provide the explicit logo notice; or use the already verified staged
official raster in a separate future installer. The last option requires replacing
the packaged asset/repackaging, not necessarily compiling viewer code again.

Prepared accompanying notice: INSTALLER_ACCOMPANYING_NOTICES.md. Do not claim this
review or the staged replacement modified/retroactively cleared the old installer.
