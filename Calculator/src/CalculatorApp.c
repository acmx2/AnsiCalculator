#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "Calculator.h"


#define BUFF_SZ 2048


/**
\mainpage Calculator brief description.
The Calculator realizes a simple expression evaluation stack automata approach.

Input data is a console expression like the following: 45 +676446- (+43 -3 +3) +78+12-((45)-(7+5))
Output data is a resulting number or an error message printed onto the console.

The Calculator pushes an input string to the stack until a closing bracket is met.
Then it evaluates a sub-expressions inside the topmost brackets on the stack, pops it,
and pushes its result back to the stack. Thus the stack grows up as the input characters
are pushed in it and goes down as the sub-expressions are folded on its top.
Finally, at the moment the end of the input string is fed, the stack contains an only
one brackets-free expression, which is evaluated and thus the result is obtained.

For the sake of simplicity the folded sub-expressions  are written onto the stack
in a string format with a '#' prefix sign to distinguish them from the terminals.
The stack buffer every time contains readable information and can be watched under debugger
as a single string.

The Calculator doesn't explicitly tokenize the input, however it detects all known
so far kinds of input errors and expression mistypings.
*/

/**
 * Writes a prompt symbol to a stream
 * @param pStream a stream, e.g. stdout
 */
void showPromptOnNewLine(FILE* pStream);

/**
 * Application entry point. Arguments are not used.
 * @param argc
 * @param argv
 * @return 0
 */
int main(int argc, char* argv[])
{
	char inputStrBuffer				[BUFF_SZ];
	char calculatorStackBuffer		[BUFF_SZ];

	FILE* pOutputStream =stdout;
	assert(pOutputStream);

	fprintf(pOutputStream, "	\nWelcome to Console Calculator.");
	fprintf(pOutputStream, "	\nIt handles expressions of no more than %u characters in length."	, (unsigned int)(BUFF_SZ-1)	);
	fprintf(pOutputStream, "	\nSupported features are: + and - signs, integers and ()[]{} brackets.");
	fprintf(pOutputStream, "	\nThe integers (and intermediate sums) are to be not greater than %lu .", (unsigned long int)CL_ULONG_MAX );

	initCalculator(calculatorStackBuffer, "()[]{}", pOutputStream, pOutputStream);

	showPromptOnNewLine(pOutputStream);

	while( fgets(inputStrBuffer, BUFF_SZ, stdin) )
	{	
		if( !calculateExpression(inputStrBuffer) )
		{
			fprintf(pOutputStream, "Please, retry.");			
		}
		
		showPromptOnNewLine(pOutputStream);
	}

	fprintf(pOutputStream, "Exit.\n");

	return 0;
}

void showPromptOnNewLine(FILE* pStream)
{
	assert(pStream);
	fprintf(pStream, "\n> ");
}
