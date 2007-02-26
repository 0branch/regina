#ifndef lint
static char *RCSid = "$Id: variable.c,v 1.22 2003/04/01 10:26:06 mark Exp $";
#endif

/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992  Anders Christensen <anders@pvv.unit.no>
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
 * Concept: Each REXX procedure (the top - or main - by default) has
 *  an array of hash-pointers. Routines that don't keep local
 *  variables (i.e hasn't seen a PROCEDURE) are using the hashpointers
 *  of the routine above them. The size of the array is HASHTABLENGTH.
 *
 * Each cell in this array is a pointer to a single-linked list of
 *  boxes. In common for all these boxes is that their name returns
 *  the same value when given as parameter to the hashfunc() function.
 *
 * Each of these boxes contains five variables: name, value, index,
 *  realbox and next. 'next' points to next box in the list.
 *
 * 'name' is the name of the variable, and 'value' is the value it
 *  contains. However, if 'realbox' is set, it points to another
 *  box which contains the real value of the variable. This mechanism
 *  gives support for EXPOSE'ing variables in PROCEDUREs.
 *
 * The 'index' is a pointer to another hashtable, and gives support
 *  for compound variables. If a variable is compound, its 'index' is
 *  set to point at the hashtable, each entry in this table do also
 *  point at the start of a single linked list of variable boxes, but
 *  these boxes has the 'after-the-period' part of the compound name
 *  as 'name'. The 'realbox', but not the 'index' may be set in these
 *  boxes.
 *
 * A variable is set when it exists in the datastructures, and the
 *  relevant 'value' pointer is non-NULL. When dropping a variable
 *  that is EXPOSE'ed, the 'value' is set to NULL.
 *
 * The 'test' and the 'test.' variables have two different
 *  variableboxes, and 'index' is only set in the box for 'test.'. A
 *  'realbox' existing for 'test' makes it exposed. A 'realbox'
 *  'test.' make the whole "array" exposed.
 *
 * A 'value' existing for 'test.' denotes the default value.
 *
 * Yet to do:
 *
 *    o the datastructure for the variables should be local, not global
 *    o must implement the code for dropping variables.
 *    o dont always handle ptr->value==NULL correct
 *    o tracing is incorrect
 */
/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */

#include "rexx.h"
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>   /* f*ck sun, they can't write a proper assert!!! */

typedef struct {
   void **Elems;
   unsigned size;
} Pool;

typedef struct { /* var_tsd: static variables of this module (thread-safe) */
#ifdef TRACEMEM
   variableptr   first_invalid;
#endif
#ifdef DEBUG
   int           DoDebug;
   char          PoolNameBuf[20];
   Pool          NamePool;
   Pool          ValuePool;
   Pool          NumPool;
   Pool          VarPool;
#endif
   int           foundflag;
   variableptr   thespot;
   long          current_valid;
   /* We CAN'T increment current_valid on each new procedure (which results
    * into create_new_varpool) and decrement it in procedure exit (which
    * executes kill_variables). Imagine the following:
    * >call proc1
    * >exit 0
    * >proc1: procedure
    * > locvar = 1
    * > call proc2
    * > call proc1 (not endlessly, but at least one time)
    * > return
    * >proc2: procedure expose locvar
    * > return
    * In the first call to proc2 current_valid will be 3, proc2 returns, proc1
    * will be executed at least once more and current_valid is 3, too. This is
    * OK if and only if the variable accessment in proc1 and proc2 are distinct
    * by a procedure counter (each procedure has its own number). This is NOT
    * realized. Thus, we increment a separate counter (next_current_valid) each
    * time a new procedure is called and assign the value to current_valid.
    * On procedure return we set back current_valid to the current_value of
    * this procedure instance which may be, but don't MUST be, current_level-1.
    * Of course, this is bogus! The next_current_valid counter may wrap around
    * and we run into trouble once more. We can reset next_current_valid to
    * 2 (initial current_valid+1) savely iff current_valid==1. This prevents
    * some problems with multiple calls to Rexx when started but don't help in
    * one execution run. Since Regina is dog slow this will PROBABLY never
    * happen.
    * For a correct way of operation see many compiler building books of
    * languages with call by name.
    * Former releases uses stupid generation counter mechanisms.
    * FGC 27.09.98 (09/27/98)
    */
   long          next_current_valid;
   int           subst;
   int           hashval;
   int           ignore_novalue;
   int           notrace;
   streng *      tmpindex;
   streng *      ovalue;
   streng *      xvalue;
   num_descr *   odescr;
   variableptr   pstem;
   variableptr   ptail;
   variableptr   rstem;
   variableptr   rtail;
   int           stemidx;
   int           tailidx;
   variableptr * var_table;
} var_tsd_t;

#define GET_REAL_BOX(ptr) {for(;(ptr->realbox);ptr=ptr->realbox);}
#define REPLACE_VALUE(val,p) {if(p->value) \
     Free_stringTSD(p->value);p->value=val;p->guard=0;\
     p->flag=(val)?VFLAG_STR:VFLAG_NONE;}

#define REPLACE_NUMBER(val,p) {if(p->num) \
     {FreeTSD(p->num->num);FreeTSD(p->num);};p->num=val;p->guard=0;\
     p->flag=(val)?VFLAG_NUM:VFLAG_NONE;}

#ifdef DEBUG
static void regina_dprintf(const tsd_t *TSD, const char *fmt,...)
{
   va_list marker;
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug)
      return;
   va_start(marker,fmt);
   vfprintf(stderr,fmt,marker);
   fflush(stderr);
   va_end(marker);
}
#  define DPRINT(x) regina_dprintf x
#  define DSTART regina_dprintf(TSD,"%2u %4d ",TSD->thread_id,__LINE__)
#  define DEND regina_dprintf(TSD,"\n")

static const volatile char *PoolName(const tsd_t *TSD,Pool *pool,const void *elem)
{
   unsigned i;
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug) /* PoolName's return value isn't really used if */
      return(NULL);  /* debugging is turned off. */
   if (pool == &vt->NamePool)
      strcpy(vt->PoolNameBuf,"NAME");
   else if (pool == &vt->ValuePool)
      strcpy(vt->PoolNameBuf,"VAL");
   else if (pool == &vt->NumPool)
      strcpy(vt->PoolNameBuf,"NUM");
   else if (pool == &vt->VarPool)
      strcpy(vt->PoolNameBuf,"VAR");
   else
      return("????");
   for (i = 0;i < pool->size;i++)
      if (pool->Elems[i] == elem)
         break;
   sprintf(vt->PoolNameBuf+strlen(vt->PoolNameBuf),"%u",i+1);
   if (i >= pool->size)
   {
         pool->size++;
         if ((pool->Elems = realloc(pool->Elems,pool->size*sizeof(void *))) ==
                                                                          NULL)
         {
            if (TSD->in_protected)
            {
               jmp_buf h;

               memcpy(h,TSD->protect_return,sizeof(jmp_buf));
               /* cheat about the const, we go away anyway :-) */
               *((int*) &TSD->delayed_error_type) = PROTECTED_DelayedExit;
               *((int*) &TSD->expected_exit_error) = 123;
               longjmp( h, 1 ) ;
            }
            TSD->MTExit(123);
         }
   }
   pool->Elems[i] = (void *) elem;
   return(vt->PoolNameBuf);
}

static void DNAME(const tsd_t *TSD,const char *name,const streng* n)
{
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug)
      return;
   if (name != NULL)
      regina_dprintf(TSD,"%s=",name);
   if (n == NULL)
   {
      regina_dprintf(TSD,"NULL");
      return;
   }
   regina_dprintf(TSD,"\"%*.*s\"%s",Str_len(n),Str_len(n),n->value,PoolName(TSD,&vt->NamePool,n));
}

static void DVALUE(const tsd_t *TSD,const char *name,const streng* v)
{
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug)
      return;
   if (name != NULL)
      regina_dprintf(TSD,"%s=",name);
   if (v == NULL)
   {
      regina_dprintf(TSD,"NULL");
      return;
   }
   regina_dprintf(TSD,"\"%*.*s\"%s",Str_len(v),Str_len(v),v->value,PoolName(TSD,&vt->ValuePool,v));
}

static void DNUM(const tsd_t *TSD,const char *name,const num_descr* n)
{
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug)
      return;
   if (name != NULL)
      regina_dprintf(TSD,"%s=",name);
   if (n == NULL)
   {
      regina_dprintf(TSD,"NULL");
      return;
   }
   regina_dprintf(TSD,"\"%*.*s\"%s",n->size,n->size,n->num,PoolName(TSD,&vt->NumPool,n));
}

static int Dfindlevel(const tsd_t *TSD,cvariableptr v)
{
   proclevel curr ;
   int i,lvl = 0;

   curr = TSD->currlevel ;

   while (curr)
   {
      if (curr->vars)
      {
         for (i=0;i<HASHTABLENGTH;i++)
         {
            if (curr->vars[i] == v)
               goto found;
         }
      }
      curr = curr->prev;
      lvl++;
   }
   return(-1);
found:
   while (curr->prev)
   {
      curr = curr->prev;
      lvl++;
   }
   return(lvl);
}

static void DVAR(const tsd_t *TSD,const char *name,cvariableptr v)
{
   var_tsd_t *vt = TSD->var_tsd;

   if (!vt->DoDebug)
      return;
   if (name != NULL)
      regina_dprintf(TSD,"%s=",name);
   if (v == NULL)
   {
      regina_dprintf(TSD,"NULL");
      return;
   }

   regina_dprintf(TSD,"%s,l=%d(",PoolName(TSD,&vt->VarPool,v),Dfindlevel(TSD,v));
   if (v->valid == 0)
      regina_dprintf(TSD,"?");
   else
   {
      DNAME(TSD,NULL,v->name);
      regina_dprintf(TSD,",");
      DVALUE(TSD,NULL,v->value);
      regina_dprintf(TSD,"=");
      DNUM(TSD,NULL,v->num);
   }
   regina_dprintf(TSD,",hwired=%ld,valid=%ld",v->hwired,v->valid);
   if (v->realbox)
   {
      regina_dprintf(TSD,"->");
      DVAR(TSD,NULL,v->realbox);
   }
   regina_dprintf(TSD,")");
}
#  define DPRINTF(x) DSTART;regina_dprintf x;DEND
#  define DARG(x,y) x
#else /* !define(DEBUG) */
#  define DPRINT(x)
#  define DSTART
#  define DEND
#  define DNAME(t,n,v)
#  define DVALUE(t,n,v)
#  define DNUM(t,n,v)
#  define DVAR(t,n,v)
#  define DPRINTF(x)
#  define DARG(x,y) y
#endif /* DEBUG */

/*
 * Allocates and initializes a hashtable for the variables. Can be used
 * both for the main variable hash table, or for an compound variable.
 */
static variableptr *make_hash_table( const tsd_t *TSD )
{
   variableptr *retval ;

   retval = MallocTSD( (HASHTABLENGTH+1)*sizeof(variableptr) ) ;
   /* Last element needed to save current_valid */
   memset( retval, 0, (HASHTABLENGTH+1)*sizeof(variableptr) );

   return retval ;
}

/*
 * known_reserved_variable returns 0 if the arguments don't be a known
 * reserved variable as described in ANSI 6.2.3.1. The value is a
 * POOL0_??? value otherwise.
 *
 * The argument don't has to be uppercased but must not contain whitespaces or
 * garbage.
 */
int known_reserved_variable( const char *name, unsigned length )
{
   unsigned i;
   char upper[20]; /* good maximum for predefined variable names */

   /*
    * a fast breakout switch
    */
   if ( ( length < 3 /* ".RC" */)
     || ( length > 7 /* ".RESULT" */)
     || ( *name != '.' ) )
      return POOL0_NOT_RESERVED;
   name++;
   length--;

   for( i = 0; i < length; i++ )
   {
      upper[i] = (char)toupper( name[i] );
   }

   /*
    * some magic stuff to reduce further errors
    */
#define RET_IF(s)  if ( ( length == sizeof( #s ) - 1 )                   \
                     && ( memcmp( upper, #s, sizeof( #s ) - 1 ) == 0 ) ) \
                      return POOL0_##s
   RET_IF( RC );
   RET_IF( RESULT );
   RET_IF( SIGL );
   RET_IF( RS );
   RET_IF( MN );
#undef REF_IF

   return POOL0_NOT_RESERVED;
}

void detach( const tsd_t *TSD, variableptr ptr )
{
   TSD = TSD; /* keep compiler happy */
   assert( ptr->hwired > 0 ) ;
/*
#ifdef TRACEMEM
   if (ptr->valid)
   {
      if (ptr->value)
         Free_stringTSD( ptr->value ) ;
      if (ptr->name)
         Free_stringTSD( ptr->name ) ;
      if (ptr->num)
      {
         FreeTSD( ptr->num->num ) ;
         FreeTSD( ptr->num ) ;
      }
      ptr->value = ptr->name = ptr->num = NULL ;
      ptr->flag = VFLAG_NONE ;
      ptr->valid = 0 ;
   }

   if (--ptr->hwired == 0)
   {
      if (ptr->prev)
         ptr->prev->next = ptr->next ;
      if (ptr->next)
         ptr->next->prev = ptr->prev ;
      else
         vt->first_invalid = ptr->prev ;

      FreeTSD( ptr ) ;
   }
#endif
 */

   ptr->hwired-- ;
   DSTART;DPRINT((TSD,"detach:            "));DVAR(TSD,NULL,ptr);DEND;
}

#ifdef TRACEMEM
void markvariables( const tsd_t *TSD, cproclevel procptr )
{
   variableptr vvptr=NULL, vptr=NULL ;
   paramboxptr pptr=NULL ;
   int i=0, j=0 ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   for (;procptr;procptr=procptr->next)
   {
      if (procptr->environment)
         markmemory( procptr->environment, TRC_VARBOX ) ;
      if (procptr->prev_env)
         markmemory( procptr->prev_env, TRC_VARBOX ) ;
      if (procptr->sig)
      {
         markmemory( procptr->sig, TRC_VARBOX ) ;
         if (procptr->sig->info)
            markmemory( procptr->sig->info, TRC_VARBOX ) ;
         if (procptr->sig->descr)
            markmemory( procptr->sig->descr, TRC_VARBOX ) ;
      }
      if (procptr->buf )
         markmemory( procptr->buf, TRC_VARBOX ) ;
      if (procptr->traps )
      {
         markmemory( procptr->traps, TRC_VARBOX ) ;
         for (i=0; i<SIGNALS; i++)
         {
            if (procptr->traps[i].name)
               markmemory( procptr->traps[i].name, TRC_VARBOX ) ;
         }
      }

      for(i=0;i<HASHTABLENGTH;i++)
      {
         for(vptr=(procptr->vars)[i];vptr;vptr=vptr->next)
         {
            markmemory((char*)vptr,TRC_VARBOX) ;
            if (vptr->name)
               markmemory((char*)vptr->name,TRC_VARNAME) ;
            if (vptr->num)
            {
               markmemory( vptr->num, TRC_VARVALUE ) ;
               markmemory( vptr->num->num, TRC_VARVALUE ) ;
            }
            if (vptr->value)
               markmemory((char*)vptr->value,TRC_VARVALUE) ;
            if (vptr->index)
            {
               markmemory( vptr->index, TRC_VARNAME) ;
               for (j=0; j<HASHTABLENGTH; j++)
               {
                  for(vvptr=(vptr->index)[j];vvptr;vvptr=vvptr->next)
                  {
                     markmemory((char*)vvptr,TRC_VARBOX) ;
                     if (vvptr->name)
                        markmemory((char*)vvptr->name,TRC_VARNAME) ;
                     if (vvptr->num)
                     {
                         markmemory( vvptr->num, TRC_VARVALUE ) ;
                         markmemory( vvptr->num->num, TRC_VARVALUE ) ;
                     }
                     if (vvptr->value)
                        markmemory((char*)vvptr->value,TRC_VARVALUE) ;
                  }
               }
            }
         }
      }
      markmemory((char*)procptr,TRC_PROCBOX) ;
/*      for (lptr=procptr->first; lptr; lptr=lptr->next)
      markmemory((char*)lptr, TRC_LABEL) ; */

      markmemory((char*)procptr->vars,TRC_HASHTAB) ;
      if (procptr->args)
      {
         for (pptr=procptr->args; pptr; pptr=pptr->next)
         {
            markmemory((char*) pptr, TRC_PROCARG) ;
            if (pptr->value)
               markmemory((char*) pptr->value, TRC_PROCARG) ;
         }
      }
   }

   for (vptr=vt->first_invalid; vptr; vptr=vptr->prev)
   {
      markmemory( vptr, TRC_VARBOX ) ;
   }
}
#endif /* TRACEMEM */

static variableptr newbox( const tsd_t *TSD, const streng *name, streng *value, variableptr *oldptr )
{
   variableptr newptr=NULL ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DSTART;DPRINT((TSD,"newbox:            "));DNAME(TSD,NULL,name);DPRINT((TSD," replaces "));
          DVAR(TSD,NULL,*oldptr);DEND;
   newptr = MallocTSD(sizeof(variable)) ;
   newptr->next = *oldptr ;
   newptr->prev = NULL ;
   newptr->realbox = NULL ;
   newptr->index = NULL ;
   newptr->stem = NULL ;
   newptr->num = NULL ;
   newptr->flag = value ? VFLAG_STR : VFLAG_NONE ;
   newptr->guard = 0 ;
   newptr->hwired = 0 ;
   newptr->valid = (long) vt->current_valid ;

   *oldptr = newptr ;
   newptr->value = value ;
   if (name)
      newptr->name = Str_dupTSD(name) ;
   else
      newptr->name = NULL ;
   DSTART;DPRINT((TSD,"newbox:            "));DVAR(TSD,"rc",newptr);DEND;
   return newptr ;
}

static variableptr make_stem( const tsd_t *TSD, const streng *name,
                              streng *value, variableptr *oldptr, int len )
{
   variableptr ptr=NULL ;

   ptr = newbox( TSD, NULL, value, oldptr ) ;
   ptr->index = make_hash_table( TSD ) ;
   DPRINTF((TSD,"make_hash_table:   rc=%p",ptr->index));
   ptr->name = Str_ndupTSD(name, len) ;
   DSTART;DPRINT((TSD,"makestem:          "));DVAR(TSD,"rc",ptr);DEND;
   return ptr ;
}

#define RXDIGIT 0x01
#define RXUPPER 0x02
#define RXLOWER 0x04
#define RXEXTRA 0x08
#define RXDOT   0x10
#define RXWHITE 0x20
#define RXVAR   ( RXUPPER | RXLOWER | RXEXTRA )
#define RXCONST ( RXUPPER | RXLOWER | RXEXTRA | RXDIGIT | RXDOT )
#define RXMANT  ( RXDIGIT | RXDOT )
#define CHAR_TYPE(c) char_types[(unsigned char) ( c )]
#define RXISDIGIT(a) ( CHAR_TYPE( a ) & RXDIGIT )
#define RXISUPPER(a) ( CHAR_TYPE( a ) & RXUPPER )
#define RXISLOWER(a) ( CHAR_TYPE( a ) & RXLOWER )
#define RXISEXTRA(a) ( CHAR_TYPE( a ) & RXEXTRA )
#define RXISDOT(a)   ( CHAR_TYPE( a ) & RXDOT   )
#define RXISWHITE(a) ( CHAR_TYPE( a ) & RXWHITE )
#define RXISVAR(a)   ( CHAR_TYPE( a ) & RXVAR   )
#define RXISCONST(a) ( CHAR_TYPE( a ) & RXCONST )
#define RXISMANT(a)  ( CHAR_TYPE( a ) & RXMANT )

static const unsigned char char_types[256] =
{
   0,       0,       0,       0,       0,       0,       0,       0,         /* nul - bel */
   0,       RXWHITE, RXWHITE, 0,       RXWHITE, 0,       0,       RXWHITE,   /* bs  - si  */
   0,       0,       0,       0,       0,       0,       0,       0,         /* dle - etb */
   0,       0,       0,       0,       0,       0,       0,       0,         /* can - us  */
   RXWHITE, RXEXTRA, 0,       RXEXTRA, RXEXTRA, 0,       0,       0,         /* sp  -  '  */
   0,       0,       0,       0,       0,       0,       RXDOT,   0,         /*  (  -  /  */
   RXDIGIT, RXDIGIT, RXDIGIT, RXDIGIT, RXDIGIT, RXDIGIT, RXDIGIT, RXDIGIT,   /*  0  -  7  */
   RXDIGIT, RXDIGIT, 0,       0,       0,       0,       0,       RXEXTRA,   /*  8  -  ?  */
   RXEXTRA, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER,   /*  @  -  G  */
   RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER,   /*  H  -  O  */
   RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER, RXUPPER,   /*  P  -  W  */
   RXUPPER, RXUPPER, RXUPPER, 0,       0,       0,       0,       RXEXTRA,   /*  X  -  _  */
   0,       RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER,   /*  `  -  g  */
   RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER,   /*  h  -  o  */
   RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER, RXLOWER,   /*  p  -  w  */
   RXLOWER, RXLOWER, RXLOWER, 0,       0,       0,       0,       0,         /*  x  - del */
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0,
   0,       0,       0,       0,       0,       0,       0,       0
} ;

/*
 * valid_var_symbol matches a symbol, which can be either a number, a
 * var_symbol, or a const_symbol in terms of ANSI 6.3.2.96.
 * see also: ANSI 6.2.2.29, 6.2.2.31, 6.2.2.35
 * Note that 6.2.2.32 means to parse an exponential character only if it
 * it part of a number. Thus, we have to parse the following:
 * var_symbol=RXVAR RXCONST*
 * const_symbol=RXCONST+
 * number=( '.' RXDIGIT+ | ( RXDIGIT+ ( '.' RXDIGIT* )? ) ) exp?
 * exp=( 'e' | 'E' ) ( '-' | '+' )? RXDIGIT+
 * We split var_symbol into simple, stem, compound and return all
 * SYMBOL_??? values.
 */
int valid_var_symbol( const streng *name )
{
   const unsigned char *cptr,*eptr;
   unsigned char ch;
   int dots,sign;

   if ( name->len == 0 )
      return SYMBOL_BAD;

   cptr = (const unsigned char *) name->value;
   eptr = cptr + name->len;

   ch = *cptr++;
   if ( RXISVAR( ch ) )
   {
      /*
       * valid begin of a variable name. Lets check it. This can't be
       * a number, thus we don't care about sign characters.
       */
      dots = 0;
      do {
         if ( cptr == eptr )
         {
            /*
             * the last character is properly loaded in ch
             */
            if ( RXISDOT( ch ) && ( dots == 1 ) )
               return SYMBOL_STEM;
            else if ( dots )
               return SYMBOL_COMPOUND;
            return SYMBOL_SIMPLE;
         }
         ch = *cptr++;
         if ( RXISDOT( ch ) )
            dots = 1;
         /*
          * characters after the first character are the same as of
          * const_symbol. They don't share the sign behaviour, though.
          */
      } while ( RXISCONST( ch ) );

      return SYMBOL_BAD;
   }

   /*
    * Check for a reserved variable first.
    * In all other cases we may have a number or a const_symbol.
    */
   if ( KNOWN_RESERVED( (char *)cptr - 1, name->len ) )
      return SYMBOL_SIMPLE;

   sign = 0;
   /*
    * Breaking/ending the following loops means to check for a const_symbol
    * with respect to the absense of a sign character.
    */
   for (;;) {
      /*
       * A number is a const_symbol with the exception of the sign within
       * an exponent. Try parsing a number first and fall back to a const
       * detection on error but keep a seen sign in mind.
       */
      if ( RXISDOT( ch ) )
      {
         /*
          * Check for a plain dot, which isn't a number, and for a following
          * digit.
          */
         if ( cptr == eptr )
            return SYMBOL_CONSTANT;
         ch = *cptr++;
         if ( !RXISDIGIT( ch ) )
            break;

         do {
            if ( cptr == eptr )
               return SYMBOL_NUMBER;

            ch = *cptr++;
            /*
             * expect just RXDIGITs as the mantissa.
             */
         } while ( RXISDIGIT( ch ) );
      }
      else
      {
         if ( !RXISDIGIT( ch ) )
            break;
         /*
          * expect RXDIGITs [ '.' RXDIGITs ] as the mantissa.
          */

         do {
            if ( cptr == eptr )
               return SYMBOL_NUMBER;

            ch = *cptr++;
         } while ( RXISDIGIT( ch ) );
         if ( RXISDOT( ch ) )
         {
            if ( cptr == eptr )
               return SYMBOL_NUMBER;

            ch = *cptr++;
            if ( RXISDIGIT( ch ) )
            {
               do {
                  if ( cptr == eptr )
                     return SYMBOL_NUMBER;

                  ch = *cptr++;
               } while ( RXISDIGIT( ch ) );
            }
         }
      }

      /*
       * We have to parse an exponent. ch has the current character.
       */
      if ( ( ch != 'e' ) && ( ch != 'E' ) )
         break;

      if ( cptr == eptr )
         return SYMBOL_CONSTANT;

      ch = *cptr++;
      if ( ( ch == '+' ) || ( ch == '-' ) )
      {
         sign = 1;
         if ( cptr == eptr )
            return SYMBOL_BAD; /* something like "1.2E+" */
         ch = *cptr++;
      }
      /*
       * parse the exponent value
       */
      if ( !RXISDIGIT( ch ) )
         break;
      do {
         if ( cptr == eptr )
            return SYMBOL_NUMBER;

         ch = *cptr++;
      } while ( RXISDIGIT( ch ) );

      break;
   }

   /*
    * We have to check for a const_symbol. If a sign has occured until now
    * we have a bad symbol.
    * ch is loaded with the current character.
    */
   if ( sign || !RXISCONST( ch ) )
      return SYMBOL_BAD;

   do {
      if ( cptr == eptr )
         return SYMBOL_CONSTANT;

      ch = *cptr++;
   } while ( RXISCONST( ch ) );

   /*
    * garbage in ch
    */
   return SYMBOL_BAD;
}


static int hashfunc( var_tsd_t *vt, const streng *name, int start, int *stop )
{
   register int sum=0, idx=0 ;
   const char *ch1=NULL, *ech0=NULL ;

   ch1 = name->value ;
   ech0 = Str_end( name ) ;

   ch1 += start ;
   sum = idx = 0 ;
   for (; (ch1<ech0);ch1++)
   {
      if (*ch1 == '.')
      {
         if (stop)
            break ;
         else
            continue ;
      }
      if (RXISDIGIT(*ch1))
         idx = idx*10 + (*ch1 - '0') ;
      else
      {
         if (idx)
         {
            sum = (sum) + RXTOLOW(*ch1) + idx ;
            idx = 0 ;
         }
         else
            sum = (sum) + RXTOLOW(*ch1) ;
      }
   }

   if (stop)
      *stop = ch1 - name->value ;

   vt->hashval = (sum + idx) & (HASHTABLENGTH-1);
   return( vt->hashval ) ;
}

variableptr *create_new_varpool( const tsd_t *TSD )
{
   variableptr *retval = make_hash_table( TSD ) ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DPRINTF((TSD,"make_hash_table:   rc=%p",retval));
   DPRINTF((TSD,"create_new_varpool:current_valid:new=%d, old=%d",
             vt->next_current_valid,vt->current_valid));
   retval[HASHTABLENGTH] = (variableptr) vt->current_valid ;
   vt->current_valid = vt->next_current_valid++;
   return retval ;
}

void set_ignore_novalue( const tsd_t *TSD )
{
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   vt->ignore_novalue = 1 ;
   DPRINTF((TSD,"set_ignore_novalue"));
}

void clear_ignore_novalue( const tsd_t *TSD )
{
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   vt->ignore_novalue = 0 ;
   DPRINTF((TSD,"clear_ignore_novalue"));
}

const streng *get_it_anyway( tsd_t *TSD, const streng *str )
{
   const streng *ptr ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   vt->notrace = 1 ;
   vt->ignore_novalue = 1 ;
   ptr = getvalue(TSD,str,FALSE) ;
   vt->ignore_novalue = 0 ;
   vt->notrace = 0 ;

   if (!ptr)
      exiterror( ERR_SYMBOL_EXPECTED, 1, tmpstr_of( TSD, str ) )  ;

   DSTART;DPRINT((TSD,"get_it_anyway:     "));DNAME(TSD,"str",str);DVALUE(TSD,", rc",ptr);DEND;
   return ptr ;
}

const streng *get_it_anyway_compound( tsd_t *TSD, const streng *str )
/* as get_it_anyway but specific to getdirvalue_compound */
{
   const streng *ptr ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   vt->notrace = 1 ;
   vt->ignore_novalue = 1 ;
   ptr = getdirvalue_compound(TSD,str) ;
   vt->ignore_novalue = 0 ;
   vt->notrace = 0 ;

   if (!ptr)
      exiterror( ERR_SYMBOL_EXPECTED, 1, tmpstr_of( TSD, str ) )  ;

   DSTART;DPRINT((TSD,"get_it_anyway_compound:"));DNAME(TSD,"str",str);DVALUE(TSD,", rc",ptr);DEND;
   return ptr ;
}

int var_was_found( const tsd_t *TSD )
{
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   DPRINTF((TSD,"var_was_found:     rc=%d",vt->foundflag));
   return vt->foundflag ;
}

const streng *isvariable( tsd_t *TSD, const streng *str )
{
   const streng *ptr=NULL ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   vt->ignore_novalue = 1 ;
   ptr = getvalue(TSD,str,FALSE) ;
   vt->ignore_novalue = 0 ;
   DSTART;DPRINT((TSD,"isvariable:        "));DNAME(TSD,"str",str);
          DVALUE(TSD,", rc",(vt->foundflag)?ptr:NULL);DEND;
   if (vt->foundflag)
      return ptr ;

   return NULL ;
}

#ifdef TRACEMEM
static void mark_variables( const tsd_t *TSD )
{
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   markmemory( vt->tmpindex, TRC_STATIC ) ;
   if (vt->ovalue)
      markmemory( vt->ovalue, TRC_STATIC ) ;
   if (vt->xvalue)
      markmemory( vt->xvalue, TRC_STATIC ) ;
   if (vt->odescr)
   {
      markmemory( vt->odescr, TRC_STATIC ) ;
      markmemory( vt->odescr->num, TRC_STATIC ) ;
   }
}
#endif

/* init_vars initializes the module.
 * Currently, we set up the thread specific data and check for environment
 * variables to change debugging behaviour.
 * The function returns 1 on success, 0 if memory is short.
 */
int init_vars( tsd_t *TSD )
{
   var_tsd_t *vt;

   if (TSD->var_tsd != NULL)
      return(1);

   if ((vt = TSD->var_tsd = MallocTSD(sizeof(var_tsd_t))) == NULL)
      return(0);
   memset(vt,0,sizeof(var_tsd_t));

#ifdef DEBUG
   {
      char junk[100];
   if ( mygetenv( TSD, "DEBUG_VARIABLE", junk, sizeof(junk) ) != NULL)
      vt->DoDebug = 1;
   }
#endif

# ifdef TRACEMEM
   regmarker( TSD, mark_variables ) ;
# endif
   vt->current_valid = 1;
   vt->next_current_valid = 2 ;
   vt->tmpindex = Str_makeTSD( MAX_INDEX_LENGTH ) ;
   DPRINTF((TSD,"init_vars"));
   return(1);
}

/*
 * This routine takes a ptr to a linked list of nodes, each describing
 * one element in a tail of a compound variable. Each of the elements
 * will eventually be cached, since they are retrieved through the
 * shortcut() routine.
 */
static streng *fix_index( tsd_t *TSD, nodeptr this )
{
   char *cptr=NULL ;
   const streng *value=NULL ;
   int osetting=0 ;
   int freespc=0 ;
   streng *large=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   assert( this ) ;
   osetting = vt->ignore_novalue ;
   vt->ignore_novalue = 1 ;

   DPRINTF((TSD,"fix_index, start:  this=%p",this));
   freespc = vt->tmpindex->max ;
   cptr = vt->tmpindex->value ;

#ifdef FANCY
   if (!this->p[0])
   {
      assert( this->type==X_CTAIL_SYMBOL || this->type==X_VTAIL_SYMBOL) ;
      if (this->type == X_CTAIL_SYMBOL)
         value = this->name ;
      else
      {
         vt->subst = 1 ;
         value = shortcut( vt->TSD, this ) ;
      }

      vt->ignore_novalue = osetting ;
      return value ;
   }
#endif

   for (;;)
   {
      assert( this->type==X_CTAIL_SYMBOL || this->type==X_VTAIL_SYMBOL) ;
      if (this->type == X_CTAIL_SYMBOL)
         value = this->name ;
      else
      {
         vt->subst = 1 ;
         value = shortcut( TSD, this ) ;
      }

      freespc -= value->len;
      if (freespc-- <= 0)
      {
         large = Str_makeTSD( vt->tmpindex->max * 2 + value->len ) ;
         memcpy( large->value, vt->tmpindex->value, (cptr-vt->tmpindex->value)) ;
         cptr = large->value + (cptr-vt->tmpindex->value) ;
         freespc += (large->max - vt->tmpindex->max) ;
         Free_stringTSD( vt->tmpindex ) ;
         vt->tmpindex = large ;

         assert( freespc >= 0 ) ;
      }

      memcpy( cptr, value->value, value->len ) ;
      cptr += value->len ;
      this = this->p[0] ;
      if (this)
         *(cptr++) = '.' ;
      else
         break ;
   }
   vt->tmpindex->len = cptr - vt->tmpindex->value ;
   assert( vt->tmpindex->len <= vt->tmpindex->max ) ;
   vt->ignore_novalue = osetting ;
   DSTART;DPRINT((TSD,"fix_index, end:    this=%p, "));DVALUE(TSD,"rc",vt->tmpindex);DEND;
   return vt->tmpindex ;
}

void expand_to_str( const tsd_t *TSD, variableptr ptr )
{
   int flag=0 ;

   flag = ptr->flag ;

   DSTART;DPRINT((TSD,"expand_to_str:     "));DVAR(TSD,"ptr",ptr);DEND;
   if (flag & VFLAG_STR)
      return ;

   if (flag & VFLAG_NUM)
   {
      assert( ptr->num ) ;
      ptr->value = str_norm( TSD, ptr->num, ptr->value ) ;
      ptr->flag |= VFLAG_STR ;
   }
   DSTART;DPRINT((TSD,"expand_to_str:     "));DVAR(TSD,"ptr",ptr);DEND;
}

static streng *subst_index( const tsd_t *TSD, const streng *name, int start, variableptr *vars )
{
   int i=0, length=0 ;
   variableptr nptr=NULL ;
   int stop=0 ;
   char *cptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   assert( start < name->len ) ;

   DPRINTF((TSD,"subst_index:       ?"));
   vt->tmpindex->len = 0 ;
   vt->subst = 0 ;

   for ( ;; )
   {
      nptr = vars[ hashfunc( vt, name, start, &stop ) ] ;

      length = stop - start ;
      for (; nptr; nptr=nptr->next )
      {
         if (nptr->name->len != length)  /* lengths differ */
            continue ;

         if (Str_cnocmp(nptr->name,name,length,start))  /* contents differ */
            continue ;

         break ;
      }

      if (nptr)
        for (;nptr->realbox; nptr=nptr->realbox) ;

      if (nptr)
         expand_to_str( TSD, nptr );

      if ((nptr) && (nptr->value))
      {
         Str_catTSD( vt->tmpindex, nptr->value ) ;
         vt->subst = 1 ;
      }
      else
      {
         cptr = vt->tmpindex->value + vt->tmpindex->len ;
         for (i=start ;i<stop; i++)
#if 0
         /*
          * MH - don't uppercase the tail
          * - YES reverted for 0.08h, breaks SYMBOL BIF if
          *   tail is not uppercased.
          */
            *(cptr++) = (char) (name->value[i] ) ;
#else
            *(cptr++) = (char) (toupper( name->value[i] )) ;
#endif
         vt->tmpindex->len = cptr - vt->tmpindex->value ;
      }

      if (stop>=Str_len(name))
         break ;

      start = stop + 1 ;
      vt->tmpindex->value[vt->tmpindex->len++] = '.' ;
   }

   return vt->tmpindex ;
}

static void kill_index( const tsd_t *TSD, variableptr *array, int kill, int prop, const streng *val )
{
   register variableptr ptr=NULL, tptr=NULL, *eptr=NULL, *aptr=NULL ;

   DPRINTF((TSD,"kill_index:        ?"));
   aptr = array ;
   eptr = aptr + HASHTABLENGTH ;
   for ( ; aptr<eptr; aptr++ )
   {
      if (*aptr)
      {
         tptr = *aptr ;
         for (;(ptr=tptr)!=NULL;)
         {
            tptr = tptr->next ;
            if (prop && ptr->realbox)
            {
               variableptr tttptr ;
               for (tttptr=ptr; tttptr->realbox; tttptr=tttptr->realbox )
                  ;
               if (val)
               {
                  streng *tmpval = Str_dupTSD(val) ;
                  REPLACE_VALUE( tmpval, tttptr ) ;
                  DSTART;DPRINT((TSD,"                   "));DVAR(TSD,"tttptr(now)",tttptr);DEND;
               }
               else if (tttptr->value)
               {
                  Free_stringTSD( tttptr->value ) ;
                  tttptr->value = NULL ;
                  DSTART;DPRINT((TSD,"                   "));DVAR(TSD,"tttptr(now)",tttptr);DEND;
               }
            }
            DSTART;DPRINT((TSD,"                   "));DVAR(TSD,"ptr(del)",ptr);DEND;
            Free_stringTSD(ptr->name) ;
            if (ptr->value)
               Free_stringTSD(ptr->value) ;

            if (ptr->index)
            {
               assert( prop==0 ) ;
               kill_index( TSD, ptr->index, kill, 0, NULL ) ;
            }

            if (ptr->num)
            {
               FreeTSD( ptr->num->num ) ;
               FreeTSD( ptr->num ) ;
            }

            if (ptr->hwired)
            {
               ptr->valid = 0 ;
#ifdef TRACEMEM
               {
                  var_tsd_t *vt = TSD->var_tsd;
                  ptr->prev = vt->first_invalid ;
                  ptr->next = NULL ;
                  if (vt->first_invalid)
                     vt->first_invalid->next = ptr ;
                  vt->first_invalid = ptr ;
               }
#endif
            }
            else
               FreeTSD(ptr) ;
         }
         *aptr = NULL ;
      }
   }
   if (kill)
   {
      DSTART;DPRINT((TSD,"                   kill=%p",array));DEND;
      FreeTSD( array ) ;
   }
}

static variableptr findsimple( const tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = TSD->currlevel->vars[hashfunc(vt,name,0,NULL)] ;
   for (;(ptr)&&(Str_ccmp(ptr->name,name));ptr=ptr->next)
      ;
   DSTART;DPRINT((TSD,"findsimple(1):     "));DNAME(TSD,"name",name);DVAR(TSD,", ptr",ptr);DEND;
   if ((vt->thespot=ptr)!=NULL)
   {
      for (;ptr->realbox; ptr=ptr->realbox)
         ;
   }
   vt->thespot=ptr;
   DSTART;DPRINT((TSD,"findsimple(2):     "));DNAME(TSD,"name",name);
          DVAR(TSD,", vt->thespot=ptr",ptr);DEND;

   return ptr ;
}

static void setvalue_simple( const tsd_t *TSD, const streng *name, streng *value )
{
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = findsimple( TSD, name ) ;
   if (ptr)
   {
      vt->foundflag = (ptr->flag & VFLAG_BOTH) ;
      REPLACE_VALUE(value,ptr) ;
      DSTART;DPRINT((TSD,"setvalue_simple:   "));DVAR(TSD,"replacement",ptr);DEND;
   }
   else
   {
      vt->foundflag = 0 ;
      vt->thespot = newbox( TSD, name, value, &((TSD->currlevel->vars)[vt->hashval]) ) ;
      DSTART;DPRINT((TSD,"setvalue_simple:   "));DVAR(TSD,"new, vt->thespot",ptr);DEND;
   }
}

static void setvalue_reserved( const tsd_t *TSD, int ridx, const streng *name, streng *value )
{
   ridx = ridx;
   setvalue_simple( TSD, name, value );
}

static const streng *getvalue_simple( tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL ;
   const streng *value=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = findsimple( TSD, name) ;

   vt->foundflag = ((ptr)&&(ptr->flag & VFLAG_BOTH)) ;

   if (ptr)
      expand_to_str( TSD, ptr );

   if (vt->foundflag)
      value = ptr->value ;
   else
   {
      value = name ;
      vt->thespot = NULL ;
      if (!vt->ignore_novalue)
         condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(value), NULL ) ;
   }

   if (!vt->notrace)
      tracevalue(TSD,value,(char) (((ptr) ? 'V' : 'L'))) ;

   DSTART;DPRINT((TSD,"getvalue_simple:   "));DNAME(TSD,"name",name);
          DVALUE(TSD," rc",value);DEND;
   return value ;
}

static const streng *getvalue_reserved( tsd_t *TSD, int ridx, const streng *name )
{
   ridx = ridx;
   return getvalue_simple( TSD, name );
}

static void setvalue_stem( const tsd_t *TSD, const streng *name, streng *value )
{
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"setvalue_stem:     ?"));

   ptr = findsimple( TSD, name ) ;

   if (ptr)
   {
      vt->foundflag = ( ptr->flag & VFLAG_BOTH) ;
      REPLACE_VALUE( value, ptr ) ;
      if (ptr->index)
         kill_index( TSD, ptr->index, 0, 1, value ) ;
   }
   else
   {
      vt->foundflag = 0 ;
      make_stem( TSD, name, value, &(TSD->currlevel->vars[vt->hashval]), name->len ) ;
   }
   vt->thespot = NULL ;
}

static void setvalue_compound( const tsd_t *TSD, const streng *name, streng *value )
{
   variableptr ptr=NULL, nptr=NULL, *nnptr=NULL, *pptr=NULL ;
   int stop=0 ;
   streng *indexstr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"setvalue_compound: ?"));
   vt->foundflag = 0 ;
   pptr = &(TSD->currlevel->vars[hashfunc(vt,name,0,&stop)]) ;
   stop++ ;
   for (ptr=*pptr;(ptr)&&(Str_cncmp(ptr->name,name,stop));ptr=ptr->next) ;

   if (!ptr)
      ptr = make_stem( TSD, name, NULL, pptr, stop ) ;

   for (;(ptr->realbox);ptr=ptr->realbox)
      ;
   indexstr = subst_index( TSD, name, stop, TSD->currlevel->vars ) ;

   if (vt->subst)   /* trace it */
      tracecompound(TSD,name,stop-1,indexstr,'C') ;

   nnptr = &((ptr->index)[hashfunc(vt,indexstr,0,NULL)]) ;
   for (nptr=*nnptr;(nptr)&&(Str_cmp(nptr->name,indexstr));nptr=nptr->next)
      ;

   if (nptr)
   {
      for (;(nptr->realbox);nptr=nptr->realbox) ;
      vt->foundflag = ( nptr && (nptr->flag & VFLAG_BOTH)) ;
      REPLACE_VALUE(value,nptr) ;
   }
   else
   {
      newbox(TSD,indexstr,value,nnptr) ;
      (*nnptr)->stem = ptr ;
   }

   vt->thespot = NULL ;
}

/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from setvalue_compound().
 *   - Started using the global variable, vt->tmpindex, in place of the local,
 *     indexstr.
 *   - manually move the first stem name into vt->tmpindex, do not call
 *     subst_index(), as that not only uppercases the tail, but also
 *     does not uppercase the tail.
 *
 *
 ****************************************************************************/
void setdirvalue_compound( const tsd_t *TSD, const streng *name, streng *value )
{
   variableptr ptr=NULL, nptr=NULL, *nnptr=NULL, *pptr=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"setdirvalue_compound: ?"));
   vt->foundflag = 0 ;
   /*  Get a good starting point, and find the stem/index separater.    */
   pptr = &(TSD->currlevel->vars[hashfunc(vt,name,0,&stop)]) ;
   stop++ ;
   /*  Find the stem in the variable pool.                              */
   for (ptr=*pptr;(ptr)&&(Str_cncmp(ptr->name,name,stop));ptr=ptr->next)
      ;

   /*  If the stem does not exist, make one.                            */
   if (!ptr)
      ptr = make_stem( TSD, name, NULL, pptr, stop ) ;

   /* Back up through the EXPOSE chain 'til get to the real variable.   */
   for (;(ptr->realbox);ptr=ptr->realbox)
      ;
   /* indexstr = subst_index( name, stop, vt->TSD->currlevel->vars ) ; */
   /*  Use the global that is defined and allocated by init_vars()      */
   /*  Don't have to worry about freeing, or causing a memory leak.     */
   /*  It is also what the subst_index() would have had us using.       */
   vt->tmpindex->len = 0;
   vt->tmpindex = Str_nocatTSD(vt->tmpindex,name,name->len - stop,stop);

   if (vt->subst)   /* trace it */
      tracecompound(TSD,name,stop-1,vt->tmpindex,'C') ;

   nnptr = &((ptr->index)[hashfunc(vt,vt->tmpindex,0,NULL)]) ;
   for (nptr=*nnptr;(nptr)&&(Str_cmp(nptr->name,vt->tmpindex));nptr=nptr->next)
      ;

   if (nptr)
   {
      for (;(nptr->realbox);nptr=nptr->realbox)
         ;
      vt->foundflag = ( nptr && (nptr->flag & VFLAG_BOTH)) ;
      REPLACE_VALUE(value,nptr) ;
   }
   else
   {
      newbox(TSD,vt->tmpindex,value,nnptr) ;
      (*nnptr)->stem = ptr ;
   }

   vt->thespot = NULL ;
}

static void expose_simple( const tsd_t *TSD, const streng *name )
{
   int hashv=0 ;
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = vt->var_table[hashv=hashfunc(vt,name,0,NULL)] ;
   for (;(ptr)&&(Str_ccmp(ptr->name,name));ptr=ptr->next)
      ;
   if (ptr)  /* hey, you just exposed that one! */
      return ;

   ptr = TSD->currlevel->vars[hashv] ;
   for (;(ptr)&&(Str_ccmp(ptr->name,name));ptr=ptr->next)
      ;
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;

   if (!ptr)
   {
/*    condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name) ) ; */
      newbox(TSD,name,NULL,&TSD->currlevel->vars[hashv]) ;
   }

   newbox(TSD,name,NULL,&vt->var_table[hashv]) ;
   vt->var_table[hashv]->realbox = ((ptr) ? (ptr) : TSD->currlevel->vars[hashv]) ;
   /* exposing is done after create_new_varpool/assignment of current_valid: */
   (vt->var_table[hashv]->realbox)->valid = vt->current_valid ;
   DSTART;DPRINT((TSD,"expose_simple:     "));DNAME(TSD,"name",name);DEND;
}

static void expose_reserved( const tsd_t *TSD, int ridx, const streng *name )
{
   ridx = ridx;
   expose_simple( TSD, name );
}

static void expose_stem( const tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL, tptr=NULL ;
   int hashv=0, junk=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"expose_stem:       ?"));
   ptr = vt->var_table[hashv=hashfunc(vt,name,0,&junk)] ;
   for (;(ptr)&&(Str_ccmp(ptr->name,name));ptr=ptr->next)
      ;
   if ((ptr)&&(ptr->realbox))
      return ; /* once is enough !!! */

   tptr = TSD->currlevel->vars[hashv] ;
   for (;(tptr)&&(Str_ccmp(tptr->name,name));tptr=tptr->next)
      ;
   for (; tptr && tptr->realbox; tptr=tptr->realbox )
      ;

   if (!tptr)
   {
/*    condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name) ) ; */
      newbox(TSD,name,NULL,&TSD->currlevel->vars[hashv]) ;
      (tptr=TSD->currlevel->vars[hashv])->index = make_hash_table( TSD ) ;
      DPRINTF((TSD,"make_hash_table:   rc=%p",tptr->index));
   }

   if (ptr)
   {
      kill_index( TSD, ptr->index, 1, 0, NULL) ;
      ptr->index = NULL ;
      assert(( ptr->realbox==NULL) || (ptr->realbox==tptr )) ;
      ptr->realbox = tptr ;
   }
   else
   {
      newbox(TSD,name,NULL,&vt->var_table[hashv]) ;
      vt->var_table[hashv]->realbox = tptr ; /* dont need ->index */
   }
   /* FGC: Maybe, we need to set valid? In case of an error try valid setting
           first; already found one error of this type. */
}

static void expose_compound( const tsd_t *TSD, const streng *name )
{
   int hashv=0, length=0, hashval2=0 ;
   variableptr ptr=NULL, nptr=NULL, tptr=NULL, tiptr=NULL ;
   int cptr=0 ;
   streng *indexstr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"expose_compound:   ?"));
   ptr = vt->var_table[hashv=hashfunc(vt,name,0,&cptr)] ;
   length = ++cptr ;
   for (;(ptr)&&(Str_cncmp(ptr->name,name,length));ptr=ptr->next)
      ;
   if ((ptr)&&(ptr->realbox))
      return ; /* whole array already exposed */

   if (!ptr) /* array does not exist */
   {
/*    condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name) ) ; */
      make_stem(TSD,name,NULL,&vt->var_table[hashv],length) ;
      ptr = vt->var_table[hashv] ;
   }

   indexstr = subst_index( TSD, name, cptr, vt->var_table ) ;

   if (vt->subst)   /* trace it */
      tracecompound(TSD,name,cptr-1,indexstr,'C') ;

   nptr = (ptr->index)[hashval2=hashfunc(vt,indexstr,0,NULL)] ;
   for (;(nptr)&&(Str_cmp(nptr->name,indexstr));nptr=nptr->next)
      ;
   if ((nptr)&&(nptr->realbox))
      return ; /* can't your remember *anything* !!! */
   else
   {
      newbox(TSD,indexstr,NULL,&ptr->index[hashval2]) ;
      nptr = ptr->index[hashval2] ;
   }

   tptr = TSD->currlevel->vars[hashv] ;
   for (;(tptr)&&(Str_cncmp(tptr->name,name,length));tptr=tptr->next)
      ;
   for (;(tptr)&&(tptr->realbox);tptr=tptr->realbox)
      ;
   if (!tptr)
   {
/*    condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name) ) ; */
      make_stem(TSD,name,NULL,&TSD->currlevel->vars[hashv],length) ;
      tptr = TSD->currlevel->vars[hashv] ;
   }

   tiptr = tptr->index[hashval2] ; /* MDW 20020119: tptr = tptr->index[hashval2] ; */
   for (; tiptr && Str_cmp(tiptr->name,indexstr); tiptr=tiptr->next) /* MDW 20020119: for (; tptr && Str_cmp(tptr->name,indexstr); tptr=tptr->next) */
      ;
   for (; tiptr && tiptr->realbox; tiptr=tiptr->realbox ) /* MDW 20020119: for (; tptr && tptr->realbox; tptr=tptr->realbox ) */
      ;
   if (!tiptr) /* MDW 20020119: if (!tptr) */
   {
      /*
       * MDW 20020119:
      newbox(TSD,indexstr,NULL,&TSD->currlevel->vars[hashv]->index[hashval2]) ;
      tptr = TSD->currlevel->vars[hashv]->index[hashval2] ;
      tptr->stem = TSD->currlevel->vars[hashv] ;
      */
      newbox(TSD,indexstr,NULL,&tptr->index[hashval2]) ;
      tiptr = tptr->index[hashval2] ;
      tiptr->stem = tptr ;
   }

   nptr->realbox = tiptr; /* MDW 20020119: nptr->realbox = tptr; */ /*TSD->currlevel->vars[hashv]->index[hashval2] */
   /* FGC: Maybe, we need to set valid? In case of an error try valid setting
           first; already found one error of this type. */
}

static const streng *getvalue_compound( tsd_t *TSD, const streng *name )
{
   int hashv=0, baselength=0 ;
   variableptr ptr=NULL, nptr=NULL ;
   streng *value=NULL ;
   streng *indexstr=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"getvalue_compound: ?"));
   ptr = TSD->currlevel->vars[hashv=hashfunc(vt,name,0,&stop)] ;
   baselength = ++stop ;
   for (;(ptr)&&(Str_cncmp(ptr->name,name,baselength));ptr=ptr->next)
      ;
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;
   indexstr = subst_index( TSD, name, stop, TSD->currlevel->vars ) ;
   hashv = hashfunc(vt,indexstr,0,NULL) ;

   if (vt->subst && !vt->notrace)   /* trace it */
      tracecompound(TSD,name,baselength-1,indexstr,'C') ;

   if (ptr)
   {   /* find specific value */
      nptr = ((variableptr *)(ptr->index))[hashv] ;
      for (;(nptr)&&(Str_cmp(nptr->name,indexstr));nptr=nptr->next)
         ;
      for (;(nptr)&&(nptr->realbox);nptr=nptr->realbox)
         ;
   }

   if ((ptr)&&(!nptr))   /* find default value */
      nptr = ptr ;

   vt->foundflag = (ptr)&&(nptr)&&(nptr->flag & VFLAG_BOTH) ;
   if (ptr && nptr)
      expand_to_str( TSD, nptr );

   if (vt->foundflag)
      value = (nptr)->value ;
   else
   {
      if (!vt->ignore_novalue)
         condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name), NULL ) ;

      if (vt->ovalue)
         Free_stringTSD( vt->ovalue ) ;

      vt->ovalue = value = Str_makeTSD( stop + 1 + Str_len(indexstr) ) ;
      Str_ncatTSD( value, name, stop ) ;
      Str_catTSD( value, indexstr ) ;
   }

   vt->thespot = NULL ;
   return( value ) ;
}

/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from getvalue_compound().
 *   - Started using the global variable, vt->tmpindex, in place of the local,
 *     indexstr.
 *   - manually move the first stem name into vt->tmpindex, do not call
 *     subst_index(), as that not only uppercases the tail, but also
 *     does not uppercase the tail.
 *
 *
 ****************************************************************************/
const streng *getdirvalue_compound( tsd_t *TSD, const streng *name )
{
   int hashv=0, baselength=0 ;
   variableptr ptr=NULL, nptr=NULL ;
   streng *value=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"getdirvalue_compound: ?"));
   /*  Get a good starting point, and find the stem/index separater.    */
   ptr = TSD->currlevel->vars[hashv=hashfunc(vt,name,0,&stop)] ;
   baselength = ++stop ;
   /*  Find the stem in the variable pool.                              */
   for (;(ptr)&&(Str_cncmp(ptr->name,name,baselength));ptr=ptr->next)
      ;
   /* Back up through the EXPOSE chain 'til get to the real variable.   */
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;
   /* indexstr = subst_index( name, stop, TSD->currlevel->vars ) ;  */
   /*  Get the index name to use from the literal variable name.        */
   /*  Use the global that is defined and allocated by init_vars()      */
   /*  Don't have to worry about freeing, or causing a memory leak.     */
   /*  It is also what the subst_index() would have had us using.       */
   vt->tmpindex->len = 0;
   vt->tmpindex = Str_nocatTSD(vt->tmpindex,name,name->len - stop,stop);
   /*  Set up to look for this name in the stem's variable pool.        */
   hashv = hashfunc(vt,vt->tmpindex,0,NULL) ;

   if (vt->subst && !vt->notrace)   /* trace it */
      tracecompound(TSD,name,baselength-1,vt->tmpindex,'C') ;

   if (ptr)
   {   /* find specific value */
      /*  Get a good starting place for the index name.                 */
      nptr = ((variableptr *)(ptr->index))[hashv] ;
      /*  Find the index in the variable pool.                          */
      for (;(nptr)&&(Str_cmp(nptr->name,vt->tmpindex));nptr=nptr->next) ;
      /* Back up through the EXPOSE chain 'til get to the real variable.*/
      for (;(nptr)&&(nptr->realbox);nptr=nptr->realbox) ;
   }

   /*  If the stem exists, but the index doesn't, this counts as found. */
   if ((ptr)&&(!nptr))   /* find default value */
      nptr = ptr ;

   vt->foundflag = (ptr)&&(nptr)&&(nptr->flag & VFLAG_BOTH) ;
   if (ptr && nptr)
      expand_to_str( TSD, nptr );
   if (vt->foundflag)
      value = (nptr)->value ;
   else
   {
      if (!vt->ignore_novalue)
         condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name), NULL ) ;
         /* condition_hook( TSD, SIGNAL_NOVALUE, 0, -1, Str_dupTSD(name) ) ;  JH 20-10-99  Dunno why it was like this! */

      if (vt->ovalue)
         Free_stringTSD( vt->ovalue ) ;
      /*  Since this is a direct, just copy the name over.              */
      vt->ovalue = value = Str_makeTSD( name->len ) ;
      Str_catTSD( value, name ) ;
   }

   vt->thespot = NULL ;
   return( value ) ;
}

/*
 * This is the entry-level routine that will take the parameters,
 *  decide what kind of variable it is (simple, stem or compound) and
 *  call the appropriate routine to do the dirty work
 */
void setvalue( const tsd_t *TSD, const streng *name, streng *value )
{
   int i, len=Str_len(name) ;

   assert( value->len <= value->max ) ;

   if ( ( i = KNOWN_RESERVED( name->value, len ) ) != 0 )
      setvalue_reserved( TSD, i, name, value );
   else
   {
      for (i=0;(i<len)&&(name->value[i]!='.');i++)
         ;

      if (i==len)
         setvalue_simple(TSD,name,value) ;
      else if ((i+1)==len)
         setvalue_stem(TSD,name,value) ;
      else
         setvalue_compound(TSD,name,value) ;
   }
}

/*
 * This is the entry-level routine used by the Variable Pool Interface
 *  to set stem variables directly.  (no translation on the index name.)
 *  As setvalue() does, it will take the parameters,
 *  decide what kind of variable it is (simple, stem or compound) and
 *  call the appropriate routine to do the dirty work
 */
/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from setvalue().
 *   - changed call from setvalue_compound() to setdirvalue_compound().
 *
 *
 ****************************************************************************/
void setdirvalue( const tsd_t *TSD, const streng *name, streng *value )
{
   int i, len=Str_len(name) ;

   assert( value->len <= value->max ) ;

   if ( ( i = KNOWN_RESERVED( name->value, len ) ) != 0 )
      setvalue_reserved( TSD, i, name, value );
   else
   {
      for (i=0;(i<len)&&(name->value[i]!='.');i++)
         ;

      if (i==len)
         setvalue_simple(TSD,name,value) ;
      else if ((i+1)==len)
         setvalue_stem(TSD,name,value) ;
      else
         setdirvalue_compound(TSD,name,value) ;
   }
}

void expose_var( const tsd_t *TSD, const streng* name )
{
   int i;
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   if (!vt->var_table)
   {
      /*
       * First call to expose_var of X_PROC in interpret().
       * Open a new table.
       */
      vt->var_table = create_new_varpool( TSD );
   }

   if (!name)
   {
      /*
       * Last call to expose_var of X_PROC in interpret().
       * Use the new table as the current table.
       */
      TSD->currlevel->vars = vt->var_table ;
      TSD->currlevel->varflag = 1 ;
      vt->var_table = NULL ;
/*      vt->current_valid++ ; */
      return ;
   }

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      expose_reserved( TSD, i, name );
   else
   {
      for (i=0;(Str_in(name,i))&&(name->value[i]!='.');i++)
         ;

      if (i>=name->len)
         expose_simple(TSD,name) ;
      else if (i==name->len-1)
         expose_stem(TSD,name) ;
      else
         expose_compound(TSD,name) ;
   }
}

const streng *getvalue( tsd_t *TSD, const streng *name, int dummy )
{
   int i;
   const char *cptr=NULL, *eptr=NULL ;

   dummy = dummy; /* keep compiler happy */

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      return getvalue_reserved( TSD, i, name );

   cptr = name->value ;
   eptr = cptr + name->len ;
   for (; cptr<eptr && *cptr!='.'; cptr++)
      ;

   /*
    * Setvalue_stem is equivalent to setvalue_simple
    */
   if ((unsigned long) cptr+1 >= (unsigned long) eptr)
      return getvalue_simple(TSD,name) ;
   else
      return getvalue_compound(TSD,name) ;
}

/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from getvalue().
 *   - changed call from getvalue_compound() to getdirvalue_compound().
 *
 *
 ****************************************************************************/
const streng *getdirvalue( tsd_t *TSD, const streng *name, int dummy )
{
   int i;
   const char *cptr,*eptr;

   dummy = dummy; /* keep compiler happy */

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      return getvalue_reserved( TSD, i, name );

   cptr = name->value ;
   eptr = cptr + name->len ;
   for (; cptr<eptr && *cptr!='.'; cptr++)
      ;

   if ((unsigned long) cptr+1 >= (unsigned long) eptr)
      return getvalue_simple(TSD,name) ;
   else
      return getdirvalue_compound(TSD,name) ;
}

static void drop_var_simple( const tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = findsimple( TSD, name ) ;
   DSTART;DPRINT((TSD,"drop_var_simple:   "));DNAME(TSD,"name",name);DVAR(TSD,", var",ptr);
          DEND;

   vt->foundflag = 0 ;
   if (ptr)
   {
      vt->foundflag = ptr->flag & VFLAG_BOTH ;
      ptr->flag = VFLAG_NONE ;
      if (ptr->value)
      {
         Free_stringTSD( ptr->value ) ;
         ptr->value = NULL ;
      }
      if (ptr->num)
      {
         FreeTSD( ptr->num->num ) ;
         FreeTSD( ptr->num ) ;
         ptr->num = NULL ;
      }
   }
}

static void drop_var_reserved( const tsd_t *TSD, int ridx, const streng *name )
{
   ridx = ridx;
   drop_var_simple( TSD, name );
}

static void drop_var_stem( const tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"drop_var_stem:     ?"));
   ptr = findsimple( TSD, name ) ;

   vt->foundflag = 0 ;
   if (ptr)
   {
      vt->foundflag = ptr->flag & VFLAG_BOTH ;
      ptr->flag = VFLAG_NONE ;
      if (ptr->value)
      {
         Free_stringTSD( ptr->value ) ;
         ptr->value = NULL ;
      }
      if (ptr->num)
      {
         FreeTSD( ptr->num->num ) ;
         FreeTSD( ptr->num ) ;
         ptr->num = NULL ;
      }

      assert(ptr->index) ;
      if (ptr->index)
         kill_index( TSD, ptr->index, 0, 1, NULL ) ;
   }
}

static void drop_var_compound( const tsd_t *TSD, const streng *name )
{
   int hashv=0, baselength=1 ;
   variableptr ptr=NULL, nptr=NULL ;
   streng *indexstr=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"drop_var_compound: ?"));
   ptr = TSD->currlevel->vars[hashv=hashfunc(vt,name,0,&stop)] ;
   baselength = ++stop ;
   for (;(ptr)&&(Str_cncmp(ptr->name,name,baselength));ptr=ptr->next)
      ;
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;
   indexstr = subst_index( TSD, name, stop, TSD->currlevel->vars ) ;
   hashv = hashfunc(vt,indexstr,0,NULL) ;

   if (vt->subst && !vt->notrace)   /* trace it */
      tracecompound(TSD,name,baselength-1,indexstr,'C') ;

   if (ptr)
   {   /* find specific value */
      nptr = ((variableptr *)(ptr->index))[hashv] ;
      for (;(nptr)&&(Str_cmp(nptr->name,indexstr));nptr=nptr->next)
         ;
      for (;(nptr)&&(nptr->realbox);nptr=nptr->realbox)
         ;
   }

   vt->foundflag = ((ptr) && (nptr) && (nptr->flag & VFLAG_BOTH)) ;

   if ((ptr)&&(nptr))
   {
      nptr->flag = VFLAG_NONE ;
      if (nptr->value)
      {
         FreeTSD( nptr->value ) ;
         nptr->value = NULL ;
      }
      if (nptr->num)
      {
         FreeTSD( nptr->num->num ) ;
         FreeTSD( nptr->num ) ;
         nptr->num = NULL ;
      }
   }
   else
   {
#if 1  /* really MH */
      if (ptr)
      {
         /*
          * We are playing with the NULL-ptr ... take care !
          */
         setvalue_compound( TSD, name, NULL ) ;
      }
#else
      /*
       * We are playing with the NULL-ptr ... take care !
       */
      setvalue_compound( TSD, name, NULL ) ;
#endif
   }
}

/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from drop_var_compound().
 *   - Started using the global variable, vt->tmpindex, in place of the local,
 *     indexstr.
 *   - manually move the first stem name into vt->tmpindex, do not call
 *     subst_index(), as that not only uppercases the tail, but also
 *     does not uppercase the tail.
 *   - changed call from setvalue_compound() to setdirvalue_compound().
 *
 *
 ****************************************************************************/
static void drop_dirvar_compound( const tsd_t *TSD, const streng *name )
{
   int hashv=0, baselength=1 ;
   variableptr ptr=NULL, nptr=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"drop_dirvar_compound: ?"));
   ptr = TSD->currlevel->vars[hashv=hashfunc(vt,name,0,&stop)] ;
   baselength = ++stop ;
   for (;(ptr)&&(Str_cncmp(ptr->name,name,baselength));ptr=ptr->next)
      ;
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;
   /* indexstr = subst_index( TSD, name, stop, TSD->currlevel->vars ) ;  */
   /*  Use the global that is defined and allocated by init_vars()      */
   /*  Don't have to worry about freeing, or causing a memory leak.     */
   /*  It is also what the subst_index() would have had us using.       */
   vt->tmpindex->len = 0;
   vt->tmpindex = Str_nocatTSD(vt->tmpindex,name,name->len - stop,stop);
   hashv = hashfunc(vt,vt->tmpindex,0,NULL) ;

   if (vt->subst && !vt->notrace)   /* trace it */
      tracecompound(TSD,name,baselength-1,vt->tmpindex,'C') ;

   if (ptr)
   {   /* find specific value */
      nptr = ((variableptr *)(ptr->index))[hashv] ;
      for (;(nptr)&&(Str_cmp(nptr->name,vt->tmpindex));nptr=nptr->next)
         ;
      for (;(nptr)&&(nptr->realbox);nptr=nptr->realbox)
         ;
   }

   vt->foundflag = ((ptr) && (nptr) && (nptr->flag & VFLAG_BOTH)) ;

   if ((ptr)&&(nptr))
   {
      nptr->flag = VFLAG_NONE ;
      if (nptr->value)
      {
         FreeTSD( nptr->value ) ;
         nptr->value = NULL ;
      }
      if (nptr->num)
      {
         FreeTSD( nptr->num->num ) ;
         FreeTSD( nptr->num ) ;
         nptr->num = NULL ;
      }
   }
   else
   {
#if 1  /* really MH */
      if (ptr)
      {
         /*
          * We are playing with the NULL-ptr ... take care !
          */
         setdirvalue_compound( TSD, name, NULL ) ;
      }
#else
      /*
       * We are playing with the NULL-ptr ... take care !
       */
      setdirvalue_compound( TSD, name, NULL ) ;
#endif
   }
}

void drop_var( const tsd_t *TSD, const streng *name )
{
   int i;

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      drop_var_reserved( TSD, i, name );
   else
   {
      for (i=0; (i<Str_len(name))&&(name->value[i]!='.'); i++ )
         ;
      if (i==Str_len(name))
         drop_var_simple( TSD, name ) ;
      else if ((i+1)==Str_len(name))
         drop_var_stem( TSD, name ) ;
      else
         drop_var_compound( TSD, name ) ;
   }
}

/* JH 20-10-99 */  /* To make Direct setting of stems Direct and not Symbolic. */
/****************************************************************************
 *
 *  JH 13/12/1999 (Original code changes on 20/10/1999)
 *
 *  BUG022            To make Direct setting of stems Direct and not Symbolic.
 *   - Adapted from drop_var().  Changed call drop_var_compound() to
 *     drop_dirvar_compound().  *** May need to do the same for drop_var_stem(). ****
 *
 ****************************************************************************/
void drop_dirvar( const tsd_t *TSD, const streng *name )
{
   int i;

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      drop_var_reserved( TSD, i, name );
   else
   {
      for (i=0; (i<Str_len(name))&&(name->value[i]!='.'); i++ )
         ;
      if (i==Str_len(name))
         drop_var_simple( TSD, name ) ;
      else if ((i+1)==Str_len(name))
         drop_var_stem( TSD, name ) ;
      else
         drop_dirvar_compound( TSD, name ) ;
   }
}

static void upper_var_simple( tsd_t *TSD, const streng *name )
{
   variableptr ptr=NULL ;
   streng *value=NULL ;
   var_tsd_t *vt = TSD->var_tsd;

   ptr = findsimple( TSD, name) ;

   vt->foundflag = ((ptr)&&(ptr->flag & VFLAG_BOTH)) ;

   if (ptr)
   {
      /*
       * If its a number, don't try and uppercase it! TBD
       */
      expand_to_str( TSD, ptr );
   }

   if (vt->foundflag)
   {
      value = ptr->value ;
      Str_upper( value ) ;
   }
   else
   {
      vt->thespot = NULL ;
      if (!vt->ignore_novalue)
         condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name), NULL ) ;
   }

   if (!vt->notrace)
      tracevalue(TSD,value,(char) (((ptr) ? 'V' : 'L'))) ;

   DSTART;DPRINT((TSD,"upper_var_simple:   "));DNAME(TSD,"name",name);
          DVALUE(TSD," rc",value);DEND;
   return ;
}

static void upper_var_reserved( tsd_t *TSD, int ridx, const streng *name )
{
   ridx = ridx;
   upper_var_simple( TSD, name );
}

static void upper_var_compound( tsd_t *TSD, const streng *name )
{
   int hashv=0, baselength=0 ;
   variableptr ptr=NULL, nptr=NULL ;
   streng *value=NULL ;
   int stop=0 ;
   var_tsd_t *vt = TSD->var_tsd;

   DPRINTF((TSD,"upper_var_compound: ?"));
   /*  Get a good starting point, and find the stem/index separater.    */
   ptr = TSD->currlevel->vars[hashv=hashfunc(vt,name,0,&stop)] ;
   baselength = ++stop ;
   /*  Find the stem in the variable pool.                              */
   for (;(ptr)&&(Str_cncmp(ptr->name,name,baselength));ptr=ptr->next)
      ;
   /* Back up through the EXPOSE chain 'til get to the real variable.   */
   for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox)
      ;
   /* indexstr = subst_index( name, stop, TSD->currlevel->vars ) ;  */
   /*  Get the index name to use from the literal variable name.        */
   /*  Use the global that is defined and allocated by init_vars()      */
   /*  Don't have to worry about freeing, or causing a memory leak.     */
   /*  It is also what the subst_index() would have had us using.       */
   vt->tmpindex->len = 0;
   vt->tmpindex = Str_nocatTSD(vt->tmpindex,name,name->len - stop,stop);
   /*  Set up to look for this name in the stem's variable pool.        */
   hashv = hashfunc(vt,vt->tmpindex,0,NULL) ;

   if (vt->subst && !vt->notrace)   /* trace it */
      tracecompound(TSD,name,baselength-1,vt->tmpindex,'C') ;

   if (ptr)
   {   /* find specific value */
      /*  Get a good starting place for the index name.                 */
      nptr = ((variableptr *)(ptr->index))[hashv] ;
      /*  Find the index in the variable pool.                          */
      for (;(nptr)&&(Str_cmp(nptr->name,vt->tmpindex));nptr=nptr->next) ;
      /* Back up through the EXPOSE chain 'til get to the real variable.*/
      for (;(nptr)&&(nptr->realbox);nptr=nptr->realbox) ;
   }

   /*  If the stem exists, but the index doesn't, this counts as an error! */
   /* we shouldn't get here though!! */
   if ((ptr)&&(!nptr))   /* find default value */
      nptr = ptr ;

   vt->foundflag = (ptr)&&(nptr)&&(nptr->flag & VFLAG_BOTH) ;
   if (ptr && nptr)
   {
      /*
       * If its a number, don't try and uppercase it! TBD
       */
      expand_to_str( TSD, nptr );
   }
   if (vt->foundflag)
   {
      value = (nptr)->value ;
      Str_upper( value ) ;
   }
   else
   {
      if (!vt->ignore_novalue)
         condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(name), NULL ) ;
         /* condition_hook( TSD, SIGNAL_NOVALUE, 0, -1, Str_dupTSD(name) ) ;  JH 20-10-99  Dunno why it was like this! */

      if (vt->ovalue)
         Free_stringTSD( vt->ovalue ) ;
      /*
       * If we are not trapping NOVALUE, and the variable doesn't exist
       * then we don't have to do anything; the variable will be uppercase
       * by default.
       */
   }

   vt->thespot = NULL ;
   return ;
}

void upper_var( tsd_t *TSD, const streng *name )
{
   int i;

   if ( ( i = KNOWN_RESERVED( name->value, Str_len( name ) ) ) != 0 )
      upper_var_reserved( TSD, i, name );
   else
   {
      for (i=0; (i<Str_len(name))&&(name->value[i]!='.'); i++ )
         ;
      if (i==Str_len(name))
      {
         upper_var_simple( TSD, name ) ;
      }
      else if ((i+1)==Str_len(name))
      {
         exiterror( ERR_INVALID_STEM, 0 )  ;
      }
      else
      {
         upper_var_compound( TSD, name ) ;
      }
   }
}

void kill_variables( const tsd_t *TSD, variableptr *array )
{
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   DPRINTF((TSD,"kill_variables:    current_valid:old=%ld, new=%ld",
            vt->current_valid,(long) array[HASHTABLENGTH]));
   vt->current_valid = (long) array[HASHTABLENGTH] ;

   kill_index( TSD, array, 1, 0, NULL ) ;

   if (vt->current_valid == 1)
      vt->next_current_valid = 2 ;
   assert(vt->current_valid) ;
}

/*
 * This is the shortcut method for retrieving the value of a variable.
 * It requires you to have a nodeptr, which may contain a shortcut
 * pointer into the variable pool. Unless, such a shortcut pointer is
 * established, if possible.
 */
const streng *shortcut( tsd_t *TSD, nodeptr this )
{
   const streng *result=NULL ;
   char ch=' ' ;
   variableptr vptr=NULL ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DSTART;DPRINT((TSD,"shortcut:          "));DNAME(TSD,"this->name",this->name);DEND;
   if ((vptr=this->u.varbx)!=NULL)
   {
      if (vptr->valid==vt->current_valid)
      {
         DSTART;DPRINT((TSD,"shortcut:          "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         ch = 'V' ;
         for (;vptr && vptr->realbox; vptr=vptr->realbox) ;
         if (vptr->flag & VFLAG_STR)
            result = vptr->value ;
         else if (vptr->flag & VFLAG_NUM)
         {
            expand_to_str( TSD, vptr );
            result = vptr->value ;
         }
         else
         {
            ch = 'L' ;
            result = vptr->name ;
            if (!vt->ignore_novalue)
               condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(result), NULL ) ;
         }
         DSTART;DPRINT((TSD,"shortcut:          "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on end"));DEND;

         if (TSD->trace_stat=='I')
            tracevalue( TSD, result, ch ) ;

         assert( !result || result->len <= result->max ) ;
         DSTART;DPRINT((TSD,"shortcut:          "));DVALUE(TSD,"rc",result);DEND;
         return result ;
      }
      else
      {
         DSTART;DPRINT((TSD,"shortcut:          "));DVAR(TSD,"INVALID vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         if (--(vptr->hwired)==0)
            if (!vptr->valid)
            {
#ifdef TRACEMEM
               if (vptr->prev)
                  vptr->prev->next = vptr->next ;
               if (vptr->next)
                  vptr->next->prev = vptr->prev ;
               else
                  vt->first_invalid = vptr->prev ;
#endif
               FreeTSD( vptr ) ;
            }
         this->u.varbx = NULL ;
      }
   }

   result = getvalue( TSD, this->name, 1 ) ;
   if (vt->thespot /*&& this->type==X_SIM_SYMBOL */)
   {
      vt->thespot->hwired++ ;
      this->u.varbx = vt->thespot ;
   }

   DSTART;DPRINT((TSD,"shortcut:          "));DVAR(TSD,"new vt->thespot",vt->thespot);DEND;
   DSTART;DPRINT((TSD,"shortcut:          "));DVALUE(TSD,"rc",result);DEND;
   assert( !result || result->len <= result->max ) ;
   return result ;
}

num_descr *shortcutnum( tsd_t *TSD, nodeptr this )
{
   variableptr vptr=NULL ;
   num_descr *result=NULL ;
   const streng *resstr=NULL ;
   char ch=' ' ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DSTART;DPRINT((TSD,"shortcutnum:       "));DNAME(TSD,"this->name",this->name);DEND;
   if ((vptr=this->u.varbx)!=NULL)
   {
      if (vptr->valid==vt->current_valid)
      {
         DSTART;DPRINT((TSD,"shortcutnum:       "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         for(; vptr && vptr->realbox; vptr=vptr->realbox) ;
         ch = 'V' ;
         if (vptr->flag & VFLAG_NUM)
         {
            result = vptr->num ;
            if (TSD->trace_stat=='I')
               tracenumber( TSD, result, 'V' ) ;
         }
         else if (vptr->flag & VFLAG_STR)
         {
            if (vptr->num)
            {
               FreeTSD( vptr->num->num ) ;
               FreeTSD( vptr->num ) ;
            }
            if (TSD->trace_stat=='I')
               tracevalue( TSD, vptr->value, 'V' ) ;
            vptr->num = is_a_descr( TSD, vptr->value ) ;
            if (vptr->num)
               vptr->flag |= VFLAG_NUM ;
            result = vptr->num ;
         }
         else
         {
            result = NULL ;
            if (TSD->trace_stat=='I')
               tracevalue( TSD, this->name, 'L' ) ;
            if (!vt->ignore_novalue)
               condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(this->name), NULL ) ;
         }
         DSTART;DPRINT((TSD,"shortcutnum:       "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on end"));DEND;
         DSTART;DPRINT((TSD,"shortcutnum:       "));DNUM(TSD,"rc",result);DEND;
         return result ;
      }
      else
      {
         DSTART;DPRINT((TSD,"shortcutnum:       "));DVAR(TSD,"INVALID vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         if (--(vptr->hwired)==0)
            if (!vptr->valid)
            {
#ifdef TRACEMEM
               if (vptr->prev)
                  vptr->prev->next = vptr->next ;
               if (vptr->next)
                  vptr->next->prev = vptr->prev ;
               else
                  vt->first_invalid = vptr->prev ;
#endif
               FreeTSD( this->u.varbx ) ;
            }
         this->u.varbx = NULL ;
      }
   }

   resstr = getvalue( TSD, this->name, 1 ) ;
   if (vt->thespot)
   {
      vt->thespot->hwired++ ;
      this->u.varbx = vt->thespot ;
      if (vt->thespot->num)
      {
         if (vt->thespot->flag & VFLAG_NUM)
            return vt->thespot->num ;
         FreeTSD(vt->thespot->num->num) ;
         FreeTSD(vt->thespot->num) ;

      }
      vt->thespot->num = is_a_descr( TSD, resstr ) ;
      if (vt->thespot->num)
         vt->thespot->flag |= VFLAG_NUM ;
   }
   else
   {
      if (vt->odescr) /* FIXME: FGC: Is this ALWAYS allowed? */
      {
         FreeTSD( vt->odescr->num ) ;
         FreeTSD( vt->odescr ) ;
      }
      vt->odescr = is_a_descr( TSD, resstr ) ;
      DSTART;DPRINT((TSD,"shortcutnum:       "));DVALUE(TSD,"NO!!! vt->thespot, resstr",resstr);DEND;
      DSTART;DPRINT((TSD,"shortcutnum:       "));DNUM(TSD,"rc",vt->odescr);DEND;
      return vt->odescr ;
   }
   DSTART;DPRINT((TSD,"shortcutnum:       "));DVAR(TSD,"new vt->thespot",vt->thespot);DEND;
   DSTART;DPRINT((TSD,"shortcutnum:       "));DNUM(TSD,"rc",vt->thespot->num);DEND;

   return( vt->thespot->num ) ;
}

void setshortcut( const tsd_t *TSD, nodeptr this, streng *value )
{
   variableptr vptr=NULL ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   assert( !value || value->len <= value->max ) ;
   DSTART;DPRINT((TSD,"setshortcut:       "));DNAME(TSD,"this->name",this->name);
          DVALUE(TSD,", value",value);DEND;
   if ((vptr=this->u.varbx)!=NULL)
   {
      if (vptr->valid==vt->current_valid)
      {
         DSTART;DPRINT((TSD,"setshortcut:       "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         for(; vptr && vptr->realbox; vptr=vptr->realbox) ;
         if (vptr->value)
            Free_stringTSD(vptr->value) ;
         if (vptr->num)
         {
            FreeTSD( vptr->num->num ) ;
            FreeTSD( vptr->num ) ;
            vptr->num = 0 ;
         }
         vptr->flag = value ? VFLAG_STR : VFLAG_NONE ;
         vptr->value = value ;
         DSTART;DPRINT((TSD,"setshortcut:       "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on end"));DEND;
         return ;
      }
      else
      {
         DSTART;DPRINT((TSD,"setshortcut:       "));DVAR(TSD,"INVALID vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         if (--(vptr->hwired)==0)
            if (!vptr->valid)
            {
#ifdef TRACEMEM
               if (vptr->prev)
                  vptr->prev->next = vptr->next ;
               if (vptr->next)
                  vptr->next->prev = vptr->prev ;
               else
                  vt->first_invalid = vptr->prev ;
#endif
               FreeTSD( this->u.varbx ) ;
            }
         this->u.varbx = NULL ;
      }
   }

   setvalue( TSD, this->name, value ) ;
   if (vt->thespot)
   {
      vt->thespot->hwired++ ;
      this->u.varbx = vt->thespot ;
   }
   DSTART;DPRINT((TSD,"setshortcut:       "));DVAR(TSD,"vt->thespot",vt->thespot);
          DPRINT((TSD," on end"));DEND;
   return ;
}

/*
 * sets the varbox in this to the given value, creating it if it doesn't
 * exists. this->name is set to the string-representation of value; if
 * string_val is non-NULL, string_val is used instead. string_val must not
 * be used later.
 */
void setshortcutnum( const tsd_t *TSD, nodeptr this, num_descr *value,
                     streng *string_val )
{
   variableptr vptr=NULL ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   assert( value->size ) ;

   DSTART;DPRINT((TSD,"setshortcutnum:    "));DNAME(TSD,"this->name",this->name);
          DNUM(TSD,", value",value);DEND;
   if ((vptr=this->u.varbx)!=NULL)
   {
      if (vptr->valid==vt->current_valid)
      {
         DSTART;DPRINT((TSD,"setshortcutnum:    "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         for(; vptr && vptr->realbox; vptr=vptr->realbox) ;
         if (vptr->num)
         {
            FreeTSD(vptr->num->num) ;
            FreeTSD(vptr->num ) ;
         }
         if (vptr->value)
         {
            Free_stringTSD( vptr->value ) ;
            vptr->value = NULL ;
         }
         vptr->flag = value ? VFLAG_NUM : VFLAG_NONE ;
         vptr->num = value ;
         DSTART;DPRINT((TSD,"setshortcutnum:    "));DVAR(TSD,"valid vptr",vptr);
                DPRINT((TSD," on end"));DEND;
         return ;
      }
      else
      {
         DSTART;DPRINT((TSD,"setshortcutnum:    "));DVAR(TSD,"INVALID vptr",vptr);
                DPRINT((TSD," on start"));DEND;
         if (--(vptr->hwired)==0)
            if (!vptr->valid)
            {
#ifdef TRACEMEM
               if (vptr->prev)
                  vptr->prev->next = vptr->next ;
               if (vptr->next)
                  vptr->next->prev = vptr->prev ;
               else
                  vt->first_invalid = vptr->prev ;
#endif
               FreeTSD( this->u.varbx ) ;
             }
         this->u.varbx = NULL ;
      }
   }

   if ( string_val == NULL )
      string_val = str_norm( TSD, value, NULL );
   setvalue( TSD, this->name, string_val );
   if (vt->thespot)
   {
      vt->thespot->hwired++ ;
      if (value)
      {
         if (vt->thespot->num)
         {
            FreeTSD( vt->thespot->num->num ) ;
            FreeTSD( vt->thespot->num ) ;
         }
         vt->thespot->num = value ;
         vt->thespot->flag |= VFLAG_NUM ;
      }
      this->u.varbx = vt->thespot ;
   }
   else
   {
      FreeTSD( value->num ) ;
      FreeTSD( value ) ;
   }
   DSTART;DPRINT((TSD,"setshortcutnum:    "));DVAR(TSD,"vt->thespot",vt->thespot);DEND;
   DSTART;DPRINT((TSD,"setshortcutnum:    "));DVAR(TSD,"this->u.varbx",this->u.varbx);
          DPRINT((TSD," on end"));DEND;
   return ;
}

streng *fix_compound( tsd_t *TSD, nodeptr this, streng *new )
{
   variableptr iptr=NULL, ptr=NULL ;
   streng *value=NULL ;
   streng *indeks=NULL ;
   int hhash=0, thash=0 ;
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   DPRINTF((TSD,"fix_compound:      ?"));
   value = NULL ;
   hhash = -400000 ;   /* Intentionally erroneous */

   assert( !new || new->len <= new->max ) ;

   iptr = this->u.varbx ;
   if (iptr)
   {
      if (iptr->valid!=vt->current_valid)
      {
         if ((--iptr->hwired==0) && !iptr->valid)
         {
#ifdef TRACEMEM
            if (iptr->prev)
               iptr->prev->next = iptr->next ;
            if (this->u.varbx->next)
               iptr->next->prev = iptr->prev ;
            else
               vt->first_invalid = iptr->prev ;
#endif
            FreeTSD( iptr ) ;
         }
         iptr = this->u.varbx = NULL ;
      }
   }

   if (!iptr)
   {
      iptr = TSD->currlevel->vars[hhash=hashfunc(vt,this->name,0,NULL)] ;
      /* should this use Str_ccmp() ??? MH */
      for (;(iptr)&&(Str_cmp(iptr->name,this->name));iptr=iptr->next) ;
      for (;(iptr)&&(iptr->realbox);iptr=iptr->realbox) ;

      if (iptr)
      {
         this->u.varbx = iptr ;
         iptr->hwired++ ;
      }
      if (!iptr && new && this->p[0])
      {
         setvalue_simple( TSD, this->name, NULL ) ;
         iptr = vt->thespot ;
         iptr->index = make_hash_table( TSD ) ;
         DPRINTF((TSD,"make_hash_table:   rc=%p",iptr->index));
      }
   }

   assert( this->p[0] ) ;
   indeks = fix_index( TSD, this->p[0] ) ;

   if (vt->subst)
      tracecompound( TSD, this->name, this->name->len-1, indeks, 'C' ) ;

   if (iptr)
   {
      ptr = iptr->index[thash=hashfunc(vt,indeks,0,NULL)] ;
      for (;(ptr)&&(Str_cmp(ptr->name,indeks));ptr=ptr->next) ;
      for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox) ;

      if (new)
      {
         vt->foundflag = (ptr!=NULL) ;
         if (vt->foundflag)
            REPLACE_VALUE( new, ptr )
         else
         {
            newbox( TSD, indeks, new, &iptr->index[thash]) ;
            iptr->index[thash]->stem = iptr ;
         }
      }
      else
      {
         vt->foundflag = ptr && (ptr->flag & VFLAG_BOTH) ;
         if (ptr)
         {
            if (ptr->flag & VFLAG_STR)
               value = ptr->value ;
            else if (ptr->flag & VFLAG_NUM)
            {
               expand_to_str( TSD, ptr );
               value = ptr->value ;
            }
            else
               goto the_default ;
         }
         else if (iptr->flag & VFLAG_STR)
            value = iptr->value ;
         else if (iptr->flag & VFLAG_NUM)
         {
            expand_to_str( TSD, iptr );
            value = ptr->value ;
         }
         else
            goto the_default ;

         tracevalue( TSD, value, 'V' ) ;
      }
   }
   else
   {
      if (new)
      {
         iptr = newbox( TSD, this->name, NULL, &(TSD->currlevel->vars[hhash])) ;
         iptr->index = make_hash_table( TSD ) ;
         DPRINTF((TSD,"make_hash_table:   rc=%p",iptr->index));
         thash = hashfunc(vt,indeks,0,NULL) ;
         newbox( TSD, indeks, new, &(iptr->index[thash])) ;
         iptr->index[thash]->stem = iptr ;
      }
      else
      {
the_default:
         if (vt->xvalue)
            Free_stringTSD( vt->xvalue ) ;
         vt->xvalue = Str_makeTSD( this->name->len + indeks->len ) ;
         vt->xvalue = Str_catTSD( vt->xvalue, this->name ) ;
         vt->xvalue = Str_catTSD( vt->xvalue, indeks ) ;
         tracevalue( TSD, vt->xvalue, 'L' ) ;
         if (!vt->ignore_novalue)
            condition_hook( TSD, SIGNAL_NOVALUE, 0, 0, -1, Str_dupTSD(vt->xvalue), NULL ) ;
         value = vt->xvalue ;
      }
   }

   assert( !value || value->len <= value->max ) ;
   return value ;
}

/*
 * queries or sets the varbox in this depending on the presence of new. If
 * new is set, varbox in this is set to the given value, creating it if it
 * doesn't exists. this->name is set in this case to the string-representation
 * of value; if string_val is non-NULL, string_val is used instead.
 * string_val must not be used later.
 * The current or new value of the variable is returned in all cases, it may
 * be NULL.
 */
num_descr *fix_compoundnum( tsd_t *TSD, nodeptr this, num_descr *new,
                            streng *string_val )
{
   variableptr iptr, ptr;
   num_descr *value;
   streng *indeks;
   int hhash, thash;
   var_tsd_t *vt;

   vt = TSD->var_tsd;
   DPRINTF((TSD,"fix_compoundnum:   ?"));
   value = NULL ;
   thash = hhash = -400000 ;   /* Intentionally erroneous */

   iptr = this->u.varbx ;
   if (iptr)
   {
      if (iptr->valid!=vt->current_valid)
      {
         if ((--iptr->hwired==0) && !iptr->valid)
         {
#ifdef TRACEMEM
            if (iptr->prev)
               iptr->prev->next = iptr->next ;
            if (this->u.varbx->next)
               iptr->next->prev = iptr->prev ;
            else
               vt->first_invalid = iptr->prev ;
#endif
            FreeTSD( iptr ) ;
         }
         iptr = this->u.varbx = NULL ;
      }
   }

   if (!iptr)
   {
      iptr = TSD->currlevel->vars[hhash=hashfunc(vt,this->name,0,NULL)] ;
      /* should this use Str_ccmp() ??? MH */
      for (;(iptr)&&(Str_cmp(iptr->name,this->name));iptr=iptr->next) ;
      for (;(iptr)&&(iptr->realbox);iptr=iptr->realbox) ;

      if (iptr)
      {
         this->u.varbx = iptr ;
         iptr->hwired++ ;
      }
      if (!iptr && new && this->p[0])
      {
         setvalue_simple( TSD, this->name, NULL );
         iptr = vt->thespot ;
         iptr->index = make_hash_table( TSD ) ;
         DPRINTF((TSD,"make_hash_table:   rc=%p",iptr->index));
      }
   }

   assert( this->p[0] ) ;
   indeks = fix_index( TSD, this->p[0] ) ;

   if (vt->subst)
      tracecompound( TSD, this->name, this->name->len-1, indeks, 'C' ) ;

   if (iptr)
   {
      ptr = iptr->index[thash=hashfunc(vt,indeks,0,NULL)] ;
      for (;(ptr)&&(Str_cmp(ptr->name,indeks));ptr=ptr->next) ;
      for (;(ptr)&&(ptr->realbox);ptr=ptr->realbox) ;

      if (new)
      {
         vt->foundflag = (ptr!=NULL) ;
         if (vt->foundflag)
            REPLACE_NUMBER( new, ptr )
         else
         {
            newbox( TSD, indeks, NULL, &iptr->index[thash]) ;
            ptr = iptr->index[thash];
            ptr->stem = iptr ;
            ptr->num = new ;
            ptr->flag = VFLAG_NUM;
         }
         if ( ptr->value != NULL )
            Free_stringTSD( ptr->value );

         if ( string_val != NULL )
            ptr->flag |= VFLAG_STR;
         else
            ptr->flag &= ~VFLAG_STR;
         ptr->value = string_val;
      }
      else
      {
         vt->foundflag = ptr && (ptr->flag & VFLAG_BOTH) ;
         if (ptr)
         {
            if (ptr->flag & VFLAG_NUM)
            {
               value = ptr->num ;
               tracenumber( TSD, value, 'V' ) ;
            }
            else if (ptr->flag & VFLAG_STR)
            {
               if (ptr->num)
               {
                  FreeTSD( ptr->num->num ) ;
                  FreeTSD( ptr->num ) ;
               }
               ptr->num = is_a_descr( TSD, ptr->value ) ;
               if ((value=ptr->num)!=NULL)
               {
                  tracevalue( TSD, ptr->value, 'V' ) ;
                  ptr->flag |= VFLAG_NUM ;
               }
            }
            else
               goto the_default ;
         }
         else if (iptr->flag & VFLAG_NUM)
         {
            value = iptr->num ;
            tracenumber( TSD, value, 'V' ) ;
         }
         else if (iptr->flag & VFLAG_STR)
         {
            if (iptr->num)
            {
               FreeTSD( iptr->num->num ) ;
               FreeTSD( iptr->num ) ;
            }
            iptr->num = is_a_descr( TSD, iptr->value ) ;
            if ((value=iptr->num)!=NULL)
            {
               iptr->flag |= VFLAG_NUM ;
               tracevalue( TSD, iptr->value, 'V' ) ;
            }
         }
         else
            goto the_default ;
      }

   }
   else
   {
      if (new)
      {
         /*
          * Happens on:
          * a) first access
          * b) left part of X_NASSIGN
          * c) this->p[0] is NULL
          * according to this function, interprt.c and yaccsrc.y.
          *
          * We have to be a X_HEAD_SYMBOL. X_HEAD_SYMBOL are created by
          * create_head in yaccsrc.y only if this->p[0] != NULL.
          *
          * --> This code will never be executed!
          * Reenable this only if you check the presence of stringval and
          * know what you do.
          */
#if 1
         fprintf( stderr, "Regina internal error detected in %s, line %u.\n"
                          "Please, send an email to M.Hessling@qut.edu.au.\n",
                          __FILE__, __LINE__ );
      }
#else
         iptr = newbox( TSD, this->name, NULL, &(TSD->currlevel->vars[hhash])) ;
         iptr->index = make_hash_table( TSD ) ;
         DPRINTF((TSD,"make_hash_table:   rc=%p",iptr->index));
         thash = hashfunc(vt,indeks,0,NULL) ;
         newbox( TSD, indeks, NULL, &(iptr->index[thash])) ;
         ptr = iptr->index[thash];
         ptr->stem = iptr ;
         ptr->num = new ;
         ptr->flag = VFLAG_NUM ;
         if ( string_val != NULL )
            ptr->flag |= VFLAG_STR;
         ptr->value = string_val;
      }
      else
#endif
      {
the_default:
         tracecompound( TSD, this->name, this->name->len-1, indeks, 'L' ) ;
         return NULL ;
      }
   }
   return value ;
}

/*
 * get_realbox returns either p or the realbox associated with p if it exists.
 * This function is NULL-pointer safe.
 */
static variableptr get_realbox( variableptr p )
{
   if ( p == NULL )
      return p;
   if ( p->realbox == NULL )
      return p;
   for ( p = p->realbox; p->realbox; p = p->realbox )
      ;
   return p;
}

/*
 * get_next_variable either initializes the iteration system (reset != 0) or
 * it returns the next variable of the variable's array. We have to do some
 * extra housekeeping to become reentrant in each call.
 *
 * NULL is returned in initializing or after all variables are processed.
 *
 * This function returns the variables of the current PROCEDURE frame only
 * and has a similar function dumpvars in debug.c.
 */
variableptr get_next_variable( tsd_t *TSD, int reset )
{
   variableptr retval;
   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DPRINTF((TSD,"get_next_variable: ?"));
   if ( reset )
   {
      /*
       * Initializes the 4 loop variables and returns NULL.
       * rstem and rtail are the corresponding "realbox" shadows of pstem
       * and ptail. This fixes bug 681991.
       */
      vt->pstem = vt->ptail = NULL;
      vt->stemidx = vt->tailidx = 0;
      vt->rstem = vt->rtail = NULL;
      return NULL;
   }

   /*
    * We have to do some tricks to change the four nested loops (see dumpvars)
    * into a reentrant loop system. The loop itself is identical to that one
    * of dumpvars.
    */
   for ( ; ; )
   {
      /*
       * While the masterindex stemidx isn't out of range.
       */
      if ( vt->rstem )
      {
         /*
          * Something's left in the masterindex's iterator pstem.
          */
         if ( vt->rstem->index )
         {
            /*
             * This works as the outer loop pair, but for stem variables.
             */
            for ( ; ; )
            {
               if ( vt->rtail )
               {
                  retval = vt->rtail;
                  vt->ptail = vt->ptail->next;
                  vt->rtail = get_realbox( vt->ptail );
                  return retval;
               }

               if ( vt->tailidx >= HASHTABLENGTH )
                  break;

               vt->ptail = vt->rstem->index[vt->tailidx++];
               vt->rtail = get_realbox( vt->ptail );
            }
         }

         vt->ptail = vt->rtail = NULL;
         vt->tailidx = 0;

         retval = vt->rstem;
         vt->pstem = vt->pstem->next;
         vt->rstem = get_realbox( vt->pstem );
         return retval;
      }

      if ( vt->stemidx >= HASHTABLENGTH )
         break;

      vt->pstem = TSD->currlevel->vars[vt->stemidx++];
      vt->rstem = get_realbox( vt->pstem );
      vt->ptail = NULL;
      vt->tailidx = 0;
   }

   return NULL;
}

#if 0
/* this was an attempt to mimic the behaviour of Object Rexx stem
 * assignment, but proved non-ANSI complient, so was dropped. I leave
 * it here so I can remember how to work with the variable pool ;-)
 */
void copy_stem( const tsd_t *TSD, nodeptr dststem, cnodeptr srcstem )
{
   /*
    * Drop dststem
    * set default value of dststem to default value of srcstem
    * for each valid stem of srcstem, set dststem value to src value
    */
   register variableptr ptr=NULL ;
#if 0
   register variableptr tptr=NULL ;
   register int j;
   streng *newname;
#endif

   var_tsd_t *vt;

   vt = TSD->var_tsd;

   DPRINTF((TSD,"copy_stem:         ?"));
   drop_var( dststem->name );
   ptr = findsimple( TSD, srcstem->name );
   if ( ptr )
   {
      if ( ptr->value )
      {
         /*
          * The srcstem has a default value, so set the dststem's
          * default value to this...
          */
         setvalue_stem( TSD, dststem->name, ptr->value );
      }
      else
      {
         /*
          * The srcstem does not have a default value, so set the dststem's
          * default value to the name of the srcstem...
          */
         setvalue_stem( TSD, dststem->name, ptr->name );
      }
#if 0
      /*
       * THE following code copies all explicitly set variables in the srcstem
       * to the equivalent dststem compound values, but this is NOT the way
       * that the ANSI standard states the behaviour should be :-(
       *
       * Find each variable for srcstem, and set dststem equivalents...
       */
      if (ptr->index)
      {
         for (j=0;j<HASHTABLENGTH;j++)
         {
            if ((tptr=((ptr->index))[j])!=NULL)
            {
               for (;tptr;tptr=tptr->next)
               {
                  if (tptr->name)
                  {
                     newname = Str_makeTSD( Str_len( dststem->name ) + 1 + Str_len( tptr->name ) ) ;
                     Str_ncpyTSD( newname, dststem->name, Str_len( dststem->name) ) ;
                     Str_catTSD( newname, tptr->name ) ;
                     if (tptr->value)
                     {
                        setvalue_compound( TSD, newname, tptr->value );
                     }
                     else
                     {
                     /*
                      * If the srcstem compund variable was dropped,
                      * then to make the destination compund variable
                      * also "appear" to be dropped, call the following
                      * line, otherwise leave it excluded.
                      */
#if 0
                        setvalue_compound( TSD, newname, NULL );
#endif
                     }
                     Free_stringTSD( newname );
                  }
               }
            }
         }
      }
#endif
   }
   else
   {
      /*
       * The source stem doesn't exist, so set the default value of
       * dststem to the name of the srcstem.
       */
      setvalue_stem( TSD, dststem->name, srcstem->name );
   }
}
#endif
