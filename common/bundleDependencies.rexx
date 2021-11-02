/*
 * This program is only applicable to MacOS X
 * This program copies required dylibs from their default installation location on a development
 * machine; usually /usr/local/lib into the application's bundle directory.
 * It then adjusts the executable and its dependent dylibs to use those dylibs.
 */
Parse Arg app binary libfile interpreter
location = '@executable_path/../Libraries'
dylibs = ''
srcs =   ''
-- fix all dylibs
Do While Lines( libfile ) > 0
   line = Linein( libfile )
   If Left( line, 1 ) = '#' Then Iterate
   Say 'Processing:' line
   Parse Var line dylib ':' src
   dylibs = dylibs dylib
   srcs = srcs src
   -- fix the executable's specification of where libregina.dylib is
   Address System 'install_name_tool -change' src location'/'dylib app'.app/Contents/MacOS/'binary
End
Say Copies( '-', 50 )
Address System 'otool -L' app'.app/Contents/MacOS/'binary
Do i = 1 To Words( dylibs )
   Call DoADylib app, Word( dylibs, i ), Word( srcs, i ), Strip( dylibs ), Strip( srcs ), location, interpreter
End
Say 'Signing' app'...'
Address System 'codesign --deep --force --verify --verbose --sign "Developer ID Application: Mark Hessling"' app'.app'
Return

DoADylib: Procedure
Parse Arg app, me, src, dests, srcs, location, interpreter
Say Copies( '-', 50 )
Say me
tab = '09'x
dest = app'.app/Contents/Libraries'
If Stream( dest, 'C', 'QUERY EXISTS' ) = '' Then Address System 'mkdir -p' dest
If interpreter = 'regina' Then
   Do
      addons = 'regina'('-va')
      lib = addons'/'src
      If Stream( lib, 'C', 'QUERY EXISTS' ) = '' Then
         Do
            lib = '/usr/local/lib/'src
         End
   End
If Stream( lib, 'C', 'QUERY EXISTS' ) = '' Then
   Do
      Say 'Source library:' src 'not found .Aborting!'
      Exit 1
   End
Address System 'cp' lib dest'/'me
Address System 'otool -L' dest'/'me With Output FIFO ''
Do Queued()
   Parse Pull line
   If Left( line, 1 ) \= tab Then Iterate
   Parse Var line . 2 dylib '(' .
   dylib = Changestr( '/', dylib, ' ' )
   lib = Word( dylib, Words( dylib ) )
   which_of_them = Wordpos( lib, srcs )
   Select
      When lib = src Then
         Do
say 'found me:'src 'full:' dylib
            Address System 'install_name_tool -id' location'/'me dest'/'me
         End
      When which_of_them \= 0 Then
         Do
            it = Word( dests, which_of_them )
say 'found one of them:' it 'full:' dylib
            cmd = 'install_name_tool -change' dylib location'/'it dest'/'me
            Address System cmd
say cmd
         End
      Otherwise Nop
   End
End
Address System 'otool -L' dest'/'me
Return
