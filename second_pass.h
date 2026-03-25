#ifndef SECOND_PASS_H
#define SECOND_PASS_H

/*
 * second_pass.h
 *
 * This header defines functions and structures used in the second pass of the assembler.
 * The second pass files are responsible for encoding instructions, generating object code,
 * and handling entry and extern labels.
 */

#include "check_action_second_pass.h"

#define PC_START 100
#define BINARY_LENGTH 11
#define BASE_FOUR_LENGTH 6
#define MAX_ACTION_SIZE 5

typedef struct {
	char name[MAX_ACTION_SIZE];
	void (*func)(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
	             bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister);
} commandPrint;

void printEntryLabels(ptr head, FILE *entryFp);
bool isExternal(ptr head, char *labelName);
int checkLabelType(ptr head, char *labelName);
void printDCList1(structDCList *list);
void printDecimalAsLetters(FILE *objectFp, int num);
void base4ToLetters(FILE *objectFp, char arr[BASE_FOUR_LENGTH]);
void binaryToBase4(const char binary[BINARY_LENGTH], char wordBaseFour[BASE_FOUR_LENGTH]);
void extractLabelName(char *src, char *dest);
bool secondPass(char *amFile, char *objectFile, char *externFile, char *entryFile);
bool isAction(char *currentName);
bool isEntry(ptr head, char *labelName);

#endif

