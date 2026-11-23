# Microsoft runtime redistribution review

## Verified permission framework

The authoritative VS2022 Distributable Code list permits eligible licensed Visual
Studio users to distribute unmodified release VC runtime files with applications,
subject to the product terms. It excludes debug_nonredist. Individual and
OSI-licensed development are expressly allowed uses of Community 2022. This review
does not purport to certify the owner's unrelated organizational license facts.

- https://learn.microsoft.com/en-us/visualstudio/releases/2022/redistribution
- https://visualstudio.microsoft.com/license-terms/vs2022-ga-community/
- https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist
- https://learn.microsoft.com/en-us/legal/windows-sdk/license

The public v14 download is supported for VS2017–2026. Therefore a newer runtime
version than the VS2022 compiler is not itself an incompatibility/license finding.
Community 2022 terms section 4 require significant application functionality and
protective terms for the Microsoft code, prohibit notices being removed and
prohibit making Microsoft source subject to an excluded license. No Microsoft
runtime source is in this repository. Preserve LGPL rights for the viewer itself.

## Actual installer evidence

MICROSOFT-SIGNATURES.json records valid signatures and version 14.50.35719.0 for all
11 VC DLL locations (8 unique filenames). The installed VS2022 redist is 14.44;
the separately downloaded official current v14 archive contains 14.52.37029.0.
Neither is byte-identical to the shipped DLLs. Cabinet extraction was read-only
inspection of data; neither the downloaded runtime nor this viewer was executed.
The mismatch is explained by versions and does not prove an unauthorized change.

The inherited Copy3rdPartyLibs.cmake can obtain release runtimes from VCTOOLSREDISTDIR
or fall back to Windows System32. A Microsoft signature establishes publisher and
integrity, not all downstream redistribution rights. Document the exact copies'
official redistribution provenance/authority rather than inferring it solely from
their filenames or presence in Windows. A matching official 14.50 archive, or
specific confirmation that these signed copies are permitted redistributables,
would close this evidence gap. Alternatively use verified authorized copies when
creating a separate future installer; viewer source compilation is not required
solely to replace app-local runtime DLLs with compatible permitted versions.

The CEF package also distributes llplugin/d3dcompiler_47.dll version 10.0.22621.1778.
Microsoft documents app-local desktop distribution of this SDK redist family, but
it differs from the currently installed SDK's copy. Document that older file's
authorized acquisition and applicable SDK terms, or replace from verified SDK
redist in a future installer. D3D-COMPILER-AUDIT.json records its hash.

## Distribution requirements still to implement/document

Record the distributor's eligible Visual Studio/SDK license route, the exact
release file source and applicable terms, retain notices, and ensure downstream
Microsoft-code protection terms apply without restricting LGPL-covered components.
This file is a compliance checklist, not a signed agreement or an assertion that
end users have accepted terms. A reviewed external download/distribution agreement
may suffice without changing installer bytes; if changes must be embedded, create
a separate installer. Obtain legal review where those terms' interaction is unclear.

The general claim 'Microsoft runtimes cannot be redistributed' is false. Full
clearance for these exact copies and this distribution arrangement remains pending.
This has no effect on publication of the source-only repository without those DLLs.
