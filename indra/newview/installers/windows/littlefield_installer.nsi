; Littlefield Grid FTRLV modified version, 2026-11-22.
; Changes relative to Kokua 7.2.0.61057 are documented in doc/CHANGES-FTRLV.md.
; Existing upstream copyright and license notices are retained.
Unicode true
!include "MUI2.nsh"
!include "x64.nsh"
!include "FileFunc.nsh"
!include "nsDialogs.nsh"

!define APP_NAME "Littlefield Grid FTRLV"
; Retained window class and uninstall key are compatibility identifiers.
!define VIEWER_WINDOW_CLASS "Littlefield Grid Kokua FTRLV Edition"
!define UNINSTALL_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\LFGKokuaFTRLV"
!define MUI_ICON "%%SOURCE%%\installers\windows\install_icon.ico"
Var RegisteredDir
Var SetupMutex
Var PathError
Var WelcomeText
Var CreateDesktop
Var CreateStartMenu
Var DesktopCheckbox
Var StartMenuCheckbox
Var LinkPath
Var LinkTarget
Var LinkResult

Name "${APP_NAME}"
OutFile "%%OUTFILE%%"
InstallDir "$PROGRAMFILES64\LFGFTRLV"
RequestExecutionLevel admin
SetCompressor /SOLID lzma
VIProductVersion "%%VERSION%%"
VIAddVersionKey "ProductName" "${APP_NAME}"
VIAddVersionKey "FileDescription" "Littlefield Grid FTRLV Windows x64 installer"
VIAddVersionKey "FileVersion" "%%VERSION%%"
VIAddVersionKey "LegalCopyright" "See included licenses and credits"

!define MUI_WELCOMEPAGE_TEXT "$WelcomeText"
!insertmacro MUI_PAGE_WELCOME
!define MUI_PAGE_CUSTOMFUNCTION_PRE SkipUpgradeDirectoryPage
!insertmacro MUI_PAGE_DIRECTORY
Page custom ShortcutOptionsPage ShortcutOptionsLeave
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_NOAUTOCLOSE
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

!macro AcquireSetupMutex
    System::Call 'kernel32::CreateMutexW(p 0, i 0, w "Global\LFGKokuaFTRLV.Setup") p .s ?e'
    Pop $0
    Pop $SetupMutex
    ${If} $SetupMutex == 0
    ${OrIf} $0 == 183
        MessageBox MB_ICONSTOP "Another Littlefield setup or uninstall is running." /SD IDOK
        SetErrorLevel 1
        Abort
    ${EndIf}
!macroend

Function .onVerifyInstDir
    Call ValidateInstallDirectory
    ${If} $PathError != ""
        Abort
    ${EndIf}
FunctionEnd

Function ValidateInstallDirectory
    Call ValidatePath
    StrCmp $PathError "" 0 done
    ${If} $RegisteredDir != ""
        StrCmp $INSTDIR $RegisteredDir done
        StrCpy $PathError "Use the registered Littlefield installation directory to upgrade."
        Return
    ${EndIf}
    ; Protect other viewers and unrelated files in custom directories.
    IfFileExists "$INSTDIR\*.*" 0 done
    FindFirst $0 $1 "$INSTDIR\*.*"
  scan:
    StrCmp $1 "" close
    StrCmp $1 "." next
    StrCmp $1 ".." next
    FindClose $0
    StrCpy $PathError "Choose a new or empty installation directory."
    Return
  next:
    FindNext $0 $1
    Goto scan
  close:
    FindClose $0
  done:
FunctionEnd

!macro RequireRuntimeFile filename
    IfFileExists "$INSTDIR\${filename}" +4
    MessageBox MB_ICONSTOP "A required viewer file is missing: ${filename}.$\r$\nRun Setup again before launching the viewer." /SD IDOK
    SetErrorLevel 1
    Abort
!macroend

Section "-Viewer"
    SetRegView 64
    SetShellVarContext all
    Call ValidateInstallDirectory
    ${If} $PathError != ""
        MessageBox MB_ICONSTOP "$PathError" /SD IDOK
        SetErrorLevel 1
        Abort
    ${EndIf}
    Push "$INSTDIR"
    Call CheckTree
    Call RequireClosedViewer
    ; Extract only the package's named files. Leave unrecognized files in place.
    SetOverwrite try
    ClearErrors
    %%INSTALL_FILES%%
    IfErrors install_failed
    SetOutPath "$INSTDIR"
    %%VERIFY_RUNTIME%%
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    IfErrors install_failed
    WriteRegStr HKLM "${UNINSTALL_KEY}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKLM "${UNINSTALL_KEY}" "DisplayVersion" "%%VERSION%%"
    WriteRegStr HKLM "${UNINSTALL_KEY}" "Publisher" "Littlefield Grid"
    WriteRegStr HKLM "${UNINSTALL_KEY}" "InstallLocation" "$INSTDIR"
    WriteRegStr HKLM "${UNINSTALL_KEY}" "DisplayIcon" "$INSTDIR\%%VIEWER_EXE%%,0"
    WriteRegStr HKLM "${UNINSTALL_KEY}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
    WriteRegDWORD HKLM "${UNINSTALL_KEY}" "NoModify" 1
    WriteRegDWORD HKLM "${UNINSTALL_KEY}" "NoRepair" 1
    IfErrors install_failed
    Goto install_done
  install_failed:
    MessageBox MB_ICONSTOP "Setup could not update all Littlefield files or registration. Close the viewer and retry setup." /SD IDOK
    SetErrorLevel 1
    Abort
  install_done:
SectionEnd

Function ShortcutOptionsPage
    !insertmacro MUI_HEADER_TEXT "Choose shortcuts" "Select the shortcuts to create for Littlefield Grid FTRLV."
    nsDialogs::Create 1018
    Pop $0
    ${If} $0 == error
        MessageBox MB_ICONSTOP "Setup could not create the shortcut options page."
        SetErrorLevel 1
        Quit
    ${EndIf}
    ${NSD_CreateCheckbox} 0 12u 100% 16u "Create Desktop Shortcut"
    Pop $DesktopCheckbox
    ${NSD_SetState} $DesktopCheckbox $CreateDesktop
    ${NSD_CreateCheckbox} 0 38u 100% 16u "Create Start Menu Shortcut"
    Pop $StartMenuCheckbox
    ${NSD_SetState} $StartMenuCheckbox $CreateStartMenu
    nsDialogs::Show
FunctionEnd

Function ShortcutOptionsLeave
    ${NSD_GetState} $DesktopCheckbox $CreateDesktop
    ${NSD_GetState} $StartMenuCheckbox $CreateStartMenu
    ; Reject a conflicting selected link before replacing any application files.
    ${If} $CreateDesktop == ${BST_CHECKED}
        StrCpy $LinkPath "$DESKTOP\${APP_NAME}.lnk"
        Call CheckSelectedShortcut
    ${EndIf}
    ${If} $CreateStartMenu == ${BST_CHECKED}
        StrCpy $LinkPath "$SMPROGRAMS\${APP_NAME}.lnk"
        Call CheckSelectedShortcut
    ${EndIf}
FunctionEnd

Function CheckSelectedShortcut
    IfFileExists "$LinkPath" 0 done
    Call ReadShortcutTarget
    ${If} $LinkResult != 0
    ${OrIf} $LinkTarget != "$INSTDIR\%%VIEWER_EXE%%"
        MessageBox MB_ICONSTOP "The selected shortcut path is occupied by an unreadable or different shortcut:$\r$\n$LinkPath$\r$\nChoose a different shortcut selection or resolve that file before continuing. Setup will not overwrite it."
        Abort
    ${EndIf}
  done:
FunctionEnd

; Read existing links without resolving/launching or rewriting them. Both setup
; and uninstall preserve same-name links that point to a different application.
!macro ShortcutReader prefix
Function ${prefix}ReadShortcutTarget
    Push $0
    Push $1
    Push $2
    Push $3
    Push $4
    StrCpy $LinkResult 1
    StrCpy $LinkTarget ""
    System::Call 'ole32::CoInitialize(p 0) i.r0'
    ${If} $0 != 0
    ${AndIf} $0 != 1
        Goto done
    ${EndIf}
    StrCpy $1 0
    StrCpy $2 0
    System::Call 'ole32::CoCreateInstance(g "{00021401-0000-0000-C000-000000000046}", p 0, i 1, g "{000214F9-0000-0000-C000-000000000046}", *p.r1) i.r0'
    ${If} $0 != 0
        Goto uninitialize
    ${EndIf}
    System::Call '$1->0(g "{0000010B-0000-0000-C000-000000000046}", *p.r2) i.r0'
    ${If} $0 == 0
        System::Call '$2->5(w "$LinkPath", i 0) i.r0'
        ${If} $0 == 0
            System::Call '$1->3(w .r3, i ${NSIS_MAX_STRLEN}, p 0, i 0) i.r4'
            ${If} $4 == 0
                StrCpy $LinkTarget $3
                StrCpy $LinkResult 0
            ${EndIf}
        ${EndIf}
        System::Call '$2->2() i.r0'
    ${EndIf}
    System::Call '$1->2() i.r0'
  uninitialize:
    System::Call 'ole32::CoUninitialize()'
  done:
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd
!macroend
!insertmacro ShortcutReader ""
!insertmacro ShortcutReader "un."

!macro InstallShortcut location ownership selection
    ${If} ${selection} == ${BST_CHECKED}
        IfFileExists "${location}\${APP_NAME}.lnk" existing
        ClearErrors
        SetOutPath "$INSTDIR"
        CreateShortcut "${location}\${APP_NAME}.lnk" "$INSTDIR\%%VIEWER_EXE%%" "" "$INSTDIR\%%VIEWER_EXE%%" 0
        IfErrors failed
        Goto selected
      existing:
        StrCpy $LinkPath "${location}\${APP_NAME}.lnk"
        Call ReadShortcutTarget
        ${If} $LinkResult != 0
        ${OrIf} $LinkTarget != "$INSTDIR\%%VIEWER_EXE%%"
            Goto failed
        ${EndIf}
        ; A matching link already works; preserve its arguments and other edits.
      selected:
        ClearErrors
        WriteRegDWORD HKLM "${UNINSTALL_KEY}" "${ownership}" 1
        IfErrors failed
    ${Else}
        !insertmacro RemoveOwnedShortcut "" "${location}" "${ownership}"
        ClearErrors
        ; Persist an explicit opt-out so the next upgrade does not enable it.
        WriteRegDWORD HKLM "${UNINSTALL_KEY}" "${ownership}" 0
        IfErrors failed
    ${EndIf}
    Goto done
  failed:
    MessageBox MB_ICONSTOP "Setup could not create or register ${location}\${APP_NAME}.lnk. An existing shortcut may belong to another application; it was not overwritten."
    SetErrorLevel 1
    Abort
  done:
!macroend

!macro RemoveOwnedShortcut prefix location ownership
    !define link_done "shortcut_done_${ownership}"
    ReadRegDWORD $0 HKLM "${UNINSTALL_KEY}" "${ownership}"
    ${If} $0 == 1
        IfFileExists "${location}\${APP_NAME}.lnk" 0 ${link_done}
        StrCpy $LinkPath "${location}\${APP_NAME}.lnk"
        Call ${prefix}ReadShortcutTarget
        ${If} $LinkResult != 0
            Goto cleanup_failed
        ${EndIf}
        ${If} $LinkTarget == "$INSTDIR\%%VIEWER_EXE%%"
            ClearErrors
            Delete "${location}\${APP_NAME}.lnk"
            IfErrors cleanup_failed
        ${EndIf}
    ${EndIf}
  ${link_done}:
    !undef link_done
!macroend

Section "-Desktop shortcut"
    !insertmacro InstallShortcut "$DESKTOP" "GridDesktopShortcut" "$CreateDesktop"
    Goto section_done
  cleanup_failed:
    MessageBox MB_ICONSTOP "Setup could not safely update the Desktop shortcut."
    SetErrorLevel 1
    Abort
  section_done:
SectionEnd

Section "-Start Menu shortcut"
    !insertmacro InstallShortcut "$SMPROGRAMS" "GridStartMenuShortcut" "$CreateStartMenu"
    Goto section_done
  cleanup_failed:
    MessageBox MB_ICONSTOP "Setup could not safely update the Start Menu shortcut."
    SetErrorLevel 1
    Abort
  section_done:
SectionEnd

Function .onInit
    ${IfNot} ${RunningX64}
        MessageBox MB_ICONSTOP "This viewer requires 64-bit Windows." /SD IDOK
        SetErrorLevel 1
        Abort
    ${EndIf}
    !insertmacro AcquireSetupMutex
    SetRegView 64
    SetShellVarContext all
    StrCpy $CreateDesktop ${BST_CHECKED}
    StrCpy $CreateStartMenu ${BST_CHECKED}
    StrCpy $WelcomeText "Setup will install ${APP_NAME}.$\r$\n$\r$\nClick Next to choose the installation directory and shortcuts."
    ReadRegStr $RegisteredDir HKLM "${UNINSTALL_KEY}" "InstallLocation"
    StrCmp $RegisteredDir "" fresh
    StrCmp $RegisteredDir "$PROGRAMFILES64\LFGKokuaFTRLV" legacy_location_blocked
    StrCpy $INSTDIR $RegisteredDir
    Call ValidatePath
    StrCmp $PathError "" 0 invalid_path
    ReadRegStr $0 HKLM "${UNINSTALL_KEY}" "UninstallString"
    StrCmp $0 '"$INSTDIR\Uninstall.exe"' 0 invalid
    ReadRegStr $0 HKLM "${UNINSTALL_KEY}" "DisplayIcon"
    StrCmp $0 "$INSTDIR\%%VIEWER_EXE%%,0" 0 invalid
    ReadRegStr $0 HKLM "${UNINSTALL_KEY}" "DisplayName"
    StrCmp $0 "${APP_NAME}" 0 invalid
    Goto new_identity
  legacy_location_blocked:
    MessageBox MB_ICONSTOP "An existing Littlefield viewer is registered at:$\r$\n$RegisteredDir$\r$\n$\r$\nThis package uses C:\Program Files\LFGFTRLV for a fresh installation. It will not relocate or replace the retained legacy installation. Cancel setup and resolve the existing installation through a separately approved procedure before installing to the new folder." /SD IDOK
    Goto init_failed
  new_identity:
    IfFileExists "$INSTDIR\%%VIEWER_EXE%%" 0 invalid
    IfFileExists "$INSTDIR\Uninstall.exe" 0 invalid
    StrCpy $WelcomeText "Setup will upgrade ${APP_NAME} in place:$\r$\n$INSTDIR$\r$\n$\r$\nClose the viewer, then click Next to choose your shortcuts. Your viewer profile will be kept."
    ; Missing preferences use the checked fresh-install defaults. Explicit
    ; saved opt-outs remain unchecked independently for each shortcut.
    ClearErrors
    ReadRegDWORD $0 HKLM "${UNINSTALL_KEY}" "GridDesktopShortcut"
    ${IfNot} ${Errors}
    ${AndIf} $0 == 0
        StrCpy $CreateDesktop ${BST_UNCHECKED}
    ${EndIf}
    ClearErrors
    ReadRegDWORD $0 HKLM "${UNINSTALL_KEY}" "GridStartMenuShortcut"
    ${IfNot} ${Errors}
    ${AndIf} $0 == 0
        StrCpy $CreateStartMenu ${BST_UNCHECKED}
    ${EndIf}
    Goto ready
  fresh:
    IfFileExists "$PROGRAMFILES64\LFGFTRLV\%%VIEWER_EXE%%" legacy_unregistered
    IfFileExists "$PROGRAMFILES64\LFGKokuaFTRLV\LittlefieldGridKokuaFTRLVEdition.exe" legacy_unregistered
    IfFileExists "$PROGRAMFILES64\LFGKokuaFTRLV\%%VIEWER_EXE%%" legacy_unregistered ready
  legacy_unregistered:
    MessageBox MB_ICONSTOP "An unregistered Littlefield installation exists in a standard Littlefield viewer folder. Setup cannot safely create a second installation. Restore its uninstall registration or arrange an explicit migration first." /SD IDOK
    Goto init_failed
  invalid:
    MessageBox MB_ICONSTOP "Setup could not verify the registered Littlefield installation." /SD IDOK
    Goto init_failed
  invalid_path:
    MessageBox MB_ICONSTOP "Setup cannot use the registered Littlefield directory:$\r$\n$INSTDIR$\r$\n$\r$\n$PathError" /SD IDOK
  init_failed:
    SetErrorLevel 1
    Abort
  ready:
FunctionEnd

Function SkipUpgradeDirectoryPage
    ${If} $RegisteredDir != ""
        Abort
    ${EndIf}
FunctionEnd

!macro InstallSafetyFunctions prefix
Function ${prefix}ValidatePath
    ; Require a local fixed drive, a canonical non-root path, and ordinary ancestors.
    StrCpy $PathError ""
    ; The NSIS instruction can fail for a fresh path whose parents do not exist.
    StrCpy $0 "$INSTDIR"
    System::Call 'kernel32::GetFullPathNameW(w r0, i ${NSIS_MAX_STRLEN}, w .r4, p 0) i .r2'
    IntCmp $2 0 unsafe unsafe
    IntCmp $2 ${NSIS_MAX_STRLEN} unsafe 0 unsafe
    StrCpy $0 $4
    StrCmp $0 $INSTDIR 0 unsafe
    ${GetRoot} "$INSTDIR" $1
    StrCmp $1 "" unsafe
    StrCmp $0 "$1\" unsafe
    System::Call 'kernel32::GetDriveTypeW(w "$1\") i .r2'
    StrCmp $2 3 0 unsafe
  ancestor:
    System::Call 'kernel32::GetFileAttributesW(w r0) i .r2 ?e'
    Pop $3
    ${If} $2 == -1
        ${If} $3 != 2
        ${AndIf} $3 != 3
            Goto unsafe
        ${EndIf}
    ${Else}
        IntOp $2 $2 & 0x410
        IntCmp $2 0x10 parent unsafe unsafe
    ${EndIf}
  parent:
    ${GetParent} "$0" $0
    StrCmp $0 $1 done
    StrCmp $0 "" unsafe ancestor
  unsafe:
    StrCpy $PathError "Choose an ordinary folder on a local fixed drive, without junctions or symbolic links."
  done:
FunctionEnd

Function ${prefix}CheckTree
    ; Reject redirected entries before any recursive traversal or package writes.
    Exch $0
    Push $1
    Push $2
    Push $3
    FindFirst $1 $2 "$0\*"
  next:
    StrCmp $2 "" close
    StrCmp $2 "." advance
    StrCmp $2 ".." advance
    System::Call 'kernel32::GetFileAttributesW(w "$0\$2") i .r3'
    IntOp $3 $3 & 0x410
    ${If} $3 & 0x400
        FindClose $1
        MessageBox MB_ICONSTOP "The Littlefield folder contains a redirected or unreadable entry." /SD IDOK
        SetErrorLevel 1
        Abort
    ${EndIf}
    ${If} $3 = 0x10
        Push "$0\$2"
        Call ${prefix}CheckTree
    ${EndIf}
  advance:
    FindNext $1 $2
    Goto next
  close:
    FindClose $1
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

Function ${prefix}RequireClosedViewer
    FindWindow $0 "${VIEWER_WINDOW_CLASS}"
    StrCmp $0 0 0 busy
    IfFileExists "$INSTDIR\%%VIEWER_EXE%%" 0 ready
    ; Windows denies write access to an executable mapped by a running process.
    System::Call 'kernel32::CreateFileW(w "$INSTDIR\%%VIEWER_EXE%%", i 0x40000000, i 7, p 0, i 3, i 0x80, p 0) p .r0'
    StrCmp $0 -1 busy
    System::Call 'kernel32::CloseHandle(p r0)'
    Goto ready
  busy:
    MessageBox MB_ICONSTOP "Close ${APP_NAME} and try again." /SD IDOK
    SetErrorLevel 1
    Abort
  ready:
FunctionEnd
!macroend

!insertmacro InstallSafetyFunctions ""
!insertmacro InstallSafetyFunctions "un."

Function un.onInit
    !insertmacro AcquireSetupMutex
    SetRegView 64
    ReadRegStr $RegisteredDir HKLM "${UNINSTALL_KEY}" "InstallLocation"
    StrCmp $RegisteredDir "" invalid
    StrCmp $RegisteredDir $INSTDIR 0 invalid
    Call un.ValidatePath
    StrCmp $PathError "" 0 invalid_path
    Return
  invalid:
    MessageBox MB_ICONSTOP "The registered Littlefield installation does not match this uninstaller." /SD IDOK
    Goto init_failed
  invalid_path:
    MessageBox MB_ICONSTOP "Uninstall cannot use the registered Littlefield directory:$\r$\n$INSTDIR$\r$\n$\r$\n$PathError" /SD IDOK
  init_failed:
    SetErrorLevel 1
    Abort
FunctionEnd

Section "Uninstall"
    SetRegView 64
    SetShellVarContext all
    SetOutPath "$TEMP"
    Push "$INSTDIR"
    Call un.CheckTree
    Call un.RequireClosedViewer
    ClearErrors
    %%DELETE_FILES%%
    !insertmacro RemoveOwnedShortcut "un." "$SMPROGRAMS" "GridStartMenuShortcut"
    !insertmacro RemoveOwnedShortcut "un." "$DESKTOP" "GridDesktopShortcut"
    DeleteRegKey HKLM "${UNINSTALL_KEY}"
    Delete "$INSTDIR\Uninstall.exe"
    RMDir "$INSTDIR"
    Goto done
  cleanup_failed:
    MessageBox MB_ICONSTOP "Some viewer files could not be removed. Close the viewer and run the uninstaller again." /SD IDOK
    SetErrorLevel 1
    Abort
  done:
SectionEnd
