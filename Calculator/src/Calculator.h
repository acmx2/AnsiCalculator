#ifndef __FG_CALCULATOR_H__ 
#define __FG_CALCULATOR_H__

#include <stdio.h>

#include "CalcTypes.h"


void initCalculator(char*, const char*, FILE*, FILE*);
int calculateExpression(const char*);

#endif
