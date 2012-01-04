/*
 * Generic script to add a image title to a Robodoc generated index.html file and to create a title.html file
 * used by htmldoc as the front page for the PDF that is generated from the HTML
 *
 * Usage:
 *    rexx addimage.rexx index-html-from-robodoc title-html-created version project-shortname project-longname
 * eg:
 *    rexx addimage.rexx doc/index.html doc/title.html 1.0 rexxeec Rexx/EEC
 */
Parse Arg fn titlefn ver shortname longname
init_year = 2008
If Left(Date('S'),4) = init_year Then copydate = init_year
Else copydate = init_year'-'Left(Date('S'),4)
idx = 0
start = 1
footer = 0
Call Stream fn, 'C', 'OPEN READ'
Do While Lines( fn ) > 0
   line = Linein( fn )
   Select
      /*
       * Excluded line was for prefious veraion of Robodoc.
      When Countstr( '<div id="logo">', line ) \= 0 & start = 1 The
       */
      /*
       * Following line works with Robodoc 4.99.6
       */
      When Countstr( '<body', line ) \= 0 & start = 1 Then
         Do
           idx = idx + 1
           line.idx = line
           idx = idx + 1
           line.idx = '<center><img src="images/'shortname'.png" border=0 alt="'longname'"></center>'
           idx = idx + 1
           line.idx = '<center>Version 'ver'</center>'
           idx = idx + 1
           line.idx = '<center>Copyright (C)' copydate 'Mark Hessling &lt;mark@rexx.org&gt;</center>'
           start = 0
           addline = 0
         End
      When Left( line, 7 ) = '<title>' Then
         Do
           line = '<title>'longname' Reference</title>'
           addline = 1
         End
      When Left( line, 6 ) = '</div>' & footer = 1 Then
         Do
           footer = 0
           addline = 0
         End
      When footer = 1 Then addline = 0
      When Countstr( '"footer"', line ) \= 0 & Countstr( '<div id', line ) \= 0 Then
         Do
           footer = 1
           addline = 0
         End
      Otherwise addline = 1
   End
   If addline Then
      Do
         idx = idx + 1
         line.idx = line
      End
End
line.0 = idx
Call Stream fn, 'C', 'CLOSE'
Call Stream fn, 'C', 'OPEN WRITE REPLACE'
Do i = 1 To line.0
   Call Lineout fn, line.i
End
Call Stream fn, 'C', 'CLOSE'

Call Stream titlefn, 'C', 'OPEN WRITE REPLACE'

Call Lineout titlefn,'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">'
Call Lineout titlefn,'<html>'
Call Lineout titlefn,'<head>'
Call Lineout titlefn,'<meta http-equiv="Content-Type" content="text/html; charset=utf-8">'
Call Lineout titlefn,'</head>'
Call Lineout titlefn,'<body>'
Call Lineout titlefn,'<center><img src="images/'shortname'.gif" border=0 alt="'longname'"></center>'
Call Lineout titlefn,'<center><h1>'longname 'Reference</h1></center>'
Call Lineout titlefn,'<center><h2>Version 'ver'</h2></center>'
Call Lineout titlefn,'<center><h3>Copyright (C)' copydate 'Mark Hessling &lt;mark@rexx.org&gt;</h3></center>'
Call Lineout titlefn,'</body>'
Call Lineout titlefn,'</html>'
Call Stream titlefn, 'C', 'CLOSE'
Return 0
