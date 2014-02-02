#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "Calculator.h"
#include "CalcSymbols.h"
#include "CalcStack.h"
#include "CalcTypes.h"



static FILE*					pCalculatorOutputStream =0;
static FILE*					pCalculatorMessageStream =0;


const char* asString(const CL_ULONG* pX, char sgn)
{
	static char s_clStringBuff[CL_ULONG_MAX_PRINT +1];
	char* pCurr =s_clStringBuff;

	assert(pX);

	if (sgn <0)
	{
		sprintf( pCurr, "%c", getMinusSign() );
		pCurr++;
	}
	sprintf( pCurr, "%lu",  (unsigned long int) *pX);

	return s_clStringBuff;
}
void writeMessage(const char* s)
{
	assert(s);
	if (!pCalculatorMessageStream)
	{
		return; 
	}
	fprintf(pCalculatorMessageStream, s);
}
void writeOutput(const CL_ULONG* pX, char sgn)
{
	assert(pX);
	assert(pCalculatorOutputStream);

	fprintf(pCalculatorOutputStream, "=%s", asString(pX,sgn) );
}

/**
 * Reads a 'big-int' modulus from a string
 * @param pBuff string buffer beginning
 * @param ppBuffAfter output pointer to the string buffer position right after the big-int
 * @param pX big-int modulus
 * @return
 */
int readUnsignedInteger(const char* pBuff, const char** ppBuffAfter, CL_ULONG* pX)
{
	unsigned long int tmp;
	char* pTmpBuffAfter;

	assert(pBuff);
	assert(pX);

	if ( !isDigit(*pBuff) )
	{
		return 0;
	}

	tmp =strtoul(pBuff, &pTmpBuffAfter, 0);
	if (tmp ==ULONG_MAX )
	{
		writeMessage("Too big integer. ");
		return 0;
	}

	*pX =tmp;
	if (ppBuffAfter)
	{
		*ppBuffAfter =pTmpBuffAfter;
	}

	return 1;
}

/**
 * Writes a big-int to the stack with a '#' prefix, that means that after this call
 * the stack may look like: 2+345+(23+5+4)+3+#-456
 * @param pX big-int modulus
 * @param sgn big-int sign
 * @return
 */
int pushFoldedIntegerToStack(const CL_ULONG* pX, char sgn)
{
	pushToStack(getFoldedExprPrefix());
	pushStringToStack( asString(pX, sgn) );

	return 1;
}

/**
 * Reads a bracket-free expression items. E.g. it reads 32+#-546 as 32 and -56.
 * @param pBuff expression buffer
 * @param ppBuffAfter buffer position right after the read item
 * @param pX read item modulus
 * @param pSgn read item sign
 * @param firstItemInExpression input indicator used to determine if a delimiter sign is obligatory or not
 * @return 1 on success, otherwise 0
 */
int readItem(const char* pBuff, const char** ppBuffAfter, CL_ULONG* pX, char* pSgn, int firstItemInExpression)
{
	const char* pCurr =pBuff;
	*pSgn =1;

	assert(pCurr);
	assert(ppBuffAfter);
	assert(pX);
	assert(pSgn);

	if (firstItemInExpression)
	{
		if ( isMinusSign(*pCurr) )
		{
			*pSgn =  -1;
		}
		if ( isSign(*pCurr) )
		{
			pCurr++;
		}
	}
	else
	{
		if ( !isSign(*pCurr) )
		{
			writeMessage("Sign symbol expected. ");
			return 0;
		}
		if (isMinusSign(*pCurr))
		{
			*pSgn =  -1;
		}
		pCurr++;
	}

	if ( isFoldedExprPrefix(*pCurr) )
	{
		pCurr++;
		if (isMinusSign(*pCurr))
		{
			*pSgn = (*pSgn <0) ? 1 : -1;
			pCurr++;
		}
	}
		
	return readUnsignedInteger(pCurr, ppBuffAfter, pX);
}

/**
 * Adds one big-int to the second an stores the result in the second.
 * @param pArg first big-int modulus
 * @param argSgn first big-int sign
 * @param pRes second/result big-int modulus
 * @param pResSign second/result big-int sign
 * @return
 */
int addTo(const CL_ULONG* pArg, char argSgn, CL_ULONG* pRes, char* pResSign)
{
	assert(pArg);
	assert(pRes);
	assert(pResSign);

	if (( argSgn<0 && *pResSign>=0 ) || ( argSgn>=0 && *pResSign<0 ))
	{
		if (*pArg == *pRes)
		{
			*pRes =0;
			*pResSign =1;
			return 1;
		}
		if (*pArg < *pRes)
		{
			*pRes -= *pArg;
		}
		else
		{
			CL_ULONG tmp = *pRes;
			*pRes = *pArg;
			*pRes -=tmp;
			*pResSign =argSgn;
		}
		return 1;
	}

	*pRes += *pArg;
	if (*pRes < *pArg || *pRes > CL_ULONG_MAX )
	{
		writeMessage("Integer overflow during summation. ");
		return 0;
	}

	return 1;
}

/**
 * Evaluates a bracket-free expression.
 * @param pBuff a buffer containing the expression. Usually this is a memory near the top of the calculator stack,
 * however this function isn't tied to the stack anyway
 * @param pX pointer to result module
 * @param pSign pointer to result sign
 * @return
 */
int evaluate(const char* pBuff, CL_ULONG* pX, char* pSign)
{
	const char* pCurr =pBuff;
	CL_ULONG tmpVal;
	char tmpSgn;
	int firstItemInExpression =1;

	assert(pCurr);
	assert(pX);
	assert(pSign);

	if( !*pCurr )
	{
		writeMessage("Empty expression. ");
		return 0;
	}

	*pX =0;
	*pSign =1;

	while(*pCurr)
	{
		if ( !readItem(pCurr, &pCurr, &tmpVal, &tmpSgn, firstItemInExpression) )
		{
			return 0;
		}

		if ( !addTo(&tmpVal, tmpSgn, pX, pSign) )
		{
			return 0;
		}

		firstItemInExpression =0;
	};

	return 1;
}

/**
 * Folds an expression on the top of the stack. That is, detects a topmost bracket-free
 * sub-expression, evaluates it and substitutes its result (prefixed with '#') for it.
 * @param c a closing bracket not on the stack. A correspondent opening bracket on the stack bounds
 * the sub-expression to evaluate.
 * @return 1 on success, otherwise 0
 */
int foldTopOfStack(char c)
{	
	size_t openingBracketPos;
	if ( !findTopmostOpeningBracket(&openingBracketPos) )
	{
		writeMessage("Opening bracket not found. ");
		return 0;
	}

	if ( !arePairBrackets( *getStackTopFrom(openingBracketPos), c) )
	{
		writeMessage("Bracket mismatch. ");
		return 0;
	}

	{
		CL_ULONG tmpVal;
		char sign;

		if( !evaluate( getStackTopFrom(openingBracketPos+1), &tmpVal, &sign ) )
		{
			writeMessage("Sub-expression error. ");
			return 0;
		}

		popStackTop(openingBracketPos);

		if( !pushFoldedIntegerToStack(&tmpVal, sign) )
		{
			writeMessage("Stack writing error. ");
			return 0;
		}
	}

	return 1;
}

/**
 * Pushes an uninterrupted digit sequence to the stack
 * @param ppCurr p-pointer to the number token first digit in the input string.
 * Will contain a p-pointer to the first symbol after the end of the number token on the call completion.
 */
void forwardNumberTokenToStack(const char** ppCurr)
{
	assert(ppCurr);

	while( isDigit(**ppCurr) )
	{
		pushToStack(**ppCurr);
		(*ppCurr)++;
	}
}

/**
 * Initializes the calculator. Should be called once per session.
 * @param pBuff calculator stack buffer, should be allocated and passed into this call by a client. The buffer size should be strictly not less than the maximum expected input string length.
 * @param pBrackets a string describing the brackets to use, e.g. "[]()<>{}"
 * @param pOutputStream a stream to write the result out
 * @param pMessageStream a stream for writing the error messages to
 */
void initCalculator(char* pBuff, const char* pBrackets, FILE* pOutputStream, FILE* pMessageStream)
{
	pCalculatorOutputStream =pOutputStream;
	pCalculatorMessageStream =pMessageStream;
	assert(pCalculatorOutputStream);

	initCalculatorBrackets(pBrackets);
	initCalculatorStack(pBuff);
}

/**
 * Evaluates an expression and writes the result out to the output stream
 * @param pInStr input null-terminated string containing the expression. Note, the
 * initCalculator(...) call should be made at least once before calling this function. -
 * The provided calculator stack buffer should be of size not less than the pInStr length.
 * @return
 */
int calculateExpression(const char* pInStr)
{
	const char* pCursor =pInStr;
	int signSymbolOrClosingBracketExpected =0;

	assert(pCursor);
	
	clearCalculatorStack(); /*clearing the stack*/

	/*reading the input in one pass*/
	while(*pCursor)
	{
		/*expect only valid characters in the input*/
		if ( !isValidCharacter(*pCursor) )
		{
			writeMessage("Expression contains a disallowed symbol. ");
			return 0; 
		}
		/*only the closing bracket or +/- symbols are expected after a number token*/
		if ( signSymbolOrClosingBracketExpected	&& 
					!isWhiteSpace(*pCursor)		&&
					!isSign(*pCursor)			&&
					!isClosingBracket(*pCursor) )
		{
			writeMessage("Sign symbol was expected between numbers. ");
			return 0;
		}
		/*if a digit is met, then read the whole number token and rise the signSymbolOrClosingBracketExpected flag*/
		if( isDigit(*pCursor) )
		{
			forwardNumberTokenToStack( &pCursor );
			signSymbolOrClosingBracketExpected =1;
			continue;
		}
		/*if sign is met, then drop the signSymbolOrClosingBracketExpected flag*/
		if ( isSign(*pCursor) )
		{
			pushToStack(*pCursor);
			signSymbolOrClosingBracketExpected =0;
			pCursor++;
			continue;
		}
		/*if a closing bracket is met, then evaluate a topmost bracket-free sub-expression on the stack*/
		if ( isClosingBracket(*pCursor) )
		{
			if( !foldTopOfStack(*pCursor) )
			{
				return 0;
			}
			signSymbolOrClosingBracketExpected =1;
			pCursor++;
			continue;
		}
		/*push everything not a whitespace to the stack*/
		if ( !isWhiteSpace(*pCursor) )
		{
			pushToStack(*pCursor);
			pCursor++;
			continue;
		}

		pCursor++;
	}

	{
		CL_ULONG nResult;
		char sign;

		if( !evaluate( getStackTopFrom(0), &nResult, &sign) )
		{
			writeMessage("Not a valid expression. ");
			return 0;
		}

		writeOutput(&nResult, sign);	
	}

	return 1;
}
