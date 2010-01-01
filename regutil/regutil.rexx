/* test of the regina utilities */
call rxfuncadd 'sysloadfuncs','RexxUtil','sysloadfuncs'

call sysloadfuncs

say 'SysWINVer' SysWinVer()
say 'SysUtilVersion' SysUtilVersion()
call MyPause
say 'SysCls' SysCls()
say 'SysAddRexxMacro' SysAddRexxMacro(x,x)
say 'SysClearRexxMacroSpace' SysClearRexxMacroSpace()
say 'SysDropRexxMacro' SysDropRexxMacro(x)
--say 'SysLoadRexxMacroSpace' SysLoadRexxMacroSpace()
say 'SysQueryRexxMacro' SysQueryRexxMacro(x)
say 'SysReorderRexxMacro' SysReorderRexxMacro(x,2)
say 'SysSaveRexxMacroSpace' SysSaveRexxMacroSpace('tmp.macro')
say 'SysCurState' SysCurState('off')
call MyPause 'cursor should be off'
say 'SysCurState' SysCurState('on')
call MyPause 'cursor should be on'
Say 'Press a key'
say 'SysGetKey' SysGetKey('n')
say 'SysTextScreenRead' SysTextScreenRead(1,1,10)
say 'SysTextScreenSize' SysTextScreenSize()

/*** Demonstrate SysCurPos ***/
call SysCls
say; say 'Demonstrating SysCurPos...'
call SysCurPos '3', '0'
call MyPause 'At 3,0: Press a key'
call SysCurPos '10', '50'
call MyPause 'At 10,20: Press a key'
call SysCurPos '20', '20'
call MyPause 'At 20,20: Press a key'
call SysCurPos '5', '55'
call MyPause 'At 5,65: Press a key'
call SysCurPos '23', '0'
call MyPause

/*
say 'SysCloseEventSem' SysCloseEventSem()
say 'SysCloseMutexSem' SysCloseMutexSem()
say 'SysCreateEventSem' SysCreateEventSem()
say 'SysCreateMutexSem' SysCreateMutexSem()
say 'SysOpenEventSem' SysOpenEventSem()
say 'SysOpenMutexSem' SysOpenMutexSem()
say 'SysPostEventSem' SysPostEventSem()
say 'SysPulseEventSem' SysPulseEventSem()
say 'SysReleaseMutexSem' SysReleaseMutexSem()
say 'SysRequestMutexSem' SysRequestMutexSem()
say 'SysResetEventSem' SysResetEventSem()
say 'SysWaitEventSem' SysWaitEventSem() */
say 'SysBootDrive' SysBootDrive()
say 'SysWINVer' SysWINVer()
say 'SysDriveInfo' SysDriveInfo('C:\')
say 'SysDriveMap' SysDriveMap()
--say 'SysSetPriority' SysSetPriority()
say 'SysSleep' SysSleep(1.1)
say 'SysSwitchSession' SysSwitchSession()
say 'SysSystemDirectory' SysSystemDirectory()
say 'SysVolumeLabel' SysVolumeLabel()
say 'SysFileSystemType' SysFileSystemType('/')
say 'SysGetErrorText' SysGetErrorText(36)

call sysfilesearch 'sysfilesearch','regutil/regfilesys.c','NOODLE.'
do i = 1 to noodle.0
  say noodle.i
  end

call sysfilesearch 'sysfilesearch','regutil/regfilesys.c','NOODLE.', 'c'
do i = 1 to noodle.0
  say noodle.i
  end

call sysfilesearch 'sysfilesearch','regutil/regfilesys.c','NOODLE.', 'cn'
do i = 1 to noodle.0
  say noodle.i
  end

call sysfilesearch 'sysfilesearch','regutil/regfilesys.c','NOODLE.', 'n'
do i = 1 to noodle.0
  say noodle.i
  end

say 'SysDropFuncs' SysDropFuncs()
Return

MyPause:
parse arg prompt
  if prompt='' then
    prompt='Press Enter key when ready . . .'
  call MySay prompt
  Pull .
  say
return

MySay:
parse arg string
  call charout , string
return
