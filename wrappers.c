/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1993-1994  Anders Christensen <anders@pvv.unit.no>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * $Id: wrappers.c,v 1.18 2003/04/26 01:09:02 florian Exp $
 */

/*
 * This file is 'a hell of a file'. It contain _anything_ that is neither
 * POSIX nor ANSI. The meaning is that when these things are needed in
 * the code, they are located in a wrapper in this file. Thus, if you need
 * to hack the code, it's likely that you only need to hack this file.
 *
 * At least, that's the theory ...
 */

#if defined(WIN32)
# ifdef _MSC_VER
#  if _MSC_VER >= 1100
/* Stupid MSC can't compile own headers without warning at least in VC 5.0 */
#   pragma warning(disable: 4115 4201 4214)
#  endif
# endif
# include <windows.h>
# ifdef _MSC_VER
#  if _MSC_VER >= 1100
#   pragma warning(default: 4115 4201 4214)
#  endif
# endif
#endif

#if defined(OS2)
# define INCL_DOSMODULEMGR
# include <os2.h>
# define DONT_TYPEDEF_PFN
#endif

#ifdef _POSIX_SOURCE
# undef _POSIX_SOURCE
#endif

#include "rexx.h"
#include <ctype.h>

#ifdef DYNAMIC

/*
 * Most Unix systems have dlopen(), so set this as the default and
 * unset it for platforms that don't have it - except for HPUX
 */
# if defined(__hpux)
#  define DYNAMIC_HPSHLOAD
# endif

# if defined(HAVE_DLFCN_H) && !defined(DYNAMIC_HPSHLOAD)
#  define DYNAMIC_DLOPEN
# endif

# if defined(DYNAMIC_STATIC)
   typedef void * handle_type;
#  define DYNLIBPRE ""
#  define DYNLIBPST ""
# elif defined(DYNAMIC_DLOPEN)
#  include <dlfcn.h>
   typedef void *handle_type ;
#  ifndef RTLD_LAZY
#   define RTLD_LAZY 1
#  endif
#  if defined(__CYGWIN__)
#   define DYNLIBPRE ""
#   define DYNLIBPST ".dll"
#   define DYNLIBLEN 4
#  else
#   define DYNLIBPRE "lib"
#   if defined(AIX)
#     define DYNLIBPST ".a"
#     define DYNLIBLEN 5
#   else
#     define DYNLIBPST ".so"
#     define DYNLIBLEN 6
#   endif
#  endif
# elif defined(DYNAMIC_AIXLOAD)
#  include <sys/types.h>
#  include <sys/ldr.h>
   typedef int (*handle_type) ;
#  define DYNLIBPRE "lib"
#  define DYNLIBPST ".a"
#  define DYNLIBLEN 5

# elif defined(DYNAMIC_HPSHLOAD)
#  include <dl.h>
   typedef shl_t handle_type ;
#  define DYNLIBPRE "lib"
#  define DYNLIBPST ".sl"
#  define DYNLIBLEN 6

# elif defined(__BEOS__)
#  include <be/kernel/image.h>
#  define DYNAMIC_BEOS  1
   typedef image_id handle_type ;
/*
 * BeOS add-ons are all in a directory specified in $ADDON_PATH
 */
#  define DYNLIBPRE "lib"
#  define DYNLIBPST ".so"
#  define DYNLIBLEN 6

# elif defined(DYNAMIC_OS2)
   typedef HMODULE handle_type ;
/*  typedef PHMODULE handle_type ; */

# elif defined(DYNAMIC_WIN32)
   typedef HINSTANCE handle_type ;
# elif defined(DYNAMIC_VMS)
#  define DYNLIBPRE "LIB"
#  define DYNLIBPST ".EXE"
#  define DYNLIBLEN 7
#endif /* DYNAMIC */

#include <errno.h>
#include <string.h>

#ifdef DYNAMIC_HPSHLOAD_NOT_NEEDED
/*
 * HP/UX doesn't find symbols unless they are 'true data'; entrypoints
 * for functions are not sufficient! Thus, we need to force it to
 * 'release' that these symbols are defined. There's no need to do
 * anything more, once it sets some flag somewhere ... it'll find'em
 * later. Weird!
 *
 * Datatype char only used to save space ... this is never used, so
 * there is no reason to waste space.
 *
 * Removed by MH 0.08h. Causes problems on HPUX 10.20 linker, and may
 * not be needed anymore.
 */

char Food_For_Fuzzy_HP_Linkers[] = {
   (char)exiterror, (char)getonechar,
   (char)nullstringptr,
   (char)atozpos, (char)checkparam, (char)loadrxfunc } ;
#endif

#if defined(DYNAMIC_AIXLOAD)
void *wrapper_dummy_for_aix( void )
{
}
#endif

/* versions of strlwr & strupr to simulate case-insensitivity */
static void rxstrlwr(unsigned char * s, unsigned char *e)
{
   while (*s && s != e)
   {
      *s = (unsigned char) (tolower(*s));
      s++;
   }
}

static void rxstrupr(unsigned char * s, unsigned char *e)
{
   while (*s && s != e)
   {
      *s = (unsigned char) (toupper(*s));
      s++;
   }
}

void *wrapper_load( const tsd_t *TSD, const streng *module )
{
   handle_type handle=(handle_type)NULL ;
#if defined(DYNAMIC_OS2)
   CHAR LoadError[256];
   APIRET rc=0L;
#endif
#if defined(DYNAMIC_WIN32)
   char LoadError[256];
#endif
#if defined(DYNAMIC_HPSHLOAD)
   char buf[1024];
#endif
   char *file_name, *module_name, *udpart, *postfix;

#ifdef DYNLIBLEN
   module_name = MallocTSD( Str_len( module ) + strlen(DYNLIBPRE) +
                           strlen(DYNLIBPST) + 1 ) ;
   strcpy(module_name, DYNLIBPRE );
   udpart = module_name + strlen(DYNLIBPRE);
   memcpy(udpart, module->value, Str_len(module) );
   strcpy(udpart + Str_len(module), DYNLIBPST );
   file_name = module_name;
   postfix = udpart + Str_len(module);
# if defined(DYNAMIC_HPSHLOAD)
   find_shared_library(TSD,module_name,"SHLIB_PATH",buf);
   file_name = buf;
# endif
#else
   file_name = module_name = str_ofTSD(module);
#endif

#if defined(DYNAMIC_STATIC)
   handle = static_dlopen( file_name );
   if (handle == NULL)
   {
      set_err_message(TSD, "static_dlopen() failed loading:", file_name );
      handle = (handle_type)NULL;
   }
#elif defined(DYNAMIC_DLOPEN)
   handle = dlopen( file_name, RTLD_LAZY ) ;

   /* deal with incorrect case in call */
   if (handle == NULL)
   {
      rxstrlwr(udpart,postfix);
      handle = dlopen(module_name, RTLD_LAZY);

      if (handle == NULL)
      {
         rxstrupr(udpart,postfix);
         handle = dlopen(module_name, RTLD_LAZY);
         /*
          * Reset the original module portion of the filename to be
          * searched again so that any error message returned uses the
          * original module name
          */
         if ( handle == NULL )
         {
            memcpy(udpart, module->value, Str_len(module) );
            handle = dlopen(module_name, RTLD_LAZY);
         }
      }
   }

   /* or maybe it's just not there */
   if (handle==NULL)
   {
      char *msg=NULL;
      msg = dlerror();
      if (msg)
         set_err_message(TSD, "dlopen() failed: ", msg ) ;
      else
         set_err_message(TSD, "", "");
   }
#elif defined(DYNAMIC_HPSHLOAD)
   handle = shl_load( file_name, BIND_IMMEDIATE | DYNAMIC_PATH, 0L ) ;
   if (handle == NULL)
   {
      rxstrlwr(udpart,postfix);
      find_shared_library(TSD,module_name,"SHLIB_PATH",buf);
      handle = shl_load( file_name, BIND_IMMEDIATE | DYNAMIC_PATH ,0L ) ;

      if (handle == NULL)
      {
         rxstrupr(udpart,postfix);
         find_shared_library(TSD,module_name,"SHLIB_PATH",buf);
         handle = shl_load( file_name, BIND_IMMEDIATE | DYNAMIC_PATH ,0L ) ;
      }
   }

   if (handle==NULL)
      set_err_message(TSD,  "shl_load() failed: ", strerror(errno)) ;
#elif defined(DYNAMIC_AIXLOAD)
   handle = load( file_name, 1, NULL ) ;
   if (handle)
   {
      int rc=loadbind( 0, (void *)wrapper_dummy_for_aix, (void *)handle ) ;
      if (rc)
         handle = NULL ;
   }
   else
   {
      set_err_message(TSD,  "load() failed: ", strerror( errno )) ;
   }
#elif defined(DYNAMIC_OS2)
   rc = DosLoadModule( LoadError, sizeof(LoadError),
                       file_name, &handle ) ;
   if (rc)
   {
      set_err_message(TSD, "DosLoadModule() unable to load DLL: ", LoadError);
      handle = (handle_type)NULL;
   }
#elif defined(DYNAMIC_WIN32)
   handle = LoadLibrary( file_name ) ;
   if (handle==NULL)
   {
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL ) ;
      set_err_message(TSD, "LoadLibrary() failed: ", LoadError);
   }
#elif defined(DYNAMIC_BEOS)
   handle = load_add_on( file_name );
   if (handle == B_ERROR)
   {
      set_err_message(TSD, "load_add_on() failed loading:", file_name );
      handle = (handle_type)NULL;
   }
#endif

   FreeTSD(module_name);

   return (void *)handle ;
}

PFN wrapper_get_addr( const tsd_t *TSD, const struct library *lptr, const streng *name )
{
   PFN addr;
   handle_type handle=(handle_type)lptr->handle;
   char *funcname ;
#if defined(DYNAMIC_WIN32)
   char LoadError[256];
#endif
#if defined(DYNAMIC_OS2)
   APIRET rc=0L;
#endif
#if defined(DYNAMIC_BEOS)
   status_t rc=0;
#endif
#if defined(DYNAMIC_STATIC)
   int rc=0;
#endif
#if defined(MODULES_NEED_USCORE)
   streng *us_func;
#endif

   funcname = str_of( TSD, name ) ;

#if defined(DYNAMIC_STATIC)
   rc = static_dlsym( handle, funcname,(void **)&addr );
   if ( rc != 0 )
   {
      char buf[150];
      sprintf(buf,"static_dlsym() failed with %d looking for %s", rc, funcname );
      set_err_message(TSD,  buf, "" ) ;
      addr = NULL;
   }
#elif defined(DYNAMIC_DLOPEN)
# if defined(MODULES_NEED_USCORE)
   /*
    * Some platforms need to have an underscore prepended to the function
    * name to be found in a loadable module.
    */
   FreeTSD( funcname );
   us_func = Str_makeTSD( Str_len( name ) + 1 );
   memcpy( us_func->value, "_", 1 );
   us_func->len = 1;
   Str_catTSD( us_func, name );
   funcname = str_of( TSD, us_func );
   Free_stringTSD( us_func );
# endif
   /*
    * Note, the following assignment is not allowed by ANSI, but SVR4.2
    * includes it as an example, so it is probably safe in this context
    */
   addr = (PFN)(dlsym( handle, funcname )) ;

   /* deal with, eg 'SysLoadFuncs' when the function is 'sysloadfuncs' or 'SYSLOADFUNCS' */
   if (addr == NULL)
   {
      rxstrupr(funcname,NULL);
      addr = (PFN)(dlsym( handle, funcname )) ;

      if (addr == NULL)
      {
         rxstrlwr(funcname,NULL);
         addr = (PFN)(dlsym( handle, funcname )) ;

         if (addr==NULL)
            set_err_message( TSD,  "dlsym() failed: ", dlerror() );
      }
   }

#elif defined(DYNAMIC_HPSHLOAD)
   {
      long eaddr ;
      int rc;

      if (rc = shl_findsym( &handle, funcname, TYPE_PROCEDURE, &eaddr ))
      {
         rxstrupr(funcname,NULL);
         if (rc = shl_findsym( &handle, funcname, TYPE_PROCEDURE, &eaddr ))
         {
            rxstrlwr(funcname,NULL);
            if (rc = shl_findsym( &handle, funcname, TYPE_PROCEDURE, &eaddr ))
            {
               addr = NULL ;
               set_err_message( TSD,  "shl_findsym() failed: ", strerror(errno) );
            }
         }
      }

      if (!rc)
         addr = (PFN)eaddr ;
   }

#elif defined(DYNAMIC_AIXLOAD)
   addr = (PFN)handle ;

#elif defined(DYNAMIC_OS2)
   rc = DosQueryProcAddr(handle,0L,funcname,&addr);
   if (rc)
   {
      char buf[150];
      sprintf(buf,"DosQueryProcAddr() failed with %ld looking for %s", (long) rc, funcname );
      set_err_message(TSD, buf, "" ) ;
   }

#elif defined(DYNAMIC_WIN32)
   /*  13/12/1999 JH moved cast, (HMODULE), from second parm to first.  Removed
    * a compiler warning,
    */
   addr = (PFN)GetProcAddress((HMODULE)handle,funcname);

   if (addr == NULL)
   {
      strlwr(funcname);
      addr = (PFN)GetProcAddress((HMODULE)handle,funcname);

      if (addr == NULL)
      {
         strupr(funcname);
         addr = (PFN)GetProcAddress((HMODULE)handle, funcname);
         if (addr == NULL)
         {
            FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LoadError, 256, NULL ) ;
            set_err_message( TSD, "GetProcAddress() failed: ", LoadError );
         }
      }
   }

#elif defined(DYNAMIC_BEOS)
   rc = get_image_symbol(handle,funcname,B_SYMBOL_TYPE_TEXT,(void **)&addr);
   if (rc == B_BAD_IMAGE_ID)
   {
      char buf[150];
      sprintf(buf,"get_image_symbol() failed with %d looking for %s", rc, funcname );
      set_err_message( TSD,  buf, "" );
      addr = NULL;
   }
#endif

   FreeTSD( funcname );

   if (addr)
      return (PFN)addr ;
   else
      return NULL ;
}

#endif /* DYNAMIC */
