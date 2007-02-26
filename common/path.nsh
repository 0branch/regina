!verbose 3
!include "WinMessages.NSH"
!verbose 4

;====================================================
; AddToPath - Adds the given dir to the search path.
;             Only adds dir if not already in PATH
;             NOTE: Not robust; if c:\bindir;c:\winnt are
;                   PATH, and you specify c:\bin, then
;                   it WILL be found!
;        Input - head of the stack
;        Note - Win9x systems requires reboot
;====================================================
Function AddToPath
  Exch $0
  Push $1
  
  Call IsNT
  Pop $1
  StrCmp $1 1 AddToPath_NT
    ; Not on NT
    ; $0 has the directory to add
    StrCpy $1 $WINDIR 2 ;get the drive letter and colon into $1
    GetFullPathName /SHORT $6 $0
    StrCpy $6 "SET PATH=%PATH%;$6" ; $6 has the string we would have written when installing (except for leading CRLF)
    ; Determine if the dir entry is already there (and written by us)
    FileOpen $5 "$1\autoexec.bat" r ; $5 has autoexec.bat handle
    
    InPath_dosLoop:
      FileRead $5 $3 ; $3 has 1 line from autoexec.bat
      StrCmp $3 "$6$\r$\n" InPath_dosLoopFound ; get out if we found our string
      StrCmp $3 "$6$\n" InPath_dosLoopFound    ; get out if we found our string
      StrCmp $3 "$6" InPath_dosLoopFound       ; get out if we found our string
      StrCmp $3 "" InPath_dosLoopEnd
      Goto InPath_dosLoop

    ; we found our string, so dir is already in PATH, don't do any more
    InPath_dosLoopFound:
      FileClose $5
      Goto AddToPath_done
    
    InPath_dosLoopEnd:
      FileClose $5

    FileOpen $1 "$1\autoexec.bat" a ; $1 now has autoexec.bat handle
    FileSeek $1 0 END
    GetFullPathName /SHORT $0 $0
    FileWrite $1 "$\r$\nSET PATH=%PATH%;$0$\r$\n"
    FileClose $1
    SetRebootFlag true
    Goto AddToPath_done

  AddToPath_NT:
    ReadRegStr $1 HKCU "Environment" "PATH"
    StrCmp $1 "" AddToPath_NTdoIt
    Push $1 ; push the current PATH value onto the stack
    Push $0 ; push the new PATH item onto the stack
    Call StrStr ; Find $0 in $1
    Pop $2 ; pos of the new PATH item in current PATH (-1 if not found)
    IntCmp $2 -1 AddToPath_NTdoIt
    Goto AddToPath_done

    AddToPath_NTdoIt:
      StrCpy $0 "$1;$0"
      WriteRegStr HKCU "Environment" "PATH" $0
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
  AddToPath_done:
    Pop $1
    Pop $0
FunctionEnd

;====================================================
; RemoveFromPath - Remove a given dir from the path
;     Input: head of the stack
;====================================================
Function un.RemoveFromPath
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  
  Call un.IsNT
  Pop $1
  StrCmp $1 1 unRemoveFromPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" r
    GetTempFileName $4
    FileOpen $2 $4 w
    GetFullPathName /SHORT $0 $0
    StrCpy $0 "SET PATH=%PATH%;$0"
    SetRebootFlag true
    Goto unRemoveFromPath_dosLoop
    
    unRemoveFromPath_dosLoop:
      FileRead $1 $3
      StrCmp $3 "$0$\r$\n" unRemoveFromPath_dosLoop
      StrCmp $3 "$0$\n" unRemoveFromPath_dosLoop
      StrCmp $3 "$0" unRemoveFromPath_dosLoop
      StrCmp $3 "" unRemoveFromPath_dosLoopEnd
      FileWrite $2 $3
      Goto unRemoveFromPath_dosLoop
    
    unRemoveFromPath_dosLoopEnd:
      FileClose $2
      FileClose $1
      StrCpy $1 $WINDIR 2
      Delete "$1\autoexec.bat"
      CopyFiles /SILENT $4 "$1\autoexec.bat"
      Delete $4
      Goto unRemoveFromPath_done

  unRemoveFromPath_NT:
    StrCpy $0 ";$0"
    StrLen $2 $0
    ReadRegStr $1 HKCU "Environment" "PATH"
    Push $1
    Push $0
    Call un.StrStr ; Find $0 in $1
    Pop $0 ; pos of our dir
    IntCmp $0 -1 unRemoveFromPath_done
      ; else, it is in path
      StrCpy $3 $1 $0 ; $3 now has the part of the path before our dir
      IntOp $2 $2 + $0 ; $2 now contains the pos after our dir in the path (';')
;      IntOp $2 $2 + 1 ; $2 now containts the pos after our dir and the semicolon.
      StrLen $0 $1
      StrCpy $1 $1 $0 $2
      StrCpy $3 "$3$1"

      WriteRegStr HKCU "Environment" "PATH" $3
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  
  unRemoveFromPath_done:
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

;====================================================
; StrStr - Finds a given string in another given string.
;               Returns -1 if not found and the pos if found.
;          Input: head of the stack - string to find
;                      second in the stack - string to find in
;          Output: head of the stack
;====================================================
Function StrStr
  Push $0
  Exch
  Pop $0 ; $0 now have the string to find
  Push $1
  Exch 2
  Pop $1 ; $1 now have the string to find in
  Exch
  Push $2
  Push $3
  Push $4
  Push $5

  StrCpy $2 -1
  StrLen $3 $0
  StrLen $4 $1
  IntOp $4 $4 - $3

  StrStr_loop:
    IntOp $2 $2 + 1
    IntCmp $2 $4 0 0 StrStrReturn_notFound
    StrCpy $5 $1 $3 $2
    StrCmp $5 $0 StrStr_done StrStr_loop

  StrStrReturn_notFound:
    StrCpy $2 -1

  StrStr_done:
    Pop $5
    Pop $4
    Pop $3
    Exch $2
    Exch 2
    Pop $0
    Pop $1
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Uninstall stuff
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;====================================================
; StrStr - Finds a given string in another given string.
;               Returns -1 if not found and the pos if found.
;          Input: head of the stack - string to find
;                      second in the stack - string to find in
;          Output: head of the stack
;====================================================
Function un.StrStr
  Push $0
  Exch
  Pop $0 ; $0 now have the string to find
  Push $1
  Exch 2
  Pop $1 ; $1 now have the string to find in
  Exch
  Push $2
  Push $3
  Push $4
  Push $5

  StrCpy $2 -1
  StrLen $3 $0
  StrLen $4 $1
  IntOp $4 $4 - $3

  unStrStr_loop:
    IntOp $2 $2 + 1
    IntCmp $2 $4 0 0 unStrStrReturn_notFound
    StrCpy $5 $1 $3 $2
    StrCmp $5 $0 unStrStr_done unStrStr_loop

  unStrStrReturn_notFound:
    StrCpy $2 -1

  unStrStr_done:
    Pop $5
    Pop $4
    Pop $3
    Exch $2
    Exch 2
    Pop $0
    Pop $1
FunctionEnd

;====================================================
; EmptyDirectory - Determines if directory passed is empty
;                  and deletes it and from PATH if not empty
;          Returns: nothing
;          Input: full path name
;====================================================
Function un.EmptyDirectory
  Pop $5
  FindFirst $2 $1 "$5\*.*"
  StrCmp $1 "" empty
  StrCmp $1 "." cont
  StrCmp $1 ".." cont
  Goto done
cont:
  FindNext $2 $1
  ;Dumpstate::debug
  StrCmp $1 "" empty
  StrCmp $1 "." cont
  StrCmp $1 ".." cont
  ; to get here we have a real file, so get out now
  Goto done
empty:
  ; Remove the directory from PATH
  Push $5
  Call un.RemoveFromPath
done:
  FindClose $2
FunctionEnd
