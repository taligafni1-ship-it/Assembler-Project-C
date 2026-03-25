/* 
 * These files of check_action are responsible for verifying 
 * the correctness and validity of the program output.
 */

#ifndef CHECK_ACTION_H
#define CHECK_ACTION_H

#include "types.h"
#include "util.h"

bool dontHaveSpaceBetweenChar(char *str);
bool isUniqueMacroContent(char *content);
bool codeLable(char input[]);
bool isValidNumber(char input[]);
bool twoOperandAction(char operands[]);
bool oneOperandAction(char operands[]);
bool zeroOperandAction(char operands[]);
void removeSpacesBeforeBracket(char *str);
bool validLabel(char word[], bool needColon);
bool hasOnlyOneOperand(char *operands);
bool hasTwoOperands(char *operands);
bool isValidMat(char word[]);
bool isValidReg(char mat[]);
int checkDC(char line[]);
bool validMatName(char word[]);
bool prn(char operands[]);
bool lea(char operands[]);
bool cmp(char operands[]);
char *strdup(const char *s);

#endif

