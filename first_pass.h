#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "check_action.h"

#define SKIP_SPACE while(line[i] != '\0' && isspace(line[i])) i++;

/* 
 * Function: firstPass
 * ------------------
 * Performs the first pass on the assembly source file.
 * Responsible for validating the input syntax, parsing labels, commands, and directives.
 * Updates the symbol table and prints error messages in English for any syntax errors found.
 */
bool firstPass(char *filename);

char *isUniqueLabel(node *head, char *labelName);
int updateLabelAddress(node *head, char *labelName, bool isDcCommand, int dcStart);
bool noEntryLabelsWithZeroAddress(node *head);
void addToEnd(ptr *head, char *label, int address, char *type);
bool isOnlyLowerLetters(char s[]);
ptr createNode(char *label, int addr, char *type);
bool mustHavelabel(char *line);
void updateAddresses(ptr head);

#endif


