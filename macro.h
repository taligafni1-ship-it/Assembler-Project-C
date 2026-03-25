#ifndef MACRO_H
#define MACRO_H

/*
 * This header (and its related macro source files) is responsible
 * for handling macro expansion in the assembler.
 */

#include "types.h"
#include "util.h"

bool notOnlyWhite(char *str);
int countWords(char *str);

bool printMacro(MacroList *list, char *targetName, FILE *destinationFile);
bool findMacro(char *asFile, char *amFile);
bool printMacro(MacroList *list, char *targetName, FILE *destinationFile);
void addMacro(MacroList *list, char *name, char *content);
bool isCommand(char *currentName);

#endif

