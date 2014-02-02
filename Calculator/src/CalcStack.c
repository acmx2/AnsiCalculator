#include <stddef.h>
#include <ctype.h>
#include <assert.h>

#include "CalcStack.h"
#include "CalcSymbols.h"


static size_t					nCalculatorStackSize		=0;
static char*					pCalculatorStackBuffer		=0;

/**
 * Initializes the stack, is called by a calculator initialization routine.
 * @param pBuffer pre-allocated calculator stack buffer pointer
 */
void initCalculatorStack(char* pBuffer)
{
	assert(pBuffer);

	pCalculatorStackBuffer =pBuffer;
	clearCalculatorStack();
}

/**
 * Clears the stack, is called by the calculator each time before input processing.
 */
void clearCalculatorStack()
{
	assert(pCalculatorStackBuffer);

	nCalculatorStackSize =0;
	pCalculatorStackBuffer[0] =0x0;
}

/**
 * Pushes a char to the stack
 * @param c
 */
void pushToStack(char c)
{
	pCalculatorStackBuffer[nCalculatorStackSize] =c;
	nCalculatorStackSize++;
	pCalculatorStackBuffer[nCalculatorStackSize] =0x0;
}

/**
 * Looks for the first occurrence of an opening bracket starting from the stack top
 * @param pPos if found, then contains a bracket position on the stack. A position
 * is calculated as an offset from the stack bottom and its correspondent element
 * can be accessed via getStackTopFrom(pos) call
 * @return 1 on success, otherwise 0
 */
int findTopmostOpeningBracket(size_t* pPos)
{
	if(nCalculatorStackSize ==0)
	{
		return 0;
	}

	*pPos =nCalculatorStackSize;
	(*pPos)--;

	while( !isOpeningBracket( pCalculatorStackBuffer[*pPos] ) )
	{
		if ( (*pPos) ==0 )
		{
			return 0;
		}
		(*pPos)--;
	}

	return 1;
}
/**
 * Returns a stack memory pointer at a given position (offset from the stack bottom)
 * @param pos position in the stack obtained, e.g. via findTopmostOpeningBracket.
 * If 0 is passed, then the whole stack buffer pointer is returned.
 * @return
 */
const char* getStackTopFrom(size_t pos)
{
	return (pCalculatorStackBuffer +pos);
}
/**
 * Pops all the elements above and including the pos out of the stack
 * @param pos position to pop out. This position is not valid after this call.
 * Namely, it will contain 0x0 as the stack always has a zero byte right above its top byte.
 */
void popStackTop(size_t pos)
{
	assert(pos <=nCalculatorStackSize);

	nCalculatorStackSize =pos;
	pCalculatorStackBuffer[nCalculatorStackSize] =0x0;
}
/**
 * Pushes a string to the stack
 * @param pS string pointer, shouldn't be 0
 */
void pushStringToStack(const char* pS)
{
	assert(pS);

	while(*pS)
	{
		pCalculatorStackBuffer[nCalculatorStackSize] =(*pS);
		nCalculatorStackSize++;
		pS++;
	}
	pCalculatorStackBuffer[nCalculatorStackSize] =0x0;
}
