#ifndef CHECK_ACTION_SECOND_PASS_H
#define CHECK_ACTION_SECOND_PASS_H


#include "types.h"
#include "util.h"

int entryOrExternAppears(char *line);

void addToWordBinary(int value, int bits);



void movPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void cmpPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void addPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void subPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void leaPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void clrPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void notPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void incPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void decPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void jmpPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void bnePrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void jsrPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void redPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void prnPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void rtsPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

void stopPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);

int findLabelAddress(ptr head, char *label);

int checkIfLabelExists(ptr head, char *label);

#endif




