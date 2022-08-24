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

#if defined(OS2) || defined(__OS2__)
# define INCL_DOSMISC
#endif

#include "rxpack.h"

#if !defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
static RexxExitHandler RxExitHandlerForSayTraceRedirection;
#endif

#if defined(USE_REXX6000)
static LONG RxSubcomHandler( RSH_ARG0_TYPE, RSH_ARG1_TYPE, RSH_ARG2_TYPE );
#else
static RexxSubcomHandler RxSubcomHandler;
#endif

#if !defined(HAVE_STRERROR)
/*
 * This function and the following description borrowed from Regina 0.08a
 *
 * Sigh! This must probably be done this way, although it's incredibly
 * backwards. Some versions of gcc comes with a complete set of ANSI C
 * include files, which contains the definition of strerror(). However,
 * that function does not exist in the default libraries of SunOS.
 * To circumvent that problem, strerror() is #define'd to get_sys_errlist()
 * in config.h, and here follows the definition of that function.
 * Originally, strerror() was #defined to sys_errlist[x], but that does
 * not work if string.h contains a declaration of the (non-existing)
 * function strerror().
 *
 * So, this is a mismatch between the include files and the library, and
 * it should not create problems for Regina. However, the _user_ will not
 * encounter any problems until he compiles Regina, so we'll have to
 * clean up after a buggy installation of the C compiler!
 */
char *rxpackage_get_sys_errlist( int num )
{
   extern char *sys_errlist[] ;
   return sys_errlist[num] ;
}
#endif

#if defined( __OS2__ ) || defined( OS2 )
/*
 * We need a getenv() that works in an OS/2 DLL
 */
char *rx_getenv( const char *name )
{
   unsigned char *value;
   if (DosScanEnv( name, &value ) )
      return NULL;
   else
      return value;
}
#define getenv( x ) rx_getenv( x )
#endif

#if defined(WIN32) || defined(WIN64)
# if !defined(REXX_PATH_MAX)
#  define REXX_PATH_MAX 4096
# endif
/*
 * Work around for bug in WIN32 stat() function; can't have trailing slash
 */
# if defined(HAVE__STATI64)
int rx_w32_stat( const char *path, struct _stati64 *buffer )
{
   int lastpos;
   int rc;
   char tmpstring[REXX_PATH_MAX ];

   rc = _stati64( path, buffer );
   if ( rc != 0 && errno == ENOENT )
   {
      if ( path )
      {
         lastpos = strlen( path ) -  1;
         if ( path[lastpos] == '\\' || path[lastpos] == '/')
         {
            memcpy( tmpstring, path, lastpos ) ;
            tmpstring[lastpos] = '\0';
            rc = _stati64( tmpstring, buffer );
         }
      }
   }
   return rc;
}
# else
int rx_w32_stat( const char *path, struct _stat *buffer )
{
   int lastpos;
   int rc;
   char tmpstring[REXX_PATH_MAX ];

   rc = stat( path, buffer );
   if ( rc != 0 && errno == ENOENT )
   {
      if ( path )
      {
         lastpos = strlen( path ) -  1;
         if ( path[lastpos] == '\\'|| path[lastpos] == '/')
         {
            memcpy( tmpstring, path, lastpos ) ;
            tmpstring[lastpos+1] = '\0';
            rc = stat( tmpstring, buffer );
         }
      }
   }
   return rc;
}
# endif
#endif

/*-----------------------------------------------------------------------------
 * Compares buffers for equality ignoring case
 *----------------------------------------------------------------------------*/
int memcmpi( RxPackageGlobalDataDef *RxPackageGlobalData, char *buf1, char *buf2, int len )
{
   register short i=0;
   char c1=0,c2=0;
   for(i=0;i<len;i++)
   {
      if (isupper(*buf1))
         c1 = tolower(*buf1);
      else
         c1 = *buf1;
      if (isupper(*buf2))
         c2 = tolower(*buf2);
      else
         c2 = *buf2;
      if (c1 != c2)
         return(c1-c2);
      ++buf1;
      ++buf2;
   }
 return(0);
}

/***********************************************************************/
/* Function  : Compares two string buffers for equality;               */
/*             case insensitive. Same as stricmp(), strcasecmp() etc.  */
/* Parameters: str1     - first string                                 */
/*             str2     - second string                                */
/* Return    : <0 if str1 < str2,                                      */
/*             =0 if str1 = str2,                                      */
/*             >0 if str1 > str2,                                      */
/***********************************************************************/
int rxstrcmpi( RxPackageGlobalDataDef *RxPackageGlobalData, char *str1, char *str2)
{
   int i,len,len1=strlen(str1),len2=strlen(str2);
   char c1,c2;

   len = min( len1, len2 );
   for( i = 0; i < len; i++ )
   {
      if ( isupper( *str1 ) )
         c1 = tolower( *str1 );
      else
         c1 = *str1;
      if ( isupper( *str2 ) )
         c2 = tolower( *str2 );
      else
         c2 = *str2;
      if ( c1 != c2 )
         return( c1 - c2 );
      ++str1;
      ++str2;
   }
   if ( len1 > len2 )
      return(1);
   if ( len1 < len2 )
      return(-1);
   return(0);
}

/*-----------------------------------------------------------------------------
 * Uppercases the supplied string.
 *----------------------------------------------------------------------------*/
char *make_upper( RxPackageGlobalDataDef *RxPackageGlobalData, char *str )
{
   char *save_str=str;
   while(*str)
   {
      if (islower(*str))
         *str = toupper(*str);
      ++str;
   }
   return(save_str);
}

/*-----------------------------------------------------------------------------
 * Uppercases the supplied string and length.
 *----------------------------------------------------------------------------*/
char *make_upper_with_length( RxPackageGlobalDataDef *RxPackageGlobalData, char *str, int len )
{
   char *save_str=str;
   while(len--)
   {
      if (islower(*str))
         *str = toupper(*str);
      ++str;
   }
   return(save_str);
}

/*-----------------------------------------------------------------------------
 * Lowercases the supplied string.
 *----------------------------------------------------------------------------*/
char *make_lower( RxPackageGlobalDataDef *RxPackageGlobalData, char *str )
{
   char *save_str=str;
   while(*str)
   {
      if (isupper(*str))
         *str = tolower(*str);
      ++str;
   }
   return(save_str);
}

/*-----------------------------------------------------------------------------
 * Allocate memory for a char * based on an RXSTRING
 *----------------------------------------------------------------------------*/
char *AllocString( RxPackageGlobalDataDef *RxPackageGlobalData, char *buf, int bufsize )
{
   char *tempstr=NULL;

   tempstr = (char *)MALLOC_TSD( RxPackageGlobalData, sizeof(char)*(bufsize+1) );
   return tempstr;
}

/*-----------------------------------------------------------------------------
 * Copy a non terminated character array to the nominated buffer (truncate
 * if necessary) and null terminate.
 *----------------------------------------------------------------------------*/
char *MkAsciz( RxPackageGlobalDataDef *RxPackageGlobalData, char *buf, int bufsize, char *str, int len )
{
   bufsize--;     /* Make room for terminating byte */
   if (len > bufsize)
      len = bufsize;
   (void)memcpy( buf, str, len );
   buf[len] = '\0';
   return buf;
}

/*-----------------------------------------------------------------------------
 * Check number of parameters
 *----------------------------------------------------------------------------*/
int my_checkparam( RxPackageGlobalDataDef *RxPackageGlobalData, RFH_ARG0_TYPE name, int argc, int mini, int maxi )
{
   if ( argc < mini )
   {
      RxDisplayError( RxPackageGlobalData, name, "*ERROR* Not enough parameters in call to \"%s\". Minimum %d\n", name, mini );
      return 1;
   }
   if ( argc > maxi
   &&   maxi != 0 )
   {
      RxDisplayError( RxPackageGlobalData, name, "*ERROR* Too many parameters in call to \"%s\". Maximum %d\n", name, maxi );
      return 1;
   }
   return 0;
}

/*-----------------------------------------------------------------------------
 * Create a REXX variable of the specified name and bind the value to it.
 *----------------------------------------------------------------------------*/
int SetRexxVariable( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, ULONG namelen, char *value, ULONG valuelen )
{
   ULONG rc=0L;
   SHVBLOCK shv;
   char buf1[150], buf2[150];
   char *err;

   /*
    * Uppercase the variable before we do anything. That way all debugging
    * output reflects the "real" name we are setting.
    */
   ( void )make_upper( RxPackageGlobalData, name );

   InternalTrace( RxPackageGlobalData, "SetRexxVariable", "\"%s\",%d,\"%s\",%d", name, namelen, value, valuelen );

   if ( RxPackageGlobalData->RxRunFlags & MODE_DEBUG)
   {
      MkAsciz( RxPackageGlobalData, buf1, sizeof( buf1 ), name, namelen );
      MkAsciz( RxPackageGlobalData, buf2, sizeof( buf2 ), value, valuelen );
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "*DEBUG* Setting variable \"%s\" to \"%s\".\n", buf1, buf2 );
   }
   shv.shvnext = ( SHVBLOCK* )NULL;
   shv.shvcode = RXSHV_SET;
   MAKERXSTRING( shv.shvname, name, ( ULONG )namelen );
   MAKERXSTRING( shv.shvvalue, (value) ? value : "", ( ULONG )valuelen );
   shv.shvnamelen = shv.shvname.strlength;
   shv.shvvaluelen = shv.shvvalue.strlength;
   rc = RexxVariablePool( &shv );
   if ( rc == RXSHV_OK
   ||   rc == RXSHV_NEWV )
   {
      if ( RxPackageGlobalData->RxRunFlags & MODE_DEBUG)
      {
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "*DEBUG* NOError Setting variable \"%s\" to \"%s\". %s.\n", buf1, buf2, "OK" );
      }
      return( 0 );
   }
   else
   {
      if ( RxPackageGlobalData->RxRunFlags & MODE_DEBUG)
      {
         switch( rc )
         {
            case RXSHV_TRUNC: err = "Name of Value truncated"; break;
            case RXSHV_BADN : err = "Invalid variable name"; break;
            case RXSHV_MEMFL: err = "Memory problem; probably none"; break;
            case RXSHV_BADF : err = "Invalid function code"; break;
            case RXSHV_NOAVL: err = "Interface not available"; break;
            default:     err = "Unknown error with RexxVariablePool()"; break;
         }
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "*DEBUG* Error Setting variable \"%s\" to \"%s\". %s.\n", buf1, buf2, err );
      }
      return( 1 );
   }
}

/*-----------------------------------------------------------------------------
 * This function gets a REXX variable and returns it in an RXSTRING
 *----------------------------------------------------------------------------*/
RXSTRING *GetRexxVariable( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, RXSTRING *value, int suffix )
{
   static SHVBLOCK shv;
   char variable_name[350];
   ULONG rc = 0L;

   InternalTrace( RxPackageGlobalData, "GetRexxVariable", "%s,%x,%d", name, value, suffix );

   shv.shvnext=NULL;                                /* only one block */
   shv.shvcode = RXSHV_FETCH;
   /*
    * This calls the RexxVariablePool() function for each value. This is
    * not the most efficient way of doing this.
    */
   if ( suffix == RXPACK_NOTAIL)
      strcpy( variable_name, name );
   else
      sprintf( variable_name, "%s%-d", name, suffix );
   ( void )make_upper( RxPackageGlobalData, variable_name );
   /*
    * Now (attempt to) get the REXX variable
    * Set shv.shvvalue to NULL to force interpreter to allocate memory.
    */
   MAKERXSTRING( shv.shvname, variable_name, (ULONG)strlen( variable_name ) );
   shv.shvvalue.strptr = NULL;
   shv.shvvalue.strlength = 0;
   /*
    * One or both of these is needed, too <sigh>
    */
   shv.shvnamelen = (ULONG)strlen( variable_name );
   shv.shvvaluelen = 0;
   rc = RexxVariablePool( &shv );              /* Set the REXX variable */
   if ( rc == RXSHV_OK )
   {
      value->strptr = ( RXSTRING_STRPTR_TYPE )MALLOC_TSD( RxPackageGlobalData, ( sizeof( char )*shv.shvvalue.strlength ) + 1 );
      if ( value->strptr != NULL )
      {
         value->strlength = shv.shvvalue.strlength;
         memcpy( value->strptr, shv.shvvalue.strptr, value->strlength );
         *( value->strptr + value->strlength ) = '\0';
      }
#if defined(REXXFREEMEMORY)
      RexxFreeMemory( shv.shvvalue.strptr );
#elif defined(WIN32) && (defined(USE_OREXX) || defined(USE_WINREXX) || defined(USE_QUERCUS) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(WIN64) && (defined(USE_OREXX) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(USE_OS2REXX)
      DosFreeMem( shv.shvvalue.strptr );
#else
      free( shv.shvvalue.strptr );
#endif
   }
   else
      value = NULL;
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      if ( value )
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariable with value \"%s\" Length: %ld\n", value->strptr, value->strlength );
      else
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariable  with value (null)\n" );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( value );
}

/*-----------------------------------------------------------------------------
 * This function gets a REXX variable as a number and returns it
 * Returns NULL if the variable is not set; an invalid number returns 0
 *----------------------------------------------------------------------------*/
int *GetRexxVariableInteger( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, int *value, int suffix )
{
   static SHVBLOCK shv;
   char variable_name[350];
   ULONG rc = 0L;

   InternalTrace( RxPackageGlobalData, "GetRexxVariableNumber", "%s,%x,%d", name, value, suffix );

   shv.shvnext=NULL;                                /* only one block */
   shv.shvcode = RXSHV_FETCH;
   /*
    * This calls the RexxVariablePool() function for each value. This is
    * not the most efficient way of doing this.
    */
   if ( suffix == RXPACK_NOTAIL)
      strcpy( variable_name, name );
   else
      sprintf( variable_name, "%s%-d", name, suffix );
   ( void )make_upper( RxPackageGlobalData, variable_name );
   /*
    * Now (attempt to) get the REXX variable
    * Set shv.shvvalue to NULL to force interpreter to allocate memory.
    */
   MAKERXSTRING( shv.shvname, variable_name, (ULONG)strlen( variable_name ) );
   shv.shvvalue.strptr = NULL;
   shv.shvvalue.strlength = 0;
   /*
    * One or both of these is needed, too <sigh>
    */
   shv.shvnamelen = (ULONG)strlen( variable_name );
   shv.shvvaluelen = 0;
   rc = RexxVariablePool( &shv );              /* Get the REXX variable */
   if ( rc == RXSHV_OK )
   {
      if ( RxStrToInt( RxPackageGlobalData, &shv.shvvalue, value ) == RXPACK_INVALID )
         value = NULL;
#if defined(REXXFREEMEMORY)
      RexxFreeMemory( shv.shvvalue.strptr );
#elif defined(WIN32) && (defined(USE_OREXX) || defined(USE_WINREXX) || defined(USE_QUERCUS) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(WIN64) && (defined(USE_OREXX) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(USE_OS2REXX)
      DosFreeMem( shv.shvvalue.strptr );
#else
      free( shv.shvvalue.strptr );
#endif
   }
   else
      value = NULL;
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      if ( value )
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariableInteger with value \"%d\"\n", *value );
      else
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariableInteger  with value (null)\n" );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( value );
}

/*-----------------------------------------------------------------------------
 * This function gets a REXX variable as a long number and returns it
 * Returns NULL if the variable is not set; an invalid number returns 0
 *----------------------------------------------------------------------------*/
int GetRexxVariableULong( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, unsigned long *value, int suffix )
{
   static SHVBLOCK shv;
   char variable_name[350];
   int rc = RXPACK_OK;

   InternalTrace( RxPackageGlobalData, "GetRexxVariableULong", "%s,%x,%d", name, value, suffix );

   shv.shvnext=NULL;                                /* only one block */
   shv.shvcode = RXSHV_FETCH;
   /*
    * This calls the RexxVariablePool() function for each value. This is
    * not the most efficient way of doing this.
    */
   if ( suffix == RXPACK_NOTAIL)
      strcpy( variable_name, name );
   else
      sprintf( variable_name, "%s%-d", name, suffix );
   ( void )make_upper( RxPackageGlobalData, variable_name );
   /*
    * Now (attempt to) get the REXX variable
    * Set shv.shvvalue to NULL to force interpreter to allocate memory.
    */
   MAKERXSTRING( shv.shvname, variable_name, (ULONG)strlen( variable_name ) );
   shv.shvvalue.strptr = NULL;
   shv.shvvalue.strlength = 0;
   /*
    * One or both of these is needed, too <sigh>
    */
   shv.shvnamelen = (ULONG)strlen( variable_name );
   shv.shvvaluelen = 0;
   rc = RexxVariablePool( &shv );              /* Get the REXX variable */
   if ( rc == RXSHV_OK )
   {
      if ( RxStrToULong( RxPackageGlobalData, &shv.shvvalue, value ) == RXPACK_INVALID )
      {
         rc = RXPACK_INVALID;
         value = NULL;
      }
#if defined(REXXFREEMEMORY)
      RexxFreeMemory( shv.shvvalue.strptr );
#elif defined(WIN32) && (defined(USE_OREXX) || defined(USE_WINREXX) || defined(USE_QUERCUS) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(WIN64) && (defined(USE_OREXX) || defined(USE_OOREXX))
      GlobalFree( shv.shvvalue.strptr );
#elif defined(USE_OS2REXX)
      DosFreeMem( shv.shvvalue.strptr );
#else
      free( shv.shvvalue.strptr );
#endif
   }
   else
   {
      rc = RXPACK_MISSING;
      value = NULL;
   }
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      if ( value )
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariableULong with rc: %d: value \"%lu\"\n", rc, *value );
      else
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Exit GetRexxVariableULong with rc: %d: value (null)\n", rc );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( rc );
}

/*-----------------------------------------------------------------------------
 * Drop a REXX variable of the specified name
 *----------------------------------------------------------------------------*/
int DropRexxVariable( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, int namelen )
{
   ULONG  rc=0L;
   SHVBLOCK       shv;

   /*
    * Uppercase the variable before we do anything. That way all debugging
    * output reflects the "real" name we are setting.
    */
   ( void )make_upper( RxPackageGlobalData, name );

   InternalTrace( RxPackageGlobalData, "DropRexxVariable", "\"%s\",%d", name, namelen );

   if ( RxPackageGlobalData->RxRunFlags & MODE_DEBUG)
   {
      char buf1[50];

      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "*DEBUG* Dropping variable \"%s\".\n",
         MkAsciz( RxPackageGlobalData, buf1, sizeof( buf1 ), name, namelen ) );
   }
   shv.shvnext = ( SHVBLOCK* )NULL;
   shv.shvcode = RXSHV_DROPV;
   MAKERXSTRING( shv.shvname, name, ( ULONG )namelen );
   shv.shvnamelen = shv.shvname.strlength;
   rc = RexxVariablePool( &shv );
   if ( rc == RXSHV_OK )
      return( 0 );
   else
   {
      if ( RxPackageGlobalData->RxRunFlags & MODE_DEBUG)
      {
         char buf1[50];
         char *err;
         switch( rc )
         {
            case RXSHV_TRUNC: err = "Name of Value truncated"; break;
            case RXSHV_BADN : err = "Invalid variable name"; break;
            case RXSHV_MEMFL: err = "Memory problem; probably none"; break;
            case RXSHV_BADF : err = "Invalid function code"; break;
            case RXSHV_NOAVL: err = "Interface not available"; break;
            default:     err = "Unknown error with RexxVariablePool()"; break;
         }

         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
            "*DEBUG* Error Dropping variable \"%s\". %s.\n",
            MkAsciz( RxPackageGlobalData, buf1, sizeof( buf1 ), name, namelen ),
            err );
      }
      return( 1 );
   }
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to a ULONG boolean. Input can be ON, Yes, 1 or OFF No, 0
 *----------------------------------------------------------------------------*/
int RxStrToBool( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, ULONG *result )
{
   char   *p=(char *)ptr->strptr;
   int    len=ptr->strlength;

   if (memcmp(p,"YES",len) == 0
   ||  memcmp(p,"yes",len) == 0
   ||  memcmp(p,"Y",len) == 0
   ||  memcmp(p,"y",len) == 0
   ||  memcmp(p,"ON",len) == 0
   ||  memcmp(p,"on",len) == 0
   ||  memcmp(p,"1",len) == 0)
   {
      *result = 1;
      return(0);
   }
   if (memcmp(p,"NO",len) == 0
   ||  memcmp(p,"no",len) == 0
   ||  memcmp(p,"N",len) == 0
   ||  memcmp(p,"n",len) == 0
   ||  memcmp(p,"OFF",len) == 0
   ||  memcmp(p,"off",len) == 0
   ||  memcmp(p,"0",len) == 0)
   {
      *result = 0;
      return(0);
   }
   RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"bool\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
   return (RXPACK_INVALID);
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to a UINT boolean. Input can be ON, Yes, 1 or OFF No, 0
 *----------------------------------------------------------------------------*/
int RxStrToIntBool( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, unsigned int *result )
{
   char   *p=(char *)ptr->strptr;
   int    len=ptr->strlength;

   if (memcmp(p,"YES",len) == 0
   ||  memcmp(p,"yes",len) == 0
   ||  memcmp(p,"Y",len) == 0
   ||  memcmp(p,"y",len) == 0
   ||  memcmp(p,"ON",len) == 0
   ||  memcmp(p,"on",len) == 0
   ||  memcmp(p,"1",len) == 0)
   {
      *result = 1;
      return(0);
   }
   if (memcmp(p,"NO",len) == 0
   ||  memcmp(p,"no",len) == 0
   ||  memcmp(p,"N",len) == 0
   ||  memcmp(p,"n",len) == 0
   ||  memcmp(p,"OFF",len) == 0
   ||  memcmp(p,"off",len) == 0
   ||  memcmp(p,"0",len) == 0)
   {
      *result = 0;
      return(0);
   }
   RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"bool\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
   return (RXPACK_INVALID);
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to signed integer. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToInt( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int *result )
{
   unsigned int i=0;
   char   *p=(char *)ptr->strptr;
   int    sum=0;
   int    neg=0;
   int    rc=0;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"int\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToInt() Input string is \"%s\" Result is %d\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to signed short integer. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToShort( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, short *result )
{
   unsigned short i=0;
   char   *p=(char *)ptr->strptr;
   short    sum=0;
   short    neg=0;
   short    rc=0;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"short\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToShort() Input string is \"%s\" Result is %d\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to unsigned integer. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits and no signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToUInt( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, unsigned int *result )
{
   int    i=0,rc=0;
   char   *p=(char *)ptr->strptr;
   unsigned int sum=0;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p++ - '0');
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"unsigned int\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc= RXPACK_INVALID;
         break;
      }
   }
   if ( rc == 0 )
      *result = sum;
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToUInt() Input string is \"%s\" Result is %d\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to signed long. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToLong( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, long *result )
{
   unsigned int i=0;
   int    rc=0;
   char   *p=(char *)ptr->strptr;
   long   sum=0L;
   int    neg=0;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"long\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToLong() Input string is \"%s\" Result is %ld\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to unsigned long. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits and no signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToULong( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, unsigned long *result )
{
   int    i=0,rc=0;
   char          *p=(char *)ptr->strptr;
   unsigned long sum=0L;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p++ - '0');
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"unsigned long\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
   }
   if ( rc == 0 )
      *result = sum;
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToULong() Input string is \"%s\" Result is %ld\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to signed long long. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToLongLong( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, rx_long_long *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   rx_long_long  sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"long long\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", RX_LL_FORMAT );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to int8_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToINT8( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int8_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   int8_t        sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"int8\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRId8 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to uint8_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToUINT8( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint8_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   uint8_t       sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"uint8\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRIu8 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to int16_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToINT16( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int16_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   int16_t       sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"int16\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRId16 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to uint16_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToUINT16( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint16_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   uint16_t      sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"uint16\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRIu16 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to int32_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToINT32( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int32_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   int32_t       sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"int32\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRId32 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to uint32_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToUINT32( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint32_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   uint32_t      sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"uint32\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRIu32 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to int64_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToINT64( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int64_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   int64_t       sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"int64\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRId64 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to uint64_t. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToUINT64( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint64_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   uint64_t      sum=0;
   int           neg=0;
   int           rc=0;
   char tmp_format[150];

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p - '0');
      else if ( i == ptr->strlength && *p == '-' )
         neg = 1;
      else if ( i == ptr->strlength && *p == '+' )
         ;
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"uint64\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   if ( rc == 0 )
   {
      if ( neg )
         sum *= -1;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxStrToLongLong() Input string is \"%%s\" Result is %s\n", PRIu64 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to double. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToDouble( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, double *result )
{
   char *endptr;
   double sum;
   char *p=(char *)ptr->strptr;

   errno = 0;
   sum = (double)strtod( p, &endptr );
   if ( sum == 0
   &&   errno != 0 )
   {
      RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"double\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
      return RXPACK_INVALID;
   }
   if ( sum == 0
   &&   (char *)endptr == p )
   {
      RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"double\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
      return -1;
    }
   *result = sum;
   return 0;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to float. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits with or without signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToFloat( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, float *result )
{
   char *endptr;
   float sum;
   char *p=(char *)ptr->strptr;

   errno = 0;
#if defined(HAVE_STRTOF)
   sum = (float)strtof( p, &endptr );
#else
   sum = (float)strtod( p, &endptr );
#endif
   if ( sum == 0
   &&   errno != 0 )
   {
      RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"float\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
      return RXPACK_INVALID;
   }
   if ( sum == 0
   &&   (char *)endptr == p )
   {
      RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"float\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
      return -1;
   }
   *result = sum;
   return 0;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING to void pointer. Return 0 if OK and -1 if error.
 * Assumes a string of decimal digits and no signs and does not check for overflow!
 *----------------------------------------------------------------------------*/
int RxStrToPointer( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, void **result )
{
   int    i=0,rc=0;
   char   *p=(char *)ptr->strptr;
   unsigned long sum=0L;
   void *rptr;

   for (i = ptr->strlength; i; i--)
   {
      if (isdigit(*p))
         sum = sum * 10 + (*p++ - '0');
      else
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid \"pointer\" value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
   }
   if ( rc == 0 )
   {
      rptr = (void *)sum;
      *result = rptr;
   }
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In RxStrToPointer() Input string is \"%s\" Result is %ld\n",__FILE__,__LINE__,RxPackGetThreadID(),ptr->strptr, sum);)
   return rc;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of char pointers
 * Allocates memory for the char pointers, NOT the strings. The pointers
 * need to be freed using RxFreeCharArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToCharArray( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, char ***retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   RXSTRING value;
   char **ret, **tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
   {
      RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid stem value of \"%s\" in call to \"%s\".\n", ptr, RxPackageGlobalData->FName);
      return RXPACK_INVALID;
   }
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items char *
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (char **)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(char *) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * char **
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariable( RxPackageGlobalData, ptr->strptr, &value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = value.strptr;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToCharArray()
 *----------------------------------------------------------------------------*/
void RxFreeCharArray( RxPackageGlobalDataDef *RxPackageGlobalData, char **ptr, int num_args )
{
#if 0
   int      i;
#endif

   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
#if 0
   /*
    * Free each item...
    * NO WAY. These are passed as arguments from the Rexx interpreter
    * and cannot be freed by us.
    */
   for ( i = 0; i < num_args; i++ )
   {
      FREE_TSD( RxPackageGlobalData, *ptr );
   }
#endif
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of ULONGs
 * Allocates memory for ULONGs which needs to be freed by the caller
 * using RxFreeULongArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToULongArray( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, unsigned long **retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   unsigned long value;
   unsigned long *ret,*tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
      return RXPACK_INVALID;
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items unsigned longs
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (unsigned long *)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(unsigned long) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * ULONG *
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, (int *)&value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = (unsigned long)value;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToULongArray()
 *----------------------------------------------------------------------------*/
void RxFreeULongArray( RxPackageGlobalDataDef *RxPackageGlobalData, unsigned long *ptr )
{
   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of LONGs
 * Allocates memory for LONGs which needs to be freed by the caller
 * using RxFreeLongArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToLongArray( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, long **retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   long     value;
   long     *ret,*tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
      return RXPACK_INVALID;
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items unsigned longs
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (long *)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(long) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * LONG *
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, (int *)&value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = (long)value;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToLongArray()
 *----------------------------------------------------------------------------*/
void RxFreeLongArray( RxPackageGlobalDataDef *RxPackageGlobalData, long *ptr )
{
   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of INTs
 * Allocates memory for INTs which needs to be freed by the caller
 * using RxFreeIntArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToIntArray( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, int **retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   int      value;
   int      *ret,*tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
      return RXPACK_INVALID;
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items unsigned longs
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (int *)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(int) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * INT *
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, (int *)&value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = (int)value;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToIntArray()
 *----------------------------------------------------------------------------*/
void RxFreeIntArray( RxPackageGlobalDataDef *RxPackageGlobalData, int *ptr )
{
   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of UINTs
 * Allocates memory for UINTs which needs to be freed by the caller
 * using RxFreeUIntArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToUIntArray( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, unsigned int **retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   unsigned int value;
   unsigned int *ret,*tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
      return RXPACK_INVALID;
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items unsigned longs
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (unsigned int *)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(unsigned int) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * UNSIGNED INT *
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, (int *)&value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = (unsigned int)value;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToUIntArray()
 *----------------------------------------------------------------------------*/
void RxFreeUIntArray( RxPackageGlobalDataDef *RxPackageGlobalData, unsigned int *ptr )
{
   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Converts a RXSTRING containing a stem name to an array of UINT16s
 * Allocates memory for UINT16s which needs to be freed by the caller
 * using RxFreeUIntArray()
 * Returns the number of items in the array
 *----------------------------------------------------------------------------*/
int RxStemToUINT16Array( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint16_t **retval )
{
   int      len=ptr->strlength;
   int      num_items,i;
   unsigned int value;
   uint16_t *ret,*tmp;

   /*
    * Validate that 'ptr' is a stem name.
    */
   if ( ptr->strptr[len-1] != '.' )
      return RXPACK_INVALID;
   /*
    * Get the number of items in the array
    */
   if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, &num_items, 0 ) == NULL )
      return RXPACK_INVALID;
   /*
    * Allocate num_items unsigned longs
    */
   if ( num_items )
   {
      if ( ( ret = tmp = (uint16_t *)MALLOC_TSD( RxPackageGlobalData, num_items * sizeof(uint16_t) ) ) == NULL )
         return RXPACK_INVALID;
      /*
       * Get each stem value, and set the equivalent entry in the allocated
       * UNSIGNED INT *
       */
      for ( i = 0; i < num_items; i++, tmp++ )
      {
         if ( GetRexxVariableInteger( RxPackageGlobalData, ptr->strptr, (int *)&value, i+1 ) == NULL )
            return RXPACK_INVALID;
         *tmp = (uint16_t)value;
      }
      *retval = ret;
   }
   else
   {
      *retval = NULL;
   }
   return num_items;
}

/*-----------------------------------------------------------------------------
 * Frees memory allocated by RxStemToUINT16Array()
 *----------------------------------------------------------------------------*/
void RxFreeUINT16Array( RxPackageGlobalDataDef *RxPackageGlobalData, uint16_t *ptr )
{
   /*
    * Validate that 'ptr' is valid.
    */
   if ( ptr == NULL )
      return;
   /*
    * Free the ptr
    */
   FREE_TSD( RxPackageGlobalData, ptr );
   return;
}

/*-----------------------------------------------------------------------------
 * Sets the variable specified to the number value supplied.
 *----------------------------------------------------------------------------*/
int RxNumberToVariable( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, ULONG number )
{
   int  len;
   char buf[50];

   /*
    * Convert the number to a string...
    */
   len = sprintf( buf, "%ld", number );
   /*
    * Set the Rexx variable if valid
    */
   if ( ptr
   &&   ptr->strlength
   &&   ptr->strptr )
   {
      if ( SetRexxVariable( RxPackageGlobalData, ptr->strptr, ptr->strlength, buf, len ) == 1 )
         return RXPACK_INVALID;
   }
   return 0;
}

/*-----------------------------------------------------------------------------
 * Converts a binary RXSTRING (8 chars) to uint8_t. Return 0 if OK and -1 if error.
 *----------------------------------------------------------------------------*/
int RxBinaryStringToUINT8( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *ptr, uint8_t *result )
{
   unsigned int  i=0;
   char          *p=(char *)ptr->strptr;
   uint8_t       sum=0;
   int           rc=0;
   char tmp_format[8150];

   for (i = ptr->strlength; i; i--)
   {
      if (*p == '1')
         sum = sum * 10 + (*p - '0');
      else if ( *p != '0' )
      {
         RxDisplayError( RxPackageGlobalData, (RFH_ARG0_TYPE)RxPackageGlobalData->FName, "*ERROR* Invalid binary value of \"%s\" in call to \"%s\".\n", p, RxPackageGlobalData->FName);
         rc = RXPACK_INVALID;
         break;
      }
      p++;
   }
   *result = sum;
   sprintf( tmp_format, "%%s-%%d Thread(%%ld): In RxBinaryStringToUINT8() Input string is \"%%s\" Result is %s\n", PRIu8 );
   DEBUGDUMP(fprintf( stderr, tmp_format, __FILE__, __LINE__, RxPackGetThreadID(), ptr->strptr, sum );)
   return rc;
}


/*-----------------------------------------------------------------------------
 * This is called when in VERBOSE mode. It prints function name & arg values.
 *----------------------------------------------------------------------------*/
RxPackageGlobalDataDef *FunctionPrologue( RxPackageGlobalDataDef *RxPackageGlobalData, PackageInitialiser *RxPackageInitialiser, char *constantPrefix, char *name, ULONG argc, RFH_ARG2_TYPE argv )
{
#define MAX_BUF 80
   ULONG i = 0L;
   char buf[MAX_BUF + 1];
   RxPackageGlobalDataDef *GlobalData;

   if ( RxPackageGlobalData == NULL )
   {
      /*
       * The package *LoadFuncs() has not been called! So do the thread-specific
       * initialisation now.
       */
#if defined(DYNAMIC_LIBRARY)
      /*
       * Get thread-safe-data for DLL. EXE global data is not thread-safe
       */
      int rc=0;
      GlobalData = GLOBAL_ENTRY_POINT();
      InitRxPackage( GlobalData, RxPackageInitialiser, constantPrefix, &rc );
#else
      /*
       * GLOBAL_ENTRY_POINT() for non-DLL has already been run by loader.c
       */
      GlobalData = __rxpack_get_tsd();
#endif
   }
   else
   {
      GlobalData = RxPackageGlobalData;
   }

   if ( GlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( GlobalData->RxTraceFilePointer, "++\n" );
      (void)fprintf( GlobalData->RxTraceFilePointer, "++ Call %s%s from thread %ld\n", name, argc ? "" : "()", RxPackGetThreadID() );
      for (i = 0; i < argc; i++)
      {
         if ( RXNULLSTRING(argv[i]) )
         {
            (void)fprintf( GlobalData->RxTraceFilePointer, "++ %3ld: *No Value*\n", i+1);
         }
         else if ( RXSTRLEN(argv[i]) > MAX_BUF )
         {
            (void)fprintf( GlobalData->RxTraceFilePointer, "++ %3ld: \"%s ...\" Length: %ld\n",
                           i+1,
                           MkAsciz( GlobalData, buf, sizeof(buf), (char *)RXSTRPTR( argv[i] ), (int)RXSTRLEN( argv[i] ) ),
                           RXSTRLEN(argv[i]) );
         }
         else
         {
            (void)fprintf( GlobalData->RxTraceFilePointer, "++ %3ld: \"%s\" Length: %ld\n",
                           i+1,
                           MkAsciz( GlobalData, buf, sizeof(buf), (char *)RXSTRPTR( argv[i] ), (int)RXSTRLEN( argv[i] ) ),
                           RXSTRLEN(argv[i]) );
         }
      }
      fflush( GlobalData->RxTraceFilePointer );
   }
   /*
    * This should be replaced with an integer to make things
    * run faster!!
    */
   if ( strcmp( name, GlobalData->FName ) != 0 )
      strcpy( GlobalData->FName, name );

   return GlobalData;
}

/*-----------------------------------------------------------------------------
 * This is called when in VERBOSE mode. It prints function name & return value.
 *----------------------------------------------------------------------------*/
long FunctionEpilogue( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, long rc )
{
   if ( RxPackageGlobalData == NULL )
   {
      return rc;
   }

   if ( RxPackageGlobalData->RxRunFlags & MODE_VERBOSE
   &&   RxPackageGlobalData->RxTraceFilePointer )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "++\n" );
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "++ Exiting %s() with %ld from thread %ld\n", name, rc, RxPackGetThreadID() );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return rc;
}

/*-----------------------------------------------------------------------------
 * This is called when in VERBOSE mode. It prints function name & return value.
 *----------------------------------------------------------------------------*/
void FunctionTrace( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, ... )
{
   va_list argptr;
   char *fmt=NULL;

   if ( RxPackageGlobalData == NULL )
   {
      return;
   }

   if ( RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>\n" );
      if ( name ) (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ">> Call %s(", name );
      va_start( argptr, name );
      fmt = va_arg( argptr, char *);
      if (fmt != NULL)
      {
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, ">> " );
         (void)vfprintf( RxPackageGlobalData->RxTraceFilePointer, fmt, argptr );
      }
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ")\n");
      fflush( RxPackageGlobalData->RxTraceFilePointer );
      va_end( argptr );
   }
   return;
}

/*-----------------------------------------------------------------------------
 * This is called when run_flags & MODE_INTERNAL is true
 *----------------------------------------------------------------------------*/
void InternalTrace( RxPackageGlobalDataDef *RxPackageGlobalData, char *name, ...)
{
   va_list argptr;
   char *fmt=NULL;

   if ( RxPackageGlobalData != NULL
   &&   RxPackageGlobalData->RxRunFlags & MODE_INTERNAL )
   {
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Call %s(", name );
      va_start( argptr, name );
      fmt = va_arg( argptr, char *);
      if (fmt != NULL)
      {
         (void)vfprintf( RxPackageGlobalData->RxTraceFilePointer, fmt, argptr );
      }
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ") from thread %ld\n", RxPackGetThreadID() );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
      va_end( argptr );
   }
   return;
}

void RxDisplayError( RxPackageGlobalDataDef *RxPackageGlobalData, RFH_ARG0_TYPE name, ...)
{
   va_list argptr;
   char *fmt=NULL;

   if ( RxPackageGlobalData != NULL )
   {
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ">>>> Calling %s(", name );
      va_start( argptr, name );
      fmt = va_arg( argptr, char *);
      if (fmt != NULL)
      {
         (void)vfprintf( RxPackageGlobalData->RxTraceFilePointer, fmt, argptr );
      }
      (void) fprintf( RxPackageGlobalData->RxTraceFilePointer, ") from thread %ld\n", RxPackGetThreadID() );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
      va_end( argptr );
   }
   return;
}


/*-----------------------------------------------------------------------------
 * This function registers the default subcommand handler...
 *----------------------------------------------------------------------------*/
int RegisterRxSubcom( RxPackageGlobalDataDef *RxPackageGlobalData, RexxSubcomHandler ptr)
{
   ULONG rc=0L;

   InternalTrace( RxPackageGlobalData, "RegisterRxSubcom", NULL );

#if defined(USE_REXX6000)
   rc = RexxRegisterSubcom( (RRSE_ARG0_TYPE)RXPACKAGENAME,
                            (RRSE_ARG1_TYPE)( (ptr) ? ptr : RxSubcomHandler ),
                            (RRSE_ARG2_TYPE)NULL );
#else
   rc = RexxRegisterSubcomExe( (RRSE_ARG0_TYPE)RXPACKAGENAME,
                               (RRSE_ARG1_TYPE)( (ptr) ? ptr : RxSubcomHandler ),
                               (RRSE_ARG2_TYPE)NULL );
#endif
   if ( rc != RXSUBCOM_OK )
      return 1;
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function registers the package's RXINI exit handler...
 *----------------------------------------------------------------------------*/
int RegisterRxInit( RxPackageGlobalDataDef *RxPackageGlobalData, RexxExitHandler ptr, char *name)
{
   ULONG rc=RXEXIT_OK;

   InternalTrace( RxPackageGlobalData, "RegisterRxInit", "Name: %s Addr: %ld", name, ptr );

   if ( (void *)ptr )
   {
      rc = RexxRegisterExitExe( (RREE_ARG0_TYPE)name,
                                (RREE_ARG1_TYPE)(ptr),
                                (RREE_ARG2_TYPE)NULL );
   }
   InternalTrace( RxPackageGlobalData, "RegisterRxInit", "returning %d", rc );
   if ( rc != RXEXIT_OK )
      return 1;
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function registers the external functions...
 *----------------------------------------------------------------------------*/
int RegisterRxFunctions( RxPackageGlobalDataDef *RxPackageGlobalData, RexxFunction *RxPackageFunctions, char *name )
{
   RexxFunction *func=NULL;
   ULONG rc=0L;

   InternalTrace( RxPackageGlobalData, "RegisterRxFunctions", NULL );

   for ( func = RxPackageFunctions; func->InternalName; func++ )
   {
#if defined(DYNAMIC_LIBRARY)
# if !defined(USE_REXX6000)
      if (func->DllLoad)
      {
         char *err;
         rc = RexxRegisterFunctionDll( func->ExternalName,
              name,
              func->InternalName );
         if ( rc != RXFUNC_OK )
         {
            int fatal = 1;
            switch( rc )
            {
               case RXFUNC_DEFINED:   err = "Already registered"; fatal = 0; break;
               case RXFUNC_NOMEM :    err = "Out of memory"; break;
               case RXFUNC_NOTREG :   err = "Not registered"; break;
               case RXFUNC_MODNOTFND: err = "Module not found"; break;
               case RXFUNC_ENTNOTFND: err = "Entry point not found"; break;
               case RXFUNC_NOTINIT:   err = "Not initialised"; break;
               case RXFUNC_BADTYPE:   err = "Bad argument?"; break;
               default:     err = "Unknown error with RexxRegisterFunctionDll()"; break;
            }
            if ( fatal )
            {
               (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
                  "FATAL Error Registering internal \"%s\" external \"%s\" in \"%s\". %s.\n",
                  func->InternalName,
                  func->ExternalName,
                  name,
                  err );
               exit(1);
            }
            else
            {
               (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
                  "*DEBUG* Error Registering internal \"%s\" external \"%s\" in \"%s\". %s.\n",
                  func->InternalName,
                  func->ExternalName,
                  name,
                  err );
            }
         }
         InternalTrace( RxPackageGlobalData, "RegisterRxFunctions","%s-%d: Registered (DLL) %s with rc = %ld",__FILE__,__LINE__,func->ExternalName,rc);
      }
# endif
#else
# if defined(WIN32) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
      (void)RexxDeregisterFunction( func->ExternalName );
# endif
# if defined(USE_REXX6000)
      rc = RexxRegisterFunction( func->ExternalName, func->EntryPoint, NULL );
      InternalTrace( RxPackageGlobalData, "RegisterRxFunctions","%s-%d: Registered (EXE) %s with rc = %d",__FILE__,__LINE__,func->ExternalName,rc);
# else
      rc = RexxRegisterFunctionExe( func->ExternalName, func->EntryPoint );
      InternalTrace( RxPackageGlobalData, "RegisterRxFunctions","%s-%d: Registered (EXE) %s with rc = %d",__FILE__,__LINE__,func->ExternalName,rc);
# endif
#endif
      if (rc != RXFUNC_OK
#ifdef RXFUNC_DUP
      &&  rc != RXFUNC_DUP /* bug in Object Rexx for Linux */
#else
      &&  rc != RXFUNC_DEFINED
#endif
      &&  rc != RXFUNC_NOTREG)
         return 1;
   }
#if !defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
   if ( RexxRegisterExitExe( ( RREE_ARG0_TYPE )RXPACKAGENAME,
                             ( RREE_ARG1_TYPE )RxExitHandlerForSayTraceRedirection,
                             ( RREE_ARG2_TYPE )NULL) != RXEXIT_OK )
      return 1;
#endif
   return 0 ;
}

/*-----------------------------------------------------------------------------
 * This function queries if the supplied external function has been registered
 *----------------------------------------------------------------------------*/
int QueryRxFunction( RxPackageGlobalDataDef *RxPackageGlobalData, char *funcname )
{
   InternalTrace( RxPackageGlobalData, "QueryRxFunction", "%s", funcname );

#if defined(USE_REXX6000)
   if ( RexxQueryFunction( funcname, NULL ) == RXFUNC_OK )
#else
   if ( RexxQueryFunction( funcname ) == RXFUNC_OK )
#endif
   {
      DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): Query function %s - found\n",__FILE__,__LINE__,RxPackGetThreadID(),funcname);)
      return 1;
   }
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): Query function %s - not found\n",__FILE__,__LINE__,RxPackGetThreadID(),funcname);)
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function deregisters the external functions...
 *----------------------------------------------------------------------------*/
int DeregisterRxFunctions( RxPackageGlobalDataDef *RxPackageGlobalData, RexxFunction *RxPackageFunctions, int verbose )
{
   RexxFunction *func=NULL;
   int rc=0;

   InternalTrace( RxPackageGlobalData, "DeregisterRxFunctions", "%d", verbose );

   for ( func = RxPackageFunctions; func->InternalName; func++ )
   {
      rc = RexxDeregisterFunction( func->ExternalName );
      if ( verbose )
         fprintf(stderr,"Deregistering...%s - %d\n",func->ExternalName,rc);
      DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): Deregistered %s with rc %d\n",__FILE__,__LINE__,RxPackGetThreadID(),func->ExternalName,rc);)
   }
#if !defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
   RexxDeregisterExit( RXPACKAGENAME, NULL );
#endif
   return 0 ;
}

/*-----------------------------------------------------------------------------
 * This function sets all package constants...
 *----------------------------------------------------------------------------*/
int SetPackageConstants( RxPackageGlobalDataDef *RxPackageGlobalData, RxPackageConstantDef *RxConstants, int drop )
{
   ULONG varlen=0,vallen=0;
   int rc;
   char varname[250];
   char *value=NULL;
   char buf[100];
   RxPackageConstantDef *con=NULL;

   InternalTrace( RxPackageGlobalData, "SetPackageConstants", "Drop: %d", drop  );

   /*
    * Before we set the new constants, drop the previous stem.
    * This may fail because we don't have a stem yet.
    */
   for ( con=RxConstants; con->name; con++ )
   {
      if ( drop )
      {
         varlen = sprintf( varname, "%s%s",
                           RxPackageGlobalData->PreviousConstantPrefix,
                           con->name );
         DropRexxVariable( RxPackageGlobalData, varname, varlen );
      }
      varlen = sprintf( varname, "%s%s",
                        RxPackageGlobalData->ConstantPrefix,
                        con->name );
      switch( con->type )
      {
         case 0:
            vallen = sprintf( buf, "%ld", con->numeric_value );
            value = buf;
            break;
         case 1:
            vallen = (ULONG)strlen( con->text_value );
            value = con->text_value;
            break;
         case 2:
            vallen = sprintf( buf, "%f", con->double_value );
            value = buf;
            break;
         case 3:
            vallen = sprintf( buf, "%c", con->char_value );
            value = buf;
            break;
         default:
            break;
      }
      /*
       * Now set the Rexx variable
       */
      rc = SetRexxVariable( RxPackageGlobalData, varname, varlen, value, vallen );
   }
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function is called to initialise the package
 *----------------------------------------------------------------------------*/
RxPackageGlobalDataDef *InitRxPackage( RxPackageGlobalDataDef *RxPackageGlobalData, PackageInitialiser *ptr, char *constantPrefix, int *rc )
{
   char *env;

   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): Start of InitRxPackage. RxPackageGlobalData: %p\n",__FILE__,__LINE__,RxPackGetThreadID(),RxPackageGlobalData);)
#if 0
   if ( MyGlob )
   {
      RxPackageGlobalData = MyGlob;
      RxPackageGlobalData->deallocate = 0;
   }
   else
   {
      if ( ( RxPackageGlobalData = ( RxPackageGlobalDataDef *)malloc( sizeof( RxPackageGlobalDataDef ) ) ) == NULL )
      {
         fprintf( stderr, "Unable to allocate memory for Global Data\n" );
         *rc = 1;
         return NULL;
      }
      memset( RxPackageGlobalData, 0, sizeof( RxPackageGlobalDataDef ) );
      (void)RxSetTraceFile( RxPackageGlobalData, "stderr" );
      (void)RxSetConstantPrefix( RxPackageGlobalData, constantPrefix );
      RxPackageGlobalData->deallocate = 1;
   }
#else
   if ( RxPackageGlobalData )
   {
      (void)RxSetTraceFile( RxPackageGlobalData, "stderr" );
      (void)RxSetConstantPrefix( RxPackageGlobalData, constantPrefix );
      RxPackageGlobalData->deallocate = 1;
   }
#endif

   RxPackageGlobalData->terminated = 0;

   if ( (env = getenv(RXPACKAGE_DEBUG_VAR)) != NULL )
   {
      RxPackageGlobalData->RxRunFlags |= atoi(env);
   }
   /*
    * Call any package-specific startup code here
    */
   if ( ptr )
   {
      *rc = (*ptr)( RxPackageGlobalData );
   }
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): End of InitRxPackage with rc = %d\n",__FILE__,__LINE__,RxPackGetThreadID(),*rc);)
   return RxPackageGlobalData;
}


/*-----------------------------------------------------------------------------
 * This function is called to terminate all activity with the package.
 *----------------------------------------------------------------------------*/
int TermRxPackage( RxPackageGlobalDataDef *RxPackageGlobalData, PackageTerminator *ptr, RexxFunction *RxPackageFunctions, char *progname, int deregfunc )
{
   int rc=0;

   if ( !RxPackageGlobalData
   ||   RxPackageGlobalData->terminated == 1 )
   {
      DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): TermRxPackage already called. RxPackageGlobalData is %lx\n",__FILE__,__LINE__,RxPackGetThreadID(),(long)RxPackageGlobalData);)
      return 0;
   }

   InternalTrace( RxPackageGlobalData, "TermRxPackage", "\"%s\",%d", progname, deregfunc );

   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): Start of TermRxPackage RxPackageGlobalData is %lx\n",__FILE__,__LINE__,RxPackGetThreadID(),(long)RxPackageGlobalData);)
   /*
    * De-register all package functions only if requested
    * BE CAREFUL!!
    * DO NOT DO THIS FOR DYNAMIC LIBRARY
    * AS IT WILL DEREGISTER FOR ALL PROCESSES
    * NOT JUST THE CURRENT ONE.
    */
   if (deregfunc)
   {
      if ( ( rc = DeregisterRxFunctions( RxPackageGlobalData, RxPackageFunctions, 0 ) ) != 0 )
         return (int)FunctionEpilogue( RxPackageGlobalData, "TermRxPackage", (long)rc );
   }
   /*
    * Call any package-specific termination code here
    */
   if ( ptr )
   {
      DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In TermRxPackage: Calling package terminator function\n",__FILE__,__LINE__,RxPackGetThreadID());)
      if ( ( rc = (*ptr)( RxPackageGlobalData ) ) != 0 )
         return (int)FunctionEpilogue( RxPackageGlobalData, "TermRxPackage", (long)rc );
   }
#if defined(USE_REXX6000)
   rc = RexxDeregisterSubcom( RDS_ARG0_TYPE)RXPACKAGENAME );
#else
   rc = RexxDeregisterSubcom( (RDS_ARG0_TYPE)RXPACKAGENAME,
                              (RDS_ARG1_TYPE)NULL );
#endif

#if !defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
   RexxDeregisterExit( ( RDE_ARG0_TYPE )RXPACKAGENAME,
                       ( RDE_ARG1_TYPE )NULL );
#endif
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In TermRxPackage: RxPackageGlobalData is %lx\n",__FILE__,__LINE__,RxPackGetThreadID(),(long)RxPackageGlobalData);)

   (void)FunctionEpilogue( RxPackageGlobalData, "TermRxPackage", (long)0 );

   if ( RxPackageGlobalData )
   {
      DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In TermRxPackage: deallocate is %d, RxTraceFilePointer is %lx stdout is %lx stderr is %lx\n",__FILE__,__LINE__,RxPackGetThreadID(),RxPackageGlobalData->deallocate,(long)RxPackageGlobalData->RxTraceFilePointer,(long)stdout,(long)stderr);)
      if ( RxPackageGlobalData->RxTraceFilePointer
      &&   RxPackageGlobalData->RxTraceFilePointer != stdout
      &&   RxPackageGlobalData->RxTraceFilePointer != stderr )
      {
         DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): In TermRxPackage: going to close filepointer: deallocate is %d, RxTraceFilePointer is %lx stdout is %lx stderr is %lx\n",__FILE__,__LINE__,RxPackGetThreadID(),RxPackageGlobalData->deallocate,(long)RxPackageGlobalData->RxTraceFilePointer,(long)stdout,(long)stderr);)
         fclose( RxPackageGlobalData->RxTraceFilePointer );
         RxPackageGlobalData->RxTraceFilePointer = NULL;
      }
      RxPackageGlobalData->terminated = 1;
   }

#if 0
/* not sure if we should dealloc here or in TERM_RXPACKAGE() ???? */
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->deallocate )
   {
      FREE_TSD( RxPackageGlobalData, RxPackageGlobalData );
      *RxPackageGlobalData = NULL;
   }
#endif
   DEBUGDUMP(fprintf(stderr,"%s-%d Thread(%ld): End of TermRxPackage with rc = 0\n",__FILE__,__LINE__,RxPackGetThreadID());)
   return 0;
}

/*-----------------------------------------------------------------------------
 * This function sets the name of thepackage trace file
 *----------------------------------------------------------------------------*/
int RxSetTraceFile( RxPackageGlobalDataDef *RxPackageGlobalData, char *name )
{
   int rc = 0;

   InternalTrace( RxPackageGlobalData, "RxSetTraceFile", "%s", name );

   /* close the current file if open and not stdout or stderr */
   if ( RxPackageGlobalData->RxTraceFilePointer != NULL
   &&   RxPackageGlobalData->RxTraceFilePointer != stdout
   &&   RxPackageGlobalData->RxTraceFilePointer != stderr )
      fclose( RxPackageGlobalData->RxTraceFilePointer );
   if ( strcmp( "stdout", name ) == 0 )
   {
      RxPackageGlobalData->RxTraceFilePointer = stdout;
      strcpy( RxPackageGlobalData->RxTraceFileName, "stdout" );
   }
   else
   {
      if ( strcmp( "stderr", name ) == 0 )
      {
         RxPackageGlobalData->RxTraceFilePointer = stderr;
         strcpy( RxPackageGlobalData->RxTraceFileName, "stderr" );
      }
      else
      {
         RxPackageGlobalData->RxTraceFilePointer = fopen( name, "a" );
         if ( RxPackageGlobalData->RxTraceFilePointer == NULL )
         {
            (void)fprintf( stderr, "ERROR: Could not open trace file: %s for writing. Defaulting to stderr!\n", name );
            RxPackageGlobalData->RxTraceFilePointer = stderr;
            strcpy( RxPackageGlobalData->RxTraceFileName, "stderr" );
            rc = 1;
         }
         else
            strcpy( RxPackageGlobalData->RxTraceFileName, name );
      }
   }

   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function obtains the name of thepackage trace file and returns it
 *----------------------------------------------------------------------------*/
char *RxGetTraceFile( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   InternalTrace( RxPackageGlobalData, "RxGetTraceFile", NULL );
   return ( RxPackageGlobalData->RxTraceFileName );
}

/*-----------------------------------------------------------------------------
 * This function sets the constants prefix for the package
 *----------------------------------------------------------------------------*/
int RxSetConstantPrefix( RxPackageGlobalDataDef *RxPackageGlobalData, char *name )
{
   InternalTrace( RxPackageGlobalData, "RxSetConstantPrefix", "%s", name );

   if ( ( strlen( name ) + 1 ) > RX_CONSTANT_PREFIX_MAX )
   {
      (void)fprintf( stderr, "ERROR: Constant prefix is too long. It must be <= %d\n", RX_CONSTANT_PREFIX_MAX );
      return( 1 );
   }
   strcpy( RxPackageGlobalData->PreviousConstantPrefix, RxPackageGlobalData->ConstantPrefix );
   strcpy( RxPackageGlobalData->ConstantPrefix, name );
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function obtains the constants prefix for the package and returns it
 *----------------------------------------------------------------------------*/
char *RxGetConstantPrefix( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   InternalTrace( RxPackageGlobalData, "RxGetConstantPrefix", NULL );
   return ( RxPackageGlobalData->ConstantPrefix );
}

/*-----------------------------------------------------------------------------
 * This function sets the run-time flags for the package
 *----------------------------------------------------------------------------*/
void RxSetRunFlags( RxPackageGlobalDataDef *RxPackageGlobalData, int flags )
{
   InternalTrace( RxPackageGlobalData, "RxSetRunFlags", "%d", flags );
   RxPackageGlobalData->RxRunFlags = flags;
}

/*-----------------------------------------------------------------------------
 * This function obtains the run-time flags for the package and returns them
 *----------------------------------------------------------------------------*/
int RxGetRunFlags( RxPackageGlobalDataDef *RxPackageGlobalData )
{
   InternalTrace( RxPackageGlobalData, "RxGetRunFlags", NULL );
   return ( RxPackageGlobalData->RxRunFlags );
}

/*-----------------------------------------------------------------------------
 * This function returns the contents of retstr to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturn( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr )
{
   InternalTrace( RxPackageGlobalData, "RxReturn", "%x,%s", retstr, retstr->strptr );

   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
          "++ Exit %s with value \"%s\" Length: %ld\n",
          RxPackageGlobalData->FName,
          retstr->strptr,
          retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function returns the contents of retstr to the Rexx interpreter
 * but displays the traced returned data in hex
 *----------------------------------------------------------------------------*/
int RxReturnHex( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr )
{
   char buf[250];

   if ( RxPackageGlobalData != NULL
   &&   ( RxPackageGlobalData->RxRunFlags & MODE_INTERNAL
     ||   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE ) )
   {
      int i, len;
      char tmpbuf[3];
      /*
       * Only generate the hex value if we are going to use it!
       */
      memset( buf, 0, sizeof( buf ) );
      len = min( 100, retstr->strlength );
      for ( i = 0; i < len; i++ )
      {
         sprintf( tmpbuf, "%2.2X", (unsigned char)retstr->strptr[i] );
         strcat( buf, tmpbuf );
      }
      if ( retstr->strlength > 100 )
      {
         buf[200] = buf[201] = buf[202] = '.';
      }
   }
   InternalTrace( RxPackageGlobalData, "RxReturnHex", "%x,%s", retstr, buf );

   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
          "++ Exit %s with value \"%s\" Length: %ld\n",
          RxPackageGlobalData->FName,
          buf,
          retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a signed integer and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnNumber( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, long num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnNumber", "%x,%ld", retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%ld", num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%ld\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes an unsigned integer and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnUnsignedNumber( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, ULONG num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnUnsignedNumber", "%x,%lu", retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%lu", num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%lu\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a signed long long and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnLongLong( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, rx_long_long num )
{
   char tmp_format[50];

   sprintf( tmp_format, "%%x,%s", RX_LL_FORMAT );
   InternalTrace( RxPackageGlobalData, "RxReturnLongLong", tmp_format, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, RX_LL_FORMAT, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      sprintf( tmp_format, "++ Exit %%s with value \"%s\" Length: %%ld\n", RX_LL_FORMAT );
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         tmp_format,
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a int8_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnINT8( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, int8_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT8", "%x,%"PRId8, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRId8, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRId8"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a uint8_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnUINT8( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, uint8_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT8", "%x,%"PRIu8, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRIu8, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRIu8"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a int16_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnINT16( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, int16_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT16", "%x,%"PRId16, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRId16, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRId16"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a uint16_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnUINT16( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, uint16_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT16", "%x,%"PRIu16, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRIu16, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRIu16"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a int32_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnINT32( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, int32_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT32", "%x,%"PRId32, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRId32, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRId32"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a uint32_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnUINT32( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, uint32_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT32", "%x,%"PRIu32, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRIu32, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRIu32"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a int64_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnINT64( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, int64_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT64", "%x,%"PRId64, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRId64, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRId64"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a uint64_t and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnUINT64( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, uint64_t num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnINT64", "%x,%"PRIu64, retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%"PRIu64, num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%"PRIu64"\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a double number and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnDouble( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, double num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnDouble", "%x,%f", retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%f", num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%f\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a float number and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnFloat( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, float num )
{
   InternalTrace( RxPackageGlobalData, "RxReturnFloat", "%x,%f", retstr, num );

   retstr->strlength = sprintf( (char *)retstr->strptr, "%f", num );
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%f\" Length: %ld\n",
         RxPackageGlobalData->FName,
         num,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}


/*-----------------------------------------------------------------------------
 * This function returns a numeric representation of a pointer.
 * If NULL, returns an empty string.
 *----------------------------------------------------------------------------*/
int RxReturnPointer( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, void *ptr )
{
   InternalTrace( RxPackageGlobalData, "RxReturnPointer", "%x,%x", retstr, ptr );

   if ( ptr )
   {
      retstr->strlength = sprintf( (char *)retstr->strptr, "%ld", (long)ptr );
   }
   else
   {
      strcpy( (char *)retstr->strptr, "" );
      retstr->strlength = 0;
   }
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with value \"%s\" Length: %ld\n",
         RxPackageGlobalData->FName,
         retstr->strptr,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   return( 0 );
}

/*-----------------------------------------------------------------------------
 * This function takes a string and returns it to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnString( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, char *str )
{
   return( RxReturnStringAndFree( RxPackageGlobalData, retstr, str, 0 ) );
}


/*-----------------------------------------------------------------------------
 * This function takes a copy of a string, frees it, and returns the copy to the Rexx interpreter
 *----------------------------------------------------------------------------*/
int RxReturnStringAndFree( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, char *str, int freeit )
{
   long len = 0;

   if ( str )
      len = (long)strlen( str );
   InternalTrace( RxPackageGlobalData, "RxReturnStringAndFree", "%x,\"%s\" Length: %d Free: %d", retstr, str, len, freeit );
   return( RxReturnDataAndFree( RxPackageGlobalData, retstr, (void *)str, len, freeit ) );
}

/*-----------------------------------------------------------------------------
 * This function takes a copy of a void, frees it (if requested), and returns
 * the copy to the Rexx interpreter.
 * The data to be freed MUST have been allocated with RexxAllocateMemory()
 *----------------------------------------------------------------------------*/
int RxReturnDataAndFree( RxPackageGlobalDataDef *RxPackageGlobalData, RXSTRING *retstr, void *str, long len, int freeit )
{
   char *ret = NULL;

   InternalTrace( RxPackageGlobalData, "RxReturnDataAndFree", "%x,\"%s\" Length: %d Free: %d", retstr, str, len, freeit );

   if ( len > RXAUTOBUFLEN )
   {
#if defined(REXXALLOCATEMEMORY)
      ret = (char *)RexxAllocateMemory( len + 1 );
#elif defined(WIN32) && ( defined(USE_WINREXX) || defined(USE_QUERCUS) || defined(USE_OREXX) || defined(USE_OOREXX) )
      ret = ( char * )GlobalLock( GlobalAlloc ( GMEM_FIXED, len + 1 ) );
#elif defined(WIN64) && ( defined(USE_OREXX) || defined(USE_OOREXX) )
      ret = ( char * )GlobalLock( GlobalAlloc ( GMEM_FIXED, len + 1 ) );
#elif defined(USE_OS2REXX)
      if ( ( BOOL )DosAllocMem( (void **)&ret, len + 1, fPERM|PAG_COMMIT ) )
         ret = ( char * )NULL;
#else
      ret = ( char * )MALLOC_TSD( RxPackageGlobalData, len + 1 );
#endif
      if ( ret == ( char * )NULL )
      {
         (void)fprintf( RxPackageGlobalData->RxTraceFilePointer, "Unable to allocate %ld bytes for return data\n", len );
         return( 1 );
      }
      retstr->strptr = (RXSTRING_STRPTR_TYPE)ret;
   }
   memcpy( retstr->strptr, str, len );
   retstr->strlength = len;
   if ( RxPackageGlobalData
   &&   RxPackageGlobalData->RxRunFlags & MODE_VERBOSE )
   {
      (void)fprintf( RxPackageGlobalData->RxTraceFilePointer,
         "++ Exit %s with length: %ld: Data: \n",
         RxPackageGlobalData->FName,
         retstr->strlength );
      fflush( RxPackageGlobalData->RxTraceFilePointer );
   }
   if ( freeit )
#if defined(REXXALLOCATEMEMORY)
      RexxFreeMemory( str );
#elif defined(WIN32) && ( defined(USE_WINREXX) || defined(USE_QUERCUS) || defined(USE_OREXX) || defined(USE_OOREXX) )
      GlobalFree( str );
#elif defined(WIN64) && ( defined(USE_OREXX) || defined(USE_OOREXX) )
      GlobalFree( str );
#elif defined(USE_OS2REXX)
      DosFreeMem( str );
#else
      FREE_TSD( RxPackageGlobalData, str );
#endif
   return( 0 );
}

#if !defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
static REH_RETURN RxExitHandlerForSayTraceRedirection

#if defined(HAVE_PROTO)
   ( REH_ARG0_TYPE ExitNumber, REH_ARG1_TYPE Subfunction, REH_ARG2_TYPE ParmBlock )
#else
   ( ExitNumber, Subfunction, ParmBlock )
   REH_ARG0_TYPE ExitNumber;    /* code defining the exit function    */
   REH_ARG1_TYPE Subfunction;   /* code defining the exit subfunction */
   REH_ARG2_TYPE ParmBlock;     /* function dependent control block   */
#endif
{
   long i = 0;
   int rc = 0;

   switch( Subfunction )
   {
      case RXSIOSAY:
      {
         RXSIOSAY_PARM *say_parm = (RXSIOSAY_PARM *)ParmBlock;
         if ( say_parm->rxsio_string.strptr != NULL )
         {
            for( i = 0; i < (long)say_parm->rxsio_string.strlength; i++ )
               fputc( ( char )say_parm->rxsio_string.strptr[i], stdout );
         }
         fputc( '\n', stdout );
         rc = RXEXIT_HANDLED;
         break;
      }
      case RXSIOTRC:
      {
         RXSIOTRC_PARM *trc_parm = (RXSIOTRC_PARM *)ParmBlock;
         if ( trc_parm->rxsio_string.strptr != NULL )
         {
            for( i = 0; i < (long)trc_parm->rxsio_string.strlength; i++ )
               fputc( ( char )trc_parm->rxsio_string.strptr[i], stderr );
         }
         fputc( '\n', stderr );
         rc = RXEXIT_HANDLED;
         break;
      }
      case RXSIOTRD:
      {
         RXSIOTRD_PARM *trd_parm = (RXSIOTRD_PARM *)ParmBlock;
         int i = 0, ch = 0;
         do
         {
            if ( i < 256 )
               trd_parm->rxsiotrd_retc.strptr[i++] = ch = getc( stdout ) ;
         } while( ch != '\012' && (ch != EOF ) ) ;
         trd_parm->rxsiotrd_retc.strlength = i - 1;
         rc = RXEXIT_HANDLED;
         break;
      }
      case RXSIODTR:
      {
         RXSIODTR_PARM *dtr_parm = (RXSIODTR_PARM *)ParmBlock;
         int i = 0, ch = 0;
         do
         {
            if ( i < 256 )
               dtr_parm->rxsiodtr_retc.strptr[i++] = ch = getc( stdout ) ;
         } while( ch != '\012' && (ch != EOF ) ) ;
         dtr_parm->rxsiodtr_retc.strlength = i - 1;
         rc = RXEXIT_HANDLED;
         break;
      }
      default:
         rc = RXEXIT_NOT_HANDLED;
         break;
   }
   return rc;
}
#endif

/*-----------------------------------------------------------------------------
 * This is the default subcommand handler for the package. It passes all
 * subcommands to the shell for execution.
 *----------------------------------------------------------------------------*/
static RSH_RETURN RxSubcomHandler

#if defined(HAVE_PROTO)
   (
   RSH_ARG0_TYPE Command,    /* Command string passed from the caller    */
   RSH_ARG1_TYPE Flags,      /* pointer to short for return of flags     */
   RSH_ARG2_TYPE Retstr)     /* pointer to RXSTRING for RC return        */
#else
   ( Command, Flags, Retstr )
   RSH_ARG0_TYPE Command;    /* Command string passed from the caller    */
   RSH_ARG1_TYPE Flags;      /* pointer to short for return of flags     */
   RSH_ARG2_TYPE Retstr;     /* pointer to RXSTRING for RC return        */
#endif
{
   RSH_RETURN_TYPE rcode=0;
   int rc=0;
   char *buf;
   RxPackageGlobalDataDef *RxPackageGlobalData = __rxpack_get_tsd();

   buf = MALLOC_TSD( RxPackageGlobalData, Command->strlength + 1 );
   if ( buf == NULL )
   {
      *Flags = RXSUBCOM_ERROR;             /* raise an error condition   */
      sprintf(Retstr->strptr, "%d", RXSUBCOM_NOEMEM);   /* format return code string  */
                                           /* and set the correct length */
      Retstr->strlength = (ULONG)strlen(Retstr->strptr);
   }
   else
   {
      memcpy( buf, Command->strptr, Command->strlength );
      buf[Command->strlength] = '\0';
      rc = system( buf );
      FREE_TSD( RxPackageGlobalData, buf );
      if (rc < 0)
         *Flags = RXSUBCOM_ERROR;             /* raise an error condition   */
      else
         *Flags = RXSUBCOM_OK;                /* not found is not an error  */

      sprintf(Retstr->strptr, "%d", rc); /* format return code string  */
                                              /* and set the correct length */
      Retstr->strlength = (ULONG)strlen(Retstr->strptr);
   }
   return rcode;                              /* processing completed       */
}

#if 0
/*
 * Both WinRexx and Personal Rexx load and unload this DLL before and
 * after each call to an external function. If this DLL is not resident
 * a new copy of the DLL is loaded together with its data.  What happens
 * then is any data set by one call to an external function is lost
 * and there is no persistence of data.  This code ensures that when the
 * DLL starts, it stays resident, and when the DLL finishes it is
 * unloaded.
 */
#if defined(DYNAMIC_LIBRARY) && (defined(USE_WINREXX) || defined(USE_QUERCUS))
BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID pReserved)
{
   switch( dwReason)
   {
      case DLL_PROCESS_ATTACH:
         LoadLibrary( RXPACKAGENAME );
         break;
      case DLL_PROCESS_DETACH:
         FreeLibrary( GetModuleHandle( RXPACKAGENAME ) );
         break;
      case DLL_THREAD_ATTACH:
         break;
      case DLL_THREAD_DETACH:
         break;
   }
   return(TRUE);
}
#endif
#endif

/***********************************************************************/
/* Determine the Rexx interpreter being used; linked and runtime       */
/* borrowed from THE                                                   */
/***********************************************************************/
static char version_buffer[RXAUTOBUFLEN+1];
static REH_RETURN RexxInterpreterVersionExit
(
   REH_ARG0_TYPE ExitNumber,    /* code defining the exit function    */
   REH_ARG1_TYPE Subfunction,   /* code defining the exit subfunction */
   REH_ARG2_TYPE ParmBlock      /* function dependent control block   */
)
/***********************************************************************/
{
   SHVBLOCK shv;
   int rc=0;
   char *name="VERSION";
   char mybuf[RXAUTOBUFLEN+1];

   strcpy( mybuf, "" );
   shv.shvnext = NULL;
   shv.shvcode = RXSHV_PRIV;
   shv.shvname.strptr = name;
   shv.shvnamelen = shv.shvname.strlength = 7;
   shv.shvvalue.strptr = mybuf;
   shv.shvvalue.strlength = sizeof( mybuf );
   rc = RexxVariablePool( &shv );            /* get the next variable */
   if ( rc == 0 )
   {
      mybuf[shv.shvvaluelen] = '\0';
      strcpy( version_buffer, mybuf );
   }
   return( RXEXIT_HANDLED );
}

/***********************************************************************/
/* buf argument must be large enough for the return value: 1024 is good*/
/***********************************************************************/
char *RxGetRexxInterpreterVersion( char *buf )
/***********************************************************************/
{
#define GRIVEXIT_NAME "VerExit"
   RXSYSEXIT Exits[2] ;
   RXSTRING Instore[2] ;
   RS_ARG7_TYPE rcode; /* even though we don't use it ooRexx 4.2 (64bit) requires a variable to pass this data back */
   int rc ;

   RexxRegisterExitExe((RREE_ARG0_TYPE)GRIVEXIT_NAME,
                       (RREE_ARG1_TYPE)RexxInterpreterVersionExit,
                       (RREE_ARG2_TYPE)NULL);

   Exits[0].sysexit_name = GRIVEXIT_NAME ;
   Exits[0].sysexit_code = RXTER ;
   Exits[1].sysexit_code = RXENDLST ;

   Instore[0].strptr = "/**/;nop;" ;
   Instore[0].strlength = (ULONG)strlen( Instore[0].strptr ) ;
   Instore[1].strptr = NULL ;
   Instore[1].strlength = 0 ;

   rc = RexxStart((RS_ARG0_TYPE)0,
                (RS_ARG1_TYPE)NULL,
                (RS_ARG2_TYPE)"GetVersion",
                (RS_ARG3_TYPE)Instore,
                (RS_ARG4_TYPE)"GETVERSION",
                (RS_ARG5_TYPE)RXCOMMAND,
                (RS_ARG6_TYPE)Exits,
                (RS_ARG7_TYPE)&rcode,
                (RS_ARG8_TYPE)NULL);
   if (rc == 0)
      strcpy( buf, version_buffer );
   else
      strcpy( buf, "Unable to obtain Rexx interpreter version" );
#ifdef USE_REXXTRANS
   strcat( buf, " (via Rexx/Trans)" );
#endif
   RexxDeregisterExit( GRIVEXIT_NAME, NULL );
   return(buf);
}
