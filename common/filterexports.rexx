/* filterexports.rexx
 * Write all lines beginning with EXPORTS to the outfile if
 * the symbol is available
 * This is used for filtering available exports
 * Called by ./rexxgd.exe that was just built prior to this step
 */
Parse Arg queryfunc infile outfile .
str = "Call" queryfunc "'func.', 'A'"
Interpret str
Call Stream outfile, 'C', 'OPEN WRITE REPLACE'
Do While( Lines( infile ) > 0 )
   line = Linein( infile )
   If Word( line, 1 ) = 'EXPORTS' Then
      Do
         Parse Var line . func .
         If FindFunc( func ) = 0 Then Iterate
      End
   Call Lineout outfile, line
End
Call Stream outfile, 'C', 'CLOSE'
Return 0

FindFunc: Procedure Expose func.
Parse Upper Arg func
Do i = 1 To func.0
   If func = func.i Then Return 1
End
Return 0
