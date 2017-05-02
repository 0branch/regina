/*--------------------------------------------------------------------�
  � This REXX program shows that  virtual memory  is  exhausted  with: �
  �                                                                    �
  �              Regina 3.7                 just past J = 17,300   and �
  �              Regina 3.6                 just past J = 17,200   and �
  �              Regina 3.5  and  3.4       just past J = 17,400   and �
  �              Regina 3.3                 just past J = 73,700.      �
  �                                                                    �
  � What this program does is emulate a much larger REXX program that  �
  � reads in many files,  each file contains a number between  one and �
  � one billion.    Each number found is  indicated  by placing a      �
  � non-blank character at the appropriate  offset  from each  segment �
  � (whose length can be  up to  and  including  segSize).             �
  �                                                                    �
  � Example:  for the number  4032,    the  4032nd  character in the   �
  �           G.0  segment would contain  (say)  a  "1".               �
  �                                                                    �
  �           For the number 57000,    the  7000th  character in the   �
  �           G.5  segment would contain  (say)  a  "1".               �
  �                                                                    �
  � The segment number is calculated thusly: seg# = number % segSize   �
  �  "    "     offset  "     "         "    segO = number//segSize +1 �
  �                                                                    �
  � This program doesn't do any reading,  as the input files are too   �
  � large to upload and would take to much time for REXX to read them. �
  �                                                                    �
  � So this REXX program constructs  100,000  fully-populated segment  �
  � records, and then   tries   to consolidate them into  100  segment �
  � records,  DROPping each old segment record as it's processed.      �
  �                                                                    �
  � The reason that the large REXX program just doesn't populate the   �
  � large segment records is that is takes to long for REXX to keep    �
  � rebuilding the segment records  using the  OVERLAY  BIF  to change �
  � the  4032nd  and  7000th   character   (as in the above examples). �
  �                                                                    �
  � It should be noted that Regina 3.3 sucessfully reads in the entire �
  � set of files  (and subsequently processes them),  Regina 3.4, 3.5, �
  � 3.6, and 3.7   exhaust system recources   while reading the files. �
  �                                                                    �
  � Also, it should be noted that the large REXX program doesn't do    �
  � any consolidation,  that's just a mechanism that this program uses �
  � to show (possibly)  how/why the large REXX program is failing with �
  � Regina releases  3.4  -->  3.7  (inclusive).                       �
  --------------------------------------------------------------------*/

g.=                                    /*the   original   stemmed array.*/
gg.=                                   /*the consolidated    "      "   */
segSize = 10000                        /*size of the segments.          */
times   = 1000000000 % segSize         /*that's  one  billion (before %)*/
parse version v
say 'segSize=' segSize  '  times='times '  REXX version:' v
dummyRec = copies('fa'x, segSize)      /*a stand-in thingy dummy record.*/

    /*------------------------------------------------------------------*/
call time 'R'
                  do i=0  for times    /*generate  100,000  of these.   */
                  g.i = dummyRec       /*generate pseudo segment record.*/
                  end   /*i*/

say '   generating' i "segments took" format(time('E'),,2) "seconds."

    /*------------------------------------------------------------------*/
call time 'R'
                  do j=0  for i        /*process each pseudo segment rec*/
                  cseg = j%100         /*calculate new consolidated seg#*/
if j//100==0 then say 'using ' j  '   and cseg: ' cseg    /*show & tell.*/
                  gg.cseg = gg.cseg || g.j                /*consolidate,*/
                  drop g.j                                /*save memory.*/
                  end  /*j*/

say 'consolidation' i%100 "segments took" format(time('E'),,2) "seconds."
segSize = segSize*100                  /*the new segment size.          */
