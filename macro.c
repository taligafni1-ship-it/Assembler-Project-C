/*
 * File: macro.c
 * Description: This file implements macro handling for an assembler.
 * It supports:
 *   - Defining and storing macros
 *   - Replacing macros in source files
 *   - Checking reserved words
 *   - Utility functions for string and line handling
 * Author: <Your Name>
 */

#include "macro.h"
#define NUM_WORDS_IN_DEFANITION_MACRO 2

MacroList macroList;

/* 
 * printMacro:
 * Search for a macro named targetName in the list.
 * If found, print its content into destinationFile.
 * Returns true if a macro was printed, otherwise false.
 */
bool printMacro(MacroList *list, char *targetName, FILE *destinationFile) {
	bool macroPrinted = false;
	int i = 0;

	for (i = 0; i < list->count; i++) {
		if (strcmp(list->macros[i].macroName, targetName) == 0) {
			fprintf(destinationFile, "%s\n", list->macros[i].macroContent); /* Print macro content */
			macroPrinted = true;
		}
	}
	return macroPrinted;
}

/* 
 * addMacro:
 * Add a new macro into the list.
 * Allocates memory for macro name and content.
 */
void addMacro(MacroList *list, char *name, char *content) {
	MacroDefinition *newMacro;

	if (list->count == list->capacity) {
		int newCapacity = list->capacity * DOUBLE_THE_ZISE;
		MacroDefinition *newArr = realloc(list->macros, newCapacity * sizeof(MacroDefinition));
		if (newArr == NULL) {
			printf("Allocation failed\n");
			return;
		}
		list->macros = newArr; /* Update pointer to new array */
		list->capacity = newCapacity;
	}

	newMacro = &list->macros[list->count];
	newMacro->macroName = malloc(strlen(name) + 1);
	newMacro->macroContent = malloc(strlen(content) + 1);

	if (!newMacro->macroName || !newMacro->macroContent) {
		free(newMacro->macroName);
		free(newMacro->macroContent);
		printf("Memory allocation for macro fields failed\n");
		return;
	}

	strcpy(newMacro->macroName, name);
	strcpy(newMacro->macroContent, content);
	list->count++;
}

/* 
 * isCommand:
 * Check if the given string is a reserved command, register, or directive.
 * Returns true if it is reserved, otherwise false.
 */
bool isCommand(char *currentName) {
	if (strcmp(currentName, "mov") == 0 ||
		strcmp(currentName, "cmp") == 0 ||
		strcmp(currentName, "add") == 0 ||
		strcmp(currentName, "sub") == 0 ||
		strcmp(currentName, "lea") == 0 ||
		strcmp(currentName, "clr") == 0 ||
		strcmp(currentName, "not") == 0 ||
		strcmp(currentName, "inc") == 0 ||
		strcmp(currentName, "dec") == 0 ||
		strcmp(currentName, "jmp") == 0 ||
		strcmp(currentName, "bne") == 0 ||
		strcmp(currentName, "jsr") == 0 ||
		strcmp(currentName, "red") == 0 ||
		strcmp(currentName, "prn") == 0 ||
		strcmp(currentName, "rts") == 0 ||
		strcmp(currentName, "stop") == 0 ||
		strcmp(currentName, "r1") == 0 ||
		strcmp(currentName, "r2") == 0 ||
		strcmp(currentName, "r3") == 0 ||
		strcmp(currentName, "r4") == 0 ||
		strcmp(currentName, "r5") == 0 ||
		strcmp(currentName, "r6") == 0 ||
		strcmp(currentName, "r7") == 0 ||
		strcmp(currentName, "r0") == 0 ||
		strcmp(currentName, "string") == 0 ||
		strcmp(currentName, "data") == 0 ||
		strcmp(currentName, "mat") == 0 ||
		strcmp(currentName, "entry") == 0 ||
		strcmp(currentName, "extern") == 0) {
		return true;
	}
	return false;
}

/* 
 * notOnlyWhite:
 * Check if the string contains any non-whitespace characters.
 * Returns true if at least one non-space character is found.
 */
bool notOnlyWhite(char *str) {
	int i = 0;

	while (str[i] != '\0') {
		if (!isspace((unsigned char)str[i])) {
			return true; /* Found a non-space character */
		}
		i++;
	}
	return false;
}

/* 
 * countWords:
 * Count the number of words in a string (words separated by spaces).
 */
int countWords(char *str) {
	int count = 0;
	bool inWord = false;
	int i = 0;

	for (i = 0; str[i] != '\0'; i++) {
		if (!isspace((unsigned char)str[i])) {
			if (!inWord) {
				inWord = true;
				count++;
			}
		} else {
			inWord = false;
		}
	}
	return count;
}

/* 
 * findMacro:
 * Parse the input assembly file (asFile).
 * Detects macro definitions and replaces macro usage with content.
 * Writes the expanded code into amFile.
 * Returns false if errors were found, true otherwise.
 */
bool findMacro(char *asFile, char *amFile) {
	char line[MAX_SIZE_FOR_LINE] = {0};
	char *token, currentName[MAX_SIZE_FOR_LINE] = {0}, tempContent[MAX_SIZE_FOR_LINE] = {0};
	bool inMacro = false, printed = false, notValid = false;
	int lineCount = 1;
	FILE *destinationFile;
	FILE *sourceFile = fopen(asFile, "r");

	/* Open source file */
	if (sourceFile == NULL) {
		return false;
	}

	destinationFile = fopen(amFile, "w+");
	if (destinationFile == NULL) {
		printf("Error opening destination file file.am");
		fclose(sourceFile);
		exit(EXIT_FAILURE);
	}

	/* First pass: detect and store macros */
	while (fgets(line, sizeof(line), sourceFile) != NULL) {
		int numWordsInLine;
		if (!notOnlyWhite(line)) continue;
		numWordsInLine = countWords(line);
		token = strtok(line, " \t\n");

		while (token != NULL) {
			/* Macro definition start */
			if (!inMacro && strcmp(token, "mcro") == 0) {
				token = strtok(NULL, " \t\n");
				if (numWordsInLine != NUM_WORDS_IN_DEFANITION_MACRO) {
					printf(" macro must be the only word in the line");
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				if (token == NULL) {
					printf("missing macro name after 'mcro'");
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				strcpy(currentName, token);
				if ((token = strtok(NULL, " \t\n")) != '\0') {
					printf("extra words after macro name '%s'", currentName);
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				tempContent[0] = '\0';
				inMacro = true;
				token = strtok(NULL, " \t\n");
				if (token == NULL) break;
			}

			/* Macro definition end */
			if (inMacro && strcmp(token, "mcroend") == 0) {
				if (numWordsInLine != 1) {
					printf(" macro must be the only word in the line");
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				if ((token = strtok(NULL, " \t\n")) != '\0') {
					printf("extra words after 'mcroend'");
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				if (isCommand(currentName)) {
					printf("macro name '%s' is a saved name", currentName);
					printf(", the error is in line %d\n", lineCount);
					lineCount++;
					token = strtok(NULL, "\n");
					notValid = true;
					continue;
				}
				addMacro(&macroList, currentName, tempContent); /* Add macro to list */
				inMacro = false;
			}

			/* Collect macro content */
			if (inMacro) {
				strcat(tempContent, token);
				strcat(tempContent, " ");
			}

			token = strtok(NULL, " \t\n");

			if (inMacro && token == NULL)
				strcat(tempContent, "\n");
		}

		if (notValid == false)
			lineCount++;
	}

	rewind(sourceFile);
	inMacro = false;

	if (notValid) return false;

	/* Second pass: print macros or normal tokens */
	while (fgets(line, sizeof(line), sourceFile) != NULL) {
		token = strtok(line, " \t\n");
		while (token != NULL) {
			/* Macro start */
			if (!inMacro && strcmp(token, "mcro") == 0) {
				token = strtok(NULL, " \t\n");
				if (token == NULL) break;
				inMacro = true;
			}

			/* Macro end */
			if (inMacro && strcmp(token, "mcroend") == 0) {
				inMacro = false;
				token = strtok(NULL, " \t\n");
				if (token == NULL) break;
			}

			if (!inMacro) {
				printed = printMacro(&macroList, token, destinationFile); /* Try to print macro */
				if (!printed) {
					fprintf(destinationFile, "%s", token);
				}
				fprintf(destinationFile, " ");
			}
			token = strtok(NULL, " \t\n");
		}

		if (!inMacro)
			fprintf(destinationFile, "\n");
	}

	fclose(sourceFile);
	fclose(destinationFile);
	return !notValid;
}

