#ifndef lint
static char *RCSid = "$Id: debug.c,v 1.7 2003/03/11 10:38:08 florian Exp $";
#endif

/*
 *  The Regina Rexx Interpreter
 *  Copyright (C) 1992-1994  Anders Christensen <anders@pvv.unit.no>
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "rexx.h"

#ifndef NDEBUG

/*
 * Prints the value of v to fp. ", number ???" may be appended if v is a number.
 * In addition, the helper fields are printed and the line is terminated.
 */
void dumpvarcontent( const tsd_t *TSD, FILE *fp, cvariableptr v, int exposed )
{
   const streng *s;
   const num_descr *n;

   s = v->value;
   if ( s )
   {
      fprintf( fp, "\"%.*s\"", s->len, s->value );
   }
   else
   {
      fprintf( fp, "<none>" );
   }

   n = v->num;
   fprintf( fp, ",\tnumber " );
   if ( n ) /* variable is a number */
   {
      fprintf( fp, "%s0.%.*sE%+d",
               ( n->negative ) ? "-" : "",
               n->size, n->num, n->exp );
   }
   else
   {
      fprintf( fp, "<none>" );
   }

   switch ( v->flag )
   {
      case VFLAG_NONE: fprintf( fp, ",\tflag NONE, " ); break;
      case VFLAG_STR:  fprintf( fp, ",\tflag STR,  " );  break;
      case VFLAG_NUM:  fprintf( fp, ",\tflag NUM,  " );  break;
      case VFLAG_BOTH: fprintf( fp, ",\tflag BOTH, " ); break;
      default:         fprintf( fp, ",\tflag %d, ", v->flag );
   }
   fprintf( fp, "hwired %ld, valid %ld, guard %d%s\n",
                v->hwired, v->valid, v->guard,
                ( exposed ) ? ", exposed" : "" );
}

/*
 * get_realbox returns either p or the realbox associated with p if it exists.
 * This function is NULL-pointer safe.
 * *exposed is set to 0 if p is set in the current frame, it is set to 1 if p
 * is an exposed value from one of the upper frames.
 */
static cvariableptr get_realbox( cvariableptr p, int *exposed )
{
   *exposed = 0;
   if ( p == NULL )
      return p;
   if ( p->realbox == NULL )
      return p;

   *exposed = 1;
   for ( p = p->realbox; p->realbox; p = p->realbox )
      ;
   return p;
}

/*
 * dumpvars dumps the set of valid variables of the current PROCEDURE frame.
 * The destination is stderr of stdout in case of STDOUT_FOR_STDERR.
 */
void dumpvars( const tsd_t *TSD )
{
   cvariableptr ptr,tptr,rb,trb;
   int i,j,isstem,isexposed;
   FILE *fp;
   streng *s;
   cvariableptr *hashptr;

   fp = stderr;
   if ( get_options_flag( TSD->currlevel, EXT_STDOUT_FOR_STDERR ) )
      fp = stdout;

   hashptr = (cvariableptr *) TSD->currlevel->vars;

   fprintf( fp, "\nDumping variables, 1. no after \">>>\" is the bin number\n" );
   for ( i = 0; i != HASHTABLENGTH; i++ )
   {
      if ( hashptr[i] == NULL )
         continue;

      /*
       * One bin of same hashvalues may have several vars connected by a
       * simple linked list.
       */
      for ( ptr = hashptr[i]; ptr != NULL; ptr = ptr->next )
      {
         rb = get_realbox( ptr, &isexposed );
         s = rb->name;
         isstem = s->value[s->len - 1] == '.';

         fprintf( fp, "   >>> %3d %s \"%.*s\",\tvalue ",
                      i, ( isstem ) ? "    Stem" : "Variable",
                      s->len, s->value );

         dumpvarcontent( TSD, fp, rb, isexposed );

         if ( !isstem )
            continue;

         for ( j = 0; j < HASHTABLENGTH; j++ )
         {
            /*
             * The variables of a stem are organized as a normal variable
             * bunch. We have to iterate as for the level's variable set.
             * Keep in mind that a variable "a.b." isn't a stem, we can't
             * iterate once more.
             */
            if ( ( tptr = rb->index[j] ) != NULL )
            {
               for ( ; tptr; tptr = tptr->next )
               {
                  trb = get_realbox( tptr, &isexposed );
                  s = trb->name;
                  if ( s )
                  {
                     fprintf( fp, "      >>> %3d  Tail \"%.*s\",\tvalue ",
                                  j, s->len, s->value );
                     dumpvarcontent( TSD, fp, trb, isexposed );
                  }
               }
            }
         }
      }
   }

   return;
}


void dumptree(const tsd_t *TSD, const treenode *this, int level, int newline)
{
   int i=0, j=0 ;
   streng *ptr=NULL;
   FILE *fp=stderr;

   if ( get_options_flag( TSD->currlevel, EXT_STDOUT_FOR_STDERR ) )
      fp = stdout;
   if ((this->charnr)!=0
   &&  (this->charnr)!=(-1))
   {
      if (newline)
         putc('\n',fp);
      for (i=0;i!=level;i++) fprintf(fp,"  ") ;
      fprintf(fp,"Lineno: %d   Charno: %d", this->lineno, this->charnr) ;
      if (newline)
      {
         ptr = getsourceline(TSD, this->lineno, this->charnr,
                             &TSD->systeminfo->tree) ;
         fprintf(fp," Sourceline: [");
         for(i=0;i<ptr->len;i++)
            putc(ptr->value[i],fp);
         putc(']',fp);
      }
      putc('\n',fp);
   }

   for (i=0;i!=level;i++)
      fprintf(fp,"  ") ;
   fprintf(fp,">>>  in type=%d == %s\n", this->type, getsym(this->type)) ;

   for (i=0;i!=level;i++) fprintf(fp,"  ") ;
   fprintf(fp,"Flags: lnum %d rnum %d lsvar %d rsvar %d lcvar %d rcvar %d\n",
      this->u.flags.lnum,
      this->u.flags.rnum,
      this->u.flags.lsvar,
      this->u.flags.rsvar,
      this->u.flags.lcvar,
      this->u.flags.rcvar );

   if ((this->name)!=NULL)
   {
      for (i=0;i!=level;i++) fprintf(fp,"  ") ;
      fprintf(fp,"Name: [");
      for (i=0;i<this->name->len;i++)
         putc(this->name->value[i],fp);
      fprintf(fp,"]\n") ;
   }

   for (j=0;j<sizeof(this->p)/sizeof(this->p[0]);j++)
      if (this->p[j]!=NULL)
      {
         for (i=0;i!=level;i++)
            fprintf(fp,"  ") ;
         fprintf(fp,"==> (%d) going down in branch %d, type %d = %s\n",
                this->type,j+1,this->p[j]->type,getsym(this->p[j]->type)) ;
         dumptree( TSD, this->p[j], level+1, 0 ) ;
      }

   for (i=0;i!=level;i++)
      fprintf(fp,"  ") ;
   fprintf(fp,"<<< out type=%d == %s\n", this->type,getsym(this->type)) ;

   if (this->next)
      dumptree( TSD, this->next, level, 1 ) ;

}

#endif /* !NDEBUG */


#ifdef TRACEMEM
void marksource( clineboxptr ptr )
{
   for (;ptr;ptr=ptr->next) {
      markmemory( ptr->line,TRC_SOURCEL ) ;
      markmemory( (char *)ptr, TRC_SOURCE ) ; }
}
#endif


static const char *sourceline( int line, const internal_parser_type *ipt, unsigned *size)
{
   clineboxptr first;
   const otree *otp;

   if (ipt->first_source_line == NULL)
   { /* must be incore_source but that value may be NULL because of a failed
      * instore[0] of RexxStart!
      */
      otp = ipt->srclines; /* NULL if incore_source==NULL */
      while (otp && (otp->num < (unsigned long) line)) {
         line -= otp->num;
         otp = otp->next;
      }
      if (otp == NULL)
      {
         *size = 0 ;
         return NULL ;
      }
      line--;
      *size = otp->elems[line].length ;
      return ipt->incore_source + otp->elems[line].offset ;
   }
   first = ipt->first_source_line;
   for (;first;)
   {
      if (first->lineno==line)
      {
         *size = first->line->len ;
         return first->line->value ;
      }
      else
         first = (first->lineno<line) ? first->next : first->prev ;
   }

   *size = 0 ;
   return NULL ;
}



streng *getsourceline( const tsd_t *TSD, int line, int charnr, const internal_parser_type *ipt )
{
   int dquote=0, squote=0 ;
   unsigned len ;
   streng *string ;
   const char *ptr, *chptr, *chend, *tmptr ;
   char *outptr ;

   assert( charnr>=0 ) ;
   if (!charnr)
     charnr++ ;

   ptr = sourceline(line,ipt,&len) ;
/*   assert( ptr ) ; */
   if (!ptr || (charnr >= (int) len))
      return nullstringptr() ;

   chptr = ptr + --charnr ;
   chend = ptr + len ;
   for (; (chptr < chend) && isspace(*chptr); chptr++) ;
   string = Str_makeTSD(BUFFERSIZE+1) ;
   outptr = string->value ;

   for (;;)
   {
restart:
      if (chptr>=chend || outptr >= string->value + BUFFERSIZE)
         break ;

      if (!squote && *chptr=='\"')
         dquote = !dquote ;

      else if (!dquote && *chptr=='\'')
         squote = !squote ;

      else if (!(dquote || squote))
      {
         switch (*chptr)
         {
            case ',':
               for(tmptr=chptr+1; tmptr<chend && isspace(*tmptr); tmptr++ ) ;
               assert( tmptr<=chend ) ;
               if (tmptr==chend)
               {
                  *(outptr++) = ' ' ;
                  chptr = sourceline(++line,ipt,&len) ;
                  chend = chptr + len ;
                  for(; chptr<chend && isspace(*chptr); chptr++) ;
                  goto restart;
               }
               break ;

            case ':':
               *(outptr++) = *chptr ;

            case ';':
               goto endloop ;

         }
      }

      *(outptr++) = *(chptr++) ;
   }

endloop:
   assert( outptr - string->value <= BUFFERSIZE ) ;
   *outptr = '\0'; /* needs to be 0-terminated */
   string->len = outptr - string->value ;
   return string ;
}

