/*
 * This is an example for using GCI. Enjoy, copy and paste!
 */

/******************************************************************************
 * Try to detect the system to show several things.
 */
parse version "REXX-"ipret"_".
IsRegina = 0
if ipret = "Regina" then do
   IsRegina = 1
   options NOEXT_COMMANDS_AS_FUNCS
   end

parse source system . source

system = translate(system)
if left(system, 3) = "WIN" then do
   library = "GCI.DLL"
   CLib = "MSVCRT"
   system = "WIN"
   end
else do
   library = "libgci.so"
   if uname('S') = "SunOS" then
      CLib = "libc.so.1"
   else
      CLib = "libc.so.6"
   end

/*
 * Check for a builtin RxFuncDefine. Call it with errorneous arguments and
 * expect a simple error if installed and a syntax signal if unknown.
 */
InternalGCI = 0
signal on syntax name NotInstalled
x = "X"
x. = "X"
h = RxFuncDefine(x,x,x,x)
if h \= 0 & DataType(h, "NUM") then
   InternalGCI = 1
NotInstalled:
drop GCI_RC

signal on syntax
if \InternalGCI then do
   if RxFuncadd(RxFuncDefine, "gci", "RxFuncDefine") \= 0 then do
      msg = "Can't load RxFuncDefine of" library
      if IsRegina then
         msg = msg || ":" RxFuncErrMsg()
      say msg
      return 1
      end
   if RxFuncadd(GciFuncDrop, "gci", "GciFuncDrop") \= 0 then do
      msg = "Can't load GciFuncDrop of" library
      if IsRegina then
         msg = msg || ":" RxFuncErrMsg()
      say msg
      return 1
      end
   end

/******************************************************************************
 * After the prelimnaries try to use the GCI.
 */
say "Trying to copy string 'hello' to a string 'world' using the C library"
stem.calltype = cdecl
stem.0 = 2
stem.1.name = "Target"
stem.1.type = indirect string80
stem.2.name = "Source"
stem.2.type = indirect string80
stem.return.type = ""           /* We are not interested in the return value */

call RxFuncDefine strcpy, CLib, "strcpy", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error, code" RESULT || ":" GCI_RC
   return 1
   end

stem.1.name = "Target"
stem.1.value = "world"
stem.2.name = "Source"
stem.2.value = "hello"
call strcpy stem
say stem.1.name"="stem.1.value stem.2.name"="stem.2.value

call funcDrop strcpy
say ""
/******************************************************************************
 * Use the "as function" feature
 */
say "Trying to find the last occurcance of '.' in 'James F. Cooper' using the C"
say "library using the 'as function' feature"
stem.calltype = cdecl as function
stem.0 = 2
stem.1.name = "String"
stem.1.type = indirect string80
stem.2.name = "Character"
stem.2.type = char
stem.return.type = indirect string80

call RxFuncDefine strrchr, CLib, "strrchr", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error, code" RESULT || ":" GCI_RC
   return 1
   end

stem.1.name = "Target"
stem.1.value = "James F. Cooper"
stem.2.name = "Character"
stem.2.value = .
say "The last dot starts at '" || strrchr( stem ) || "'"
say ""
say "Trying to find the last occurcance of '.' in 'James Cooper' using the C lib"
say "We expect a NULL pointer which leads to a dropped value which becomes ''"
say "for a return value."

stem.1.name = "Target"
stem.1.value = "James Cooper"
say "The last dot starts at '" || strrchr( stem ) || "'"

call funcDrop strrchr
say ""
/******************************************************************************
 * Use the "with parameters as function" feature.
 * Note that you must omit "as function" if the return value doesn't exist.
 * We use separate functions for Windows and unix.
 */
if system = "WIN" then signal useWindows

say "Trying to use the math library to compute some natural logarithms"
stem.calltype = cdecl with parameters as function
stem.0 = 1
stem.1.name = "X"
stem.1.type = float128
stem.return.type = float128
call RxFuncDefine logl, "m", "logl", stem
if RESULT \= 0 then do
   stem.1.type = float96
   stem.return.type = float96
   call RxFuncDefine logl, "m", "logl", stem
   end
if RESULT \= 0 then do
   stem.1.type = float64
   stem.return.type = float64
   call RxFuncDefine logl, "m", "log", stem
   end
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error, code" RESULT || ":" GCI_RC
   return 1
   end
say "some logarithms"
do i = 1 to 5
   say "log("i")="logl(i)
   end

call funcDrop logl
say ""
/*******************************/
say "Using a structure and checking the file system's size."
say "You may look into the source."
/*
 * This examples has removed all unnecessary stuff.
 */
stem.calltype = cdecl as function
stem.0 = 2
stem.1.type = indirect string256
stem.2.type = indirect container
stem.2.0 = 10                                /* statvfs64 */
stem.2.1.type    = unsigned                  /* bsize */
stem.2.2.type    = unsigned                  /* frsize */
stem.2.3.type    = unsigned64                /* blocks */
stem.2.4.type    = unsigned64                /* bfree */
stem.2.5.type    = unsigned64                /* bavail */
stem.2.6.type    = unsigned64                /* files */
stem.2.7.type    = unsigned64                /* ffree */
stem.2.8.type    = unsigned64                /* favail */
stem.2.9.type    = unsigned                  /* fsid */
stem.2.10.type   = string256                /* indifferent between unices */
stem.return.type = integer

call RxFuncDefine statvfs, CLib, "statvfs64", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error, code" RESULT || ":" GCI_RC
   return 1
   end

args. = 0
args.1.value = source
args.2.value = 10    /* otherwise the argument becomes NULL */
if statvfs( args ) \= -1 then do
   say "statvfs-info of" source
   say "block size =" args.2.1.value "byte"
   size = trunc(args.2.3.value * args.2.1.value / (1024 * 1024))
   avail = trunc(args.2.5.value * args.2.1.value / (1024 * 1024))
   say "file system size =" size"MB, available =" avail"MB"
   say "file nodes =" args.2.6.value "available =" args.2.8.value
   say "sid =" args.2.9.value
   end
else
   say "Sorry, '"source"' not found."

call funcDrop statvfs
say ""
/*******************************/
say "Finally we use qsort of the C library for sorting some strings using arrays."
/*
 * This examples has removed all unnecessary stuff.
 * We need a sorting routine. Without callbacks we have to use one of a
 * library. "strcmp" is a good example. We have to play with the dynamic
 * link loader.
 * The strategy is:
 * Load the loader functions (dlopen, dlsym, dlclose)
 * Load the compare routine (strcmp) using the loader functions
 * Load the sorting routine (qsort) and do the sort
 * Additional sort operations may have to redefine qsort only.
 */
stem.calltype = cdecl with parameters as function
stem.0 = 2
stem.1.type = indirect string256
stem.2.type = integer
stem.return.type = integer       /* handle, but who cares? */
call RxFuncDefine dlopen, "dl", "dlopen", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining dlopen, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining dlopen, code" RESULT || ":" GCI_RC
   return 1
   end

stem.calltype = cdecl with parameters as function
stem.0 = 2
stem.1.type = integer            /* handle */
stem.2.type = indirect string256
stem.return.type = integer       /* entry point address, but who cares? */
call RxFuncDefine dlsym, "dl", "dlsym", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining dlsym, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining dlsym, code" RESULT || ":" GCI_RC
   return 1
   end

stem.calltype = cdecl with parameters as function
stem.0 = 1
stem.1.type = integer            /* handle */
stem.return.type = integer
call RxFuncDefine dlclose, "dl", "dlclose", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining dlclose, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining dlclose, code" RESULT || ":" GCI_RC
   return 1
   end

CLibHandle = dlopen( CLib, 1 /* RTLD_LAZY */ )
if CLibHandle = 0 then do
   say "dlopen() can't load" CLib
   return 1
   end

strcmp = dlsym( CLibHandle, "strcmp" )
if strcmp = 0 then do
   say "dlsym() can't relocate strcmp()"
   return 1
   end

stem.calltype = cdecl
stem.0 = 4
stem.1.type = indirect array
stem.1.0 = 3
stem.1.1.type = string95
stem.2.type = integer
stem.3.type = integer
stem.4.type = integer
stem.return.type = ""
call RxFuncDefine qsort10, CLib, "qsort", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining qsort, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining qsort, code" RESULT || ":" GCI_RC
   return 1
   end

args.0 = 4
args.1.value = 3
args.1.1.value = "Ann"
args.1.2.value = "Charles"
args.1.3.value = "Betty"
args.2.value = 3
args.3.value = 96
args.4.value = strcmp
say "Sorting (" args.1.1.value args.1.2.value args.1.3.value ") ..."
call qsort10 args
say "Sorted values are (" args.1.1.value args.1.2.value args.1.3.value ")"

call dlclose CLibHandle
call funcDrop qsort
call funcDrop dlclose
call funcDrop dlsym
call funcDrop dlopen
return 0


/***************************************************/
useWindows:
stem.calltype = stdcall with parameters as function
stem.0 = 4
stem.1.name = "HWND"
stem.1.type = unsigned
stem.2.name = "Text"
stem.2.type = indirect string1024
stem.3.name = "Caption"
stem.3.type = indirect string1024
stem.4.name = "Type"
stem.4.type = unsigned
stem.return.type = integer

call RxFuncDefine messagebox, "user32", "MessageBoxA", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error, code" RESULT || ":" GCI_RC
   return 1
   end
MB_YESNO_INFO = x2d(44)
if messagebox( 0, "Do you love this rocking GCI?", "GCI", MB_YESNO_INFO ) = 6 then
   say "Yes, you're right, GCI is cool."
else
   say "No, you're kidding! GCI is cool."

call funcDrop messagebox
say ""
/*******************************/
say "Finally, we operate on containers and check this file's date."
say "You may look into the source."
/*
 * This examples has removed all unnecessary stuff.
 */
stem.calltype = stdcall as function
stem.0 = 2
stem.1.type = indirect string256
stem.2.type = indirect container
stem.2.0 = 8                                 /* WIN32_FIND_DATA */
stem.2.1.type = unsigned                     /* FileAttributes */
stem.2.2.type = unsigned64                   /* Creation */
stem.2.3.type = unsigned64                   /* Access */
stem.2.4.type = unsigned64                   /* Write */
stem.2.5.type = unsigned64                   /* Size */
stem.2.6.type = unsigned64                   /* Reserved */
stem.2.7.type = string259                    /* FileName */
stem.2.8.type = string13                     /* AlternateFileName */
stem.return.type = integer

stem2.calltype = stdcall with parameters
stem2.0 = 1
stem2.1.type = integer
stem2.return.type = ""

call RxFuncDefine findfirstfile, "kernel32", "FindFirstFileA", stem
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining FindFirstFile, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining FindFirstFile, code" RESULT || ":" GCI_RC
   return 1
   end
call RxFuncDefine findclose, "kernel32", "FindClose", stem2
if RESULT \= 0 then do
   if IsRegina & InternalGCI then
      say "Error defining FindClose, code" RESULT || ":" RxFuncErrMsg()
   else
      say "Error defining FindClose, code" RESULT || ":" GCI_RC
   return 1
   end

args. = 0
args.1.value = source
args.2.value = 8    /* otherwise the argument becomes NULL */
handle = findfirstfile( args )
if handle \= -1 then do
   say "argument's name="source
   say "filename="args.2.7.value
   say "8.3-name="args.2.8.value
   numeric digits 40
   filetime = args.2.4.value
   d = /*second*/ 1000*1000*10   *   /*seconds per day*/ 60*60*24
   daypart = trunc(filetime / d)
   date = date( 'N', daypart + date('B', 16010101, 'S'), 'B')
   ns = filetime - daypart * d
   secs = ns % (10*1000*1000)
   fract = ns // (10*1000*1000)
   time = time('N', secs, 'S') || "." || right(fract, 7, '0')
   say "ns from 1.1.1601="filetime "= GMT," date || "," time
   numeric digits 9
   call findclose handle
   end
else
   say "Sorry, '"source"' not found."

call funcDrop findfirstfile
call funcDrop findclose
say ""
return 0

/*****************************************************************************/
syntax:
   /*
    * Not all interpreters are ANSI compatible.
    */
   code = .MN
   if code = '.MN' then
      code = RC
   say "Error" code "in line" SIGL || ":" condition('D')
   say "GCI_RC=" || GCI_RC
   exit 0

/*****************************************************************************/
funcDrop:
   /*
    * Drops one defined function depending on whether is is defined in the
    * lightweight library or in the interpreter's kernel.
    */
   if InternalGCI then
      call RxFuncDrop arg(1)
   else
      call GciFuncDrop arg(1)
   return

