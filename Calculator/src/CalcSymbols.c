#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "CalcSymbols.h"


#define CL_PLUS_SIGN '+'
#define CL_MINUS_SIGN '-'
#define CL_FOLDED_EXPR_PREFIX '#'


static const char*				pCalculatorBrackets				=0;
static size_t					nCalculatorBracketPairsCount	=0;


void initCalculatorBrackets(const char* pBrackets)
{
	pCalculatorBrackets =pBrackets;
	assert(pCalculatorBrackets);

	nCalculatorBracketPairsCount =strlen(pCalculatorBrackets);
	assert(nCalculatorBracketPairsCount%2 ==0);

	nCalculatorBracketPairsCount /=2;
}
int isWhiteSpace(char c)
{
	return isspace(c);
}
int isDigit(char c)
{
	return isdigit(c);
}
int isPlusSign(char c)
{
	return (c==CL_PLUS_SIGN) ? 1 : 0;
}
int isMinusSign(char c)
{
	return (c==CL_MINUS_SIGN) ? 1 : 0;
}
int isSign(char c)
{
	return (isPlusSign(c) || isMinusSign(c));
}
int isValidCharacter(char c)
{
	return (		isWhiteSpace(c	)			||
					isDigit(c)					||
					isOpeningBracket(c)			||
					isClosingBracket(c)			||
					isMinusSign(c)				||
					isPlusSign(c)			);

}
int getClosingBracketIndex(char c)
{
	size_t i =0;
	for(; i< nCalculatorBracketPairsCount; i++)
	{
		if (pCalculatorBrackets[2*i +1] ==c)
		{
			return i;
		}
	}
	return -1;
}
int getOpeningBracketIndex(char c)
{
	size_t i =0;
	for(; i< nCalculatorBracketPairsCount; i++)
	{
		if (pCalculatorBrackets[2*i] ==c)
		{
			return i;
		}
	}
	return -1;
}
int arePairBrackets(char c1, char c2)
{
	int idx1 =getOpeningBracketIndex(c1);
	int idx2 =getClosingBracketIndex(c2);
	if (idx1>=0 && idx1==idx2)
	{
		return 1;
	}
	return 0;
}
int isOpeningBracket(char c)
{
	return (getOpeningBracketIndex(c)<0) ? 0 : 1;
}
int isClosingBracket(char c)
{
	return (getClosingBracketIndex(c)<0) ? 0 : 1;
}
int isFoldedExprPrefix(char c)
{
	return (c==getFoldedExprPrefix()) ? 1 : 0;
}
char getFoldedExprPrefix()
{
	return CL_FOLDED_EXPR_PREFIX;
}
char getMinusSign()
{
	return CL_MINUS_SIGN;
}
