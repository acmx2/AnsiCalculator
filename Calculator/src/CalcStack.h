#ifndef __FG_CALC_STACK_H__ 
#define __FG_CALC_STACK_H__

#include <stddef.h>

#include "CalcTypes.h"


void initCalculatorStack(char*);
void clearCalculatorStack();
void pushToStack(char c);
void pushStringToStack(const char* pS);
int findTopmostOpeningBracket(size_t* pPos);
const char* getStackTopFrom(size_t pos);
void popStackTop(size_t pos);

#endif
