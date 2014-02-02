#ifndef __FG_CALC_SYMBOLS_H__ 
#define __FG_CALC_SYMBOLS_H__

void initCalculatorBrackets(const char*);

int isWhiteSpace(char c);
int isDigit(char c);
int isPlusSign(char c);
int isMinusSign(char c);
int isSign(char c);
int isValidCharacter(char c);

int isFoldedExprPrefix(char c);
char getFoldedExprPrefix();
char getMinusSign();

int arePairBrackets(char c1, char c2);
int isOpeningBracket(char c);
int isClosingBracket(char c);

#endif
