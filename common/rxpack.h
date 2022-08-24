/*
 * Copyright (C) 1998-2001  Mark Hessling <M.Hessling@qut.edu.au>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _RXPACK_H
#define _RXPACK_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
# include "defines.h"
#endif

#include <stdio.h>

#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif

#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

#ifdef HAVE_MATH_H
# include <math.h>
#endif

#ifdef HAVE_STDARG_H
# include <stdarg.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_GETOPT_H
# include <getopt.h>
#endif

#ifdef HAVE_IO_H
# include <io.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
/*
 * The includes for stdint.h and inttypes.h are deliberately included in quotes
 * so that the Windows replacement files can be found in the "win" directory
 */
#ifdef HAVE_STDINT_H
# include "stdint.h"
#endif
#ifdef HAVE_INTTYPES_H
# include "inttypes.h"
#endif

/*
 * All things that this application may require out of os2.h must be
 * specified here in one place.  This is because EMX includes all Rexx
 * related stuff in os2.h, not in a serperate header file.  This makes
 * it difficult to use another Rexx interpreter with EMX :-(
 */
#if defined(OS2) || defined(__OS2__)
# if !defined(EMXVIDEO)
#  define INCL_VIO
#  define INCL_KBD
# endif
# if defined(USE_OS2REXX)
#  define INCL_RXSHV      /* Shared variable support */
#  define INCL_RXFUNC     /* External functions support */
#  define INCL_RXSYSEXIT  /* System exit routines */
#  define INCL_RXSUBCOM   /* Subcommand routines */
#  include <os2.h>
# else
#  include <os2.h>
#  define INCL_RXSHV      /* Shared variable support */
#  define INCL_RXFUNC     /* External functions support */
#  define INCL_RXSYSEXIT  /* System exit routines */
#  define INCL_RXSUBCOM   /* Subcommand routines */
# endif
#else
# define INCL_RXSHV      /* Shared variable support */
# define INCL_RXFUNC     /* External functions support */
# define INCL_RXSYSEXIT  /* System exit routines */
# define INCL_RXSUBCOM   /* Subcommand routines */
#endif

/*
 * The following header file is supplied by the application package.
 * It specifies any application-specific header files and #defines
 * the following:
 * RXPACKAGE_MAGIC_NUMBER  12345     - any number
 * RXPACKAGE_DEBUG_VAR     "ENV_VAR" - an env variable enclosed in quotes
 */
#include "apphead.h"

#include "rxdef.h"

#include "rxmt.h"

#define RETBUFLEN 250

#if defined(DYNAMIC)
# define DYNAMIC_LIBRARY 1
#endif

#ifndef TRUE
# define TRUE   1
# define FALSE  0
#endif

/* values returned from GetRexxVariable...() indicating status of the retrieved Rexx variable */
#define RXPACK_INVALID  (-1)
#define RXPACK_OK       0
#define RXPACK_MISSING  1

#define RXPACK_NOTAIL (-1)
/*
 * Run time modes
 */
#define MODE_DEBUG        1
#define MODE_VERBOSE      2
#define MODE_INTERNAL     4
#define MODE_HALTONERROR  8

#define REXX_FAIL      1

#define RXSTRCAT(dst,dstlen,src,srclen)   \
{                                         \
   memcpy((dst)+(dstlen),(src),(srclen)); \
   (dstlen)+=(srclen);                    \
   *((dst)+(dstlen))='\0';                \
}
#define RXSTRCPY(dst,dstlen,src,srclen)   \
{                                         \
   memcpy((dst),(src),(srclen));          \
   (dstlen)=(srclen);                     \
   *((dst)+(dstlen))='\0';                \
}

#if !defined(max)
# define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif

#if !defined(min)
# define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif

#if !defined(MAX_PATH)
# if defined(NAME_MAX)
#  define MAX_PATH NAME_MAX
# elif defined(MAXNAMLEN)
#  define MAX_PATH MAXNAMLEN
# else
#  define MAX_PATH 255
# endif
#endif

typedef int8_t RX_INT8;
typedef uint8_t RX_UINT8;
typedef short RX_SHORT;
typedef int RX_INT;
typedef unsigned int RX_UINT;
typedef uint16_t RX_UINT16;
typedef long RX_LONG;
typedef unsigned long RX_ULONG;
typedef unsigned long RX_BOOL;
typedef unsigned int RX_INTBOOL;

/*
 * Typedef a common "long long"
 */
#if defined(_MSC_VER) || defined(__LCC__)
   /* MS VC++ or LCC on Win32 */
   typedef signed __int64 rx_long_long;
#  define RX_LL_FORMAT "%I64d"
#elif defined(__QNX__)
   typedef long rx_long_long;
#  define RX_LL_FORMAT "%ld"
#elif (defined(__WATCOMC__) && !defined(__QNX__)) || (defined(__GNUC__) && defined(WIN32))
   /* Watcom C++ on WIn32 or OS/2 or Cygwin on Win32 */
   typedef long long rx_long_long;
#  define RX_LL_FORMAT "%I64d"
#elif defined(HAVE_LONG_LONG)
   /* Any compiler that supports "long long" */
   typedef long long rx_long_long;
#  define RX_LL_FORMAT "%lld"
#else
   /* No "long long" support; make it "long" */
   typedef long rx_long_long;
#  define RX_LL_FORMAT "%ld"
#endif

#if defined(HAVE__STATI64)
# define rx_stat_buf  _stati64
# if defined(WIN32) || defined(WIN64)
#  define rx_stat      rx_w32_stat
# else
#  define rx_stat      stati64
# endif
# define rx_fstat     _fstati64
# define rx_fseek     _fseeki64
# define rx_ftell     _ftelli64
#else
# define rx_stat_buf  stat
# if defined(WIN32) || defined(WIN64)
#  define rx_stat     rx_w32_stat
#else
#  define rx_stat     stat
#endif
# define rx_fstat     fstat
# define rx_fseek     fseek
# define rx_ftell     ftell
#endif

#ifdef USE_REXX6000
typedef USHORT RexxFunctionHandler(PSZ, ULONG, PRXSTRING, PSZ, PRXSTRING) ;
#endif
/*
 * Standard REXX API function - idea borrowed from Patrick McPhee's Regutil
 */
#define rxfunc(x) RFH_RETURN x( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE stck, RFH_ARG4_TYPE retstr )
/*#define rxfunc(x) APIRET APIENTRY x( RFH_ARG0_TYPE name, RFH_ARG1_TYPE argc, RFH_ARG2_TYPE argv, RFH_ARG3_TYPE stck, RFH_ARG4_TYPE retstr )*/

/*-----------------------------------------------------------------------------
 * Definition of an external function
 *----------------------------------------------------------------------------*/
typedef struct {
   RRFD_ARG0_TYPE      ExternalName;
   RRFE_ARG1_TYPE      EntryPoint;
   RRFD_ARG2_TYPE      InternalName;
   int                 DllLoad;
} RexxFunction;


/*
 * The following structure contains details of a package's constants
 */
typedef struct
{
   char *name;          /* base name of constant */
   int type;            /* 0 (numeric), 1 (text), 2 (float), 3 (char) */
   long numeric_value;  /* numeric value of constant */
   char *text_value;    /* text value of constant */
   double double_value; /* double value of constant */
   char char_value;     /* char value of constant */
} RxPackageConstantDef;

typedef int PackageInitialiser( RxPackageGlobalDataDef * );
typedef int PackageTerminator( RxPackageGlobalDataDef * );

#ifdef HAVE_PROTO
# define Args(a) a
#else
# define Args(a) ()
#endif

/*
 * The following #defines are intended to prevent a problem with duplicate function names
 * when code is statically linked with the non-thread-safe version of this code. ie Rexx/SQL
 * static library linked with Rexx/ISAM static library
 */
#define FunctionPrologue        RxpFunctionPrologue
#define FunctionTrace           RxpFunctionTrace
#define FunctionEpilogue        RxpFunctionEpilogue
#define InternalTrace           RxpInternalTrace
#define RxDisplayError          RxpRxDisplayError
#define InitRxPackage           RxpInitRxPackage
#define TermRxPackage           RxpTermRxPackage
#define RegisterRxFunctions     RxpRegisterRxFunctions
#define RegisterRxSubcom        RxpRegisterRxSubcom
#define RegisterRxInit          RxpRegisterRxInit
#define QueryRxFunction         RxpQueryRxFunction
#define DeregisterRxFunctions   RxpDeregisterRxFunctions
#define SetPackageConstants     RxpSetPackageConstants
#define make_upper              Rxpmake_upper
#define make_upper_with_length  Rxpmake_upper_with_length
#define make_lower              Rxpmake_lower
#define AllocString             RxpAllocString
#define MkAsciz                 RxpMkAsciz
#define SetRexxVariable         RxpSetRexxVariable
#define GetRexxVariable         RxpGetRexxVariable
#define GetRexxVariableInteger  RxpGetRexxVariableInteger
#define GetRexxVariableULong    RxpGetRexxVariableULong
#define DropRexxVariable        RxpDropRexxVariable
#define RxSetTraceFile          RxpRxSetTraceFile
#define RxGetTraceFile          RxpRxGetTraceFile
#define RxSetConstantPrefix     RxpRxSetConstantPrefix
#define RxGetConstantPrefix     RxpRxGetConstantPrefix
#define RxSetRunFlags           RxpRxSetRunFlags
#define RxGetRunFlags           RxpRxGetRunFlags
#define RxReturn                RxpRxReturn
#define RxReturnHex             RxpRxReturnHex
#define RxReturnString          RxpRxReturnString
#define RxReturnStringAndFree   RxpRxReturnStringAndFree
#define RxReturnDataAndFree     RxpRxReturnDataAndFree
/* the following 3 should be deprecated */
#define RxReturnNumber          RxpRxReturnNumber
#define RxReturnLongLong        RxpRxReturnLongLong
#define RxReturnUnsignedNumber  RxpRxReturnUnsignedNumber
#define RxReturnINT8            RxpRxReturnINT8
#define RxReturnUINT8           RxpRxReturnUINT8
#define RxReturnINT16           RxpRxReturnINT16
#define RxReturnUINT16          RxpRxReturnUINT16
#define RxReturnINT32           RxpRxReturnINT32
#define RxReturnUINT32          RxpRxReturnUINT32
#define RxReturnINT64           RxpRxReturnINT64
#define RxReturnUINT64          RxpRxReturnUINT64
#define RxReturnDouble          RxpRxReturnDouble
#define RxReturnFloat           RxpRxReturnFloat
#define RxReturnPointer         RxpRxReturnPointer
#define memcmpi                 Rxpmemcmpi
#define rxstrcmpi               Rxprxstrcmpi
#define my_checkparam           Rxpmy_checkparam
#define RxStrToBool             RxpRxStrToBool
#define RxStrToIntBool          RxpRxStrToIntBool
/* the following 5 should be deprecated */
#define RxStrToShort            RxpRxStrToShort
#define RxStrToInt              RxpRxStrToInt
#define RxStrToUInt             RxpRxStrToUInt
#define RxStrToLong             RxpRxStrToLong
#define RxStrToULong            RxpRxStrToULong
#define RxStrToLongLong         RxpRxStrToLongLong
#define RxStrToINT8             RxpRxStrToINT8
#define RxStrToUINT8            RxpRxStrToUINT8
#define RxStrToINT16            RxpRxStrToINT16
#define RxStrToUINT16           RxpRxStrToUINT16
#define RxStrToINT32            RxpRxStrToINT32
#define RxStrToUINT32           RxpRxStrToUINT32
#define RxStrToINT64            RxpRxStrToINT64
#define RxStrToUINT64           RxpRxStrToUINT64
#define RxStrToDouble           RxpRxStrToDouble
#define RxStrToFloat            RxpRxStrToFloat
#define RxStrToPointer          RxpRxStrToPointer
#define RxStemToCharArray       RxpRxStemToCharArray
#define RxFreeCharArray         RxpRxFreeCharArray
#define RxStemToIntArray        RxpRxStemToIntArray
#define RxFreeIntArray          RxpRxFreeIntArray
#define RxStemToUIntArray       RxpRxStemToUIntArray
#define RxFreeUIntArray         RxpRxFreeUIntArray
#define RxStemToUINT16Array     RxpRxStemToUINT16Array
#define RxFreeUINT16Array       RxpRxFreeUINT16Array
#define RxStemToLongArray       RxpRxStemToLongArray
#define RxFreeLongArray         RxpRxFreeLongArray
#define RxStemToULongArray      RxpRxStemToULongArray
#define RxFreeULongArray        RxpRxFreeULongArray
#define RxNumberToVariable      RxpRxNumberToVariable
#define RxGetRexxInterpreterVersion  RxpRxGetRexxInterpreterVersion

RxPackageGlobalDataDef *FunctionPrologue Args(( RxPackageGlobalDataDef *, PackageInitialiser *, char *, char *, ULONG, RFH_ARG2_TYPE ));
long FunctionEpilogue Args(( RxPackageGlobalDataDef *, char *, long ));
#ifdef HAVE_PROTO
void FunctionTrace( RxPackageGlobalDataDef *, char *, ... );
void InternalTrace( RxPackageGlobalDataDef *, char *, ... );
void RxDisplayError( RxPackageGlobalDataDef *, RFH_ARG0_TYPE, ... );
#else
void RxDisplayError( );
void FunctionTrace();
void InternalTrace();
#endif

RxPackageGlobalDataDef *InitRxPackage Args(( RxPackageGlobalDataDef *, PackageInitialiser *, char *, int * ));
int TermRxPackage Args(( RxPackageGlobalDataDef *, PackageTerminator *, RexxFunction *, char *, int ));
int RegisterRxFunctions Args(( RxPackageGlobalDataDef *, RexxFunction *, char * ));
int RegisterRxSubcom Args(( RxPackageGlobalDataDef *, RexxSubcomHandler ));
int RegisterRxInit Args(( RxPackageGlobalDataDef *, RexxExitHandler, char *));
int QueryRxFunction Args(( RxPackageGlobalDataDef *, char * ));
int DeregisterRxFunctions Args(( RxPackageGlobalDataDef *, RexxFunction *, int ));
int SetPackageConstants Args(( RxPackageGlobalDataDef *, RxPackageConstantDef *, int ));
char *make_upper Args(( RxPackageGlobalDataDef *,char * ));
char *make_upper_with_length Args(( RxPackageGlobalDataDef *,char *, int ));
char *make_lower Args(( RxPackageGlobalDataDef *,char * ));
char *AllocString Args(( RxPackageGlobalDataDef *, char *, int ));
char *MkAsciz Args(( RxPackageGlobalDataDef *,char *, int, char *, int ));
int SetRexxVariable Args(( RxPackageGlobalDataDef *,char *, ULONG, char *, ULONG ));
RXSTRING *GetRexxVariable Args(( RxPackageGlobalDataDef *, char *, RXSTRING *, int ));
int *GetRexxVariableInteger Args(( RxPackageGlobalDataDef *, char *, int *, int ));
int GetRexxVariableULong Args(( RxPackageGlobalDataDef *, char *, unsigned long *, int ));
int DropRexxVariable Args(( RxPackageGlobalDataDef *,char *, int ));
int RxSetTraceFile Args(( RxPackageGlobalDataDef *, char * ));
char *RxGetTraceFile Args(( RxPackageGlobalDataDef * ));
int RxSetConstantPrefix Args(( RxPackageGlobalDataDef *, char * ));
char *RxGetConstantPrefix Args(( RxPackageGlobalDataDef * ));
void RxSetRunFlags Args(( RxPackageGlobalDataDef *, int ));
int RxGetRunFlags Args(( RxPackageGlobalDataDef * ));
int RxReturn Args(( RxPackageGlobalDataDef *, RXSTRING * ));
int RxReturnHex Args(( RxPackageGlobalDataDef *, RXSTRING * ));
int RxReturnString Args(( RxPackageGlobalDataDef *, RXSTRING *, char * ));
int RxReturnStringAndFree Args(( RxPackageGlobalDataDef *, RXSTRING *, char *, int ));
int RxReturnDataAndFree Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, void *str, long len, int freeit ));
int RxReturnNumber Args(( RxPackageGlobalDataDef *, RXSTRING *, long ));
int RxReturnLongLong Args(( RxPackageGlobalDataDef *, RXSTRING *, rx_long_long num ));
int RxReturnUnsignedNumber Args(( RxPackageGlobalDataDef *, RXSTRING *, ULONG ));
int RxReturnINT8 Args(( RxPackageGlobalDataDef *, RXSTRING *, int8_t));
int RxReturnUINT8 Args(( RxPackageGlobalDataDef *, RXSTRING *, uint8_t));
int RxReturnINT16 Args(( RxPackageGlobalDataDef *, RXSTRING *, int16_t));
int RxReturnUINT16 Args(( RxPackageGlobalDataDef *, RXSTRING *, uint16_t));
int RxReturnINT32 Args(( RxPackageGlobalDataDef *, RXSTRING *, int32_t));
int RxReturnUINT32 Args(( RxPackageGlobalDataDef *, RXSTRING *, uint32_t));
int RxReturnINT64 Args(( RxPackageGlobalDataDef *, RXSTRING *, int64_t));
int RxReturnUINT64 Args(( RxPackageGlobalDataDef *, RXSTRING *, uint64_t));
int RxReturnDouble Args(( RxPackageGlobalDataDef *, RXSTRING *, double ));
int RxReturnFloat Args(( RxPackageGlobalDataDef *, RXSTRING *, float ));
int RxReturnPointer Args(( RxPackageGlobalDataDef *, RXSTRING *, void * ));
int memcmpi Args(( RxPackageGlobalDataDef *,char *, char *, int ));
int rxstrcmpi Args(( RxPackageGlobalDataDef *, char *, char * ));
int my_checkparam Args(( RxPackageGlobalDataDef *, RFH_ARG0_TYPE, int, int, int ));
int RxStrToBool Args(( RxPackageGlobalDataDef *,RXSTRING *, ULONG * ));
int RxStrToIntBool Args(( RxPackageGlobalDataDef *, RXSTRING *, unsigned int * ));
int RxStrToInt Args(( RxPackageGlobalDataDef *, RXSTRING *, int * ));
int RxStrToShort Args(( RxPackageGlobalDataDef *, RXSTRING *, short * ));
int RxStrToUInt Args(( RxPackageGlobalDataDef *, RXSTRING *, unsigned int * ));
int RxStrToLong Args(( RxPackageGlobalDataDef *, RXSTRING *, long * ));
int RxStrToULong Args(( RxPackageGlobalDataDef *, RXSTRING *, unsigned long * ));
int RxStrToLongLong Args(( RxPackageGlobalDataDef *, RXSTRING *, rx_long_long * ));
int RxStrToINT8 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int8_t *result ));
int RxStrToUINT8 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint8_t *result ));
int RxStrToINT16 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int16_t *result ));
int RxStrToUINT16 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint16_t *result ));
int RxStrToINT32 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int32_t *result ));
int RxStrToUINT32 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint32_t *result ));
int RxStrToINT64 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int64_t *result ));
int RxStrToUINT64 Args(( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint64_t *result ));
int RxStrToDouble Args(( RxPackageGlobalDataDef *, RXSTRING *, double * ));
int RxStrToFloat Args(( RxPackageGlobalDataDef *, RXSTRING *, float * ));
int RxStrToPointer Args(( RxPackageGlobalDataDef *, RXSTRING *, void ** ));
int RxStemToCharArray Args(( RxPackageGlobalDataDef *, RXSTRING *, char *** ));
void RxFreeCharArray Args(( RxPackageGlobalDataDef *,char **, int ));
int RxStemToIntArray Args(( RxPackageGlobalDataDef *, RXSTRING *, int ** ));
void RxFreeIntArray Args(( RxPackageGlobalDataDef *,int * ));
int RxStemToUIntArray Args(( RxPackageGlobalDataDef *, RXSTRING *, unsigned int ** ));
void RxFreeUIntArray Args(( RxPackageGlobalDataDef *,unsigned int * ));
int RxStemToUINT16Array Args(( RxPackageGlobalDataDef *, RXSTRING *, uint16_t ** ));
void RxFreeUINT16Array Args(( RxPackageGlobalDataDef *,uint16_t * ));
int RxStemToLongArray Args(( RxPackageGlobalDataDef *, RXSTRING *, long ** ));
void RxFreeLongArray Args(( RxPackageGlobalDataDef *,long * ));
int RxStemToULongArray Args(( RxPackageGlobalDataDef *, RXSTRING *, unsigned long ** ));
void RxFreeULongArray Args(( RxPackageGlobalDataDef *,unsigned long * ));
int RxNumberToVariable Args(( RxPackageGlobalDataDef *, RXSTRING *, ULONG ));
int RxBinaryStringToUINT8 Args(( RxPackageGlobalDataDef *, RXSTRING *, uint8_t * ));
char *RxGetRexxInterpreterVersion Args(( char *buf ));
#if defined(WIN32) || defined(WIN64)
# if defined(HAVE__STATI64)
int rx_w32_stat( const char *path, struct _stati64 *buffer );
# else
int rx_w32_stat( const char *path, struct _stat *buffer );
# endif
#endif

#ifdef DEBUG
# define DEBUGDUMP(x) {x;}
#else
# define DEBUGDUMP(x) {}
#endif
/*
 * Directory and PATH separators
 */
#if defined(MSDOS) || ( defined(__WATCOMC__) && !defined(__QNX__) ) || defined(_MSC_VER) || defined(DOS) || defined(OS2) ||defined(__OS2__) || defined(__WINS__) || defined(__EPOC32__) || defined(__LCC__)
# define FILE_SEPARATORS     "\\/:"
# define FILE_SEPARATOR      '\\'
# define FILE_SEPARATOR_STR  "\\"
# define PATH_SEPARATOR      ';'
# define PATH_SEPARATOR_STR  ";"
#elif defined(VMS)
# define FILE_SEPARATORS    "]"
# define FILE_SEPARATOR     ']'
# define FILE_SEPARATOR_STR "]"
# define PATH_SEPARATOR     '?'
# define PATH_SEPARATOR_STR "?"
#elif defined(MAC)
# define FILE_SEPARATOR     "]"
# define FILE_SEPARATOR     ']'
# define FILE_SEPARATOR_STR "]"
# define PATH_SEPARATOR     '?'
# define PATH_SEPARATOR_STR "?"
#else
# define FILE_SEPARATORS    "/"
# define FILE_SEPARATOR     '/'
# define FILE_SEPARATOR_STR "/"
# define PATH_SEPARATOR     ':'
# define PATH_SEPARATOR_STR ":"
#endif

#define MALLOC_TSD( TSD, bytes ) (TSD)->RxMTMalloc( TSD, bytes )
#define FREE_TSD( TSD, ptr ) (TSD)->RxMTFree( TSD, ptr )

/* special test for va_copy() availability */
#if defined(_MSC_VER)
# if _MSC_VER < 1800
#  define va_copy(dest,src) (dest = src)
# endif
#endif

#endif /* !_RXPACK_H */
