AnsiCalculator
==============

An approach to the ANSI C programming


The Calculator evaluates the arithmetic expressions  like a stack automata, in one pass.

Input data is a console expression, e.g.: 45 +676446- (+43 -3 +3) +78+12-((45)-(7+5)) Output data is a resulting number or an error message printed onto the console.

The Calculator pushes an input string to the stack until a closing bracket is met. Then it evaluates a sub-expressions inside the topmost brackets on the stack, pops it, and pushes its result back to the stack. Thus the stack grows up as the input characters are pushed in it and goes down as the sub-expressions are folded on its top. Finally, at the moment the end of the input string is fed, the stack contains an only one brackets-free expression, which is evaluated and thus the result is obtained.

For the sake of simplicity the folded sub-expressions are written onto the stack in a string format. The stack buffer contains a readable information and can be watched under debugger as a single string.

The Calculator doesn't explicitly tokenize the input, however it detects all known so far kinds of input errors and expression mistypings. 
