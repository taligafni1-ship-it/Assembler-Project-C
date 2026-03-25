/*******************************************************
 * Project: Custom Two-Pass Assembler
 * Authors: Leah Singer, Avital Gafni
 * 
 * Description:
 * This program implements a two-pass assembler designed
 * to translate assembly source code into a custom 
 * binary representation. The final machine code is 
 * printed in a unique base-4 format, where each word of
 * machine code is represented according to the project’s 
 * specific encoding rules.
 * 
 * The assembler performs the following steps:
 *  1. First pass:
 *     - Reads the input assembly code line by line.
 *     - Identifies and stores labels, macros, and data 
 *       declarations.
 *     - Performs initial validation of instructions.
 * 
 *  2. Second pass:
 *     - Resolves label addresses.
 *     - Generates the final machine code.
 *     - Produces output files containing the custom
 *       base-4 encoded binary code, as well as data
 *       and symbol information if required.
 * 
 * This project is a full educational assembler written
 * in C, demonstrating parsing, symbol management, and 
 * code generation techniques.
 * 
 * Date: 2025
 *******************************************************/



#include "main.h"

#define MEMORY_SIZE 256
#define INITIAL_CAPACITY 10

structDCList dcList;
MacroList macroList;
ptr head;
int icCounter;
int dcCounter;
int lineCounter;

/*
 * initMacroList:
 *	Initialize a MacroList with given initial capacity.
 *	Allocates memory for the macros array and sets count to 0.
 */
void initMacroList(MacroList *list, int initialCapacity) {
	list->macros = malloc(initialCapacity * sizeof(MacroDefinition));
	if (!list->macros) {
		perror("Failed to allocate MacroList");
		exit(1);
	}
	list->count = 0;
	list->capacity = initialCapacity;
}

/*
 * initStructDCList:
 *	Initialize a structDCList with given initial capacity.
 *	Allocates memory for the structs array and sets count to 0.
 */
void initStructDCList(structDCList *list, int initialCapacity) {
	list->structs = malloc(initialCapacity * sizeof(structDC));
	if (!list->structs) {
		perror("Failed to allocate structDCList");
		exit(1);
	}
	list->count = 0;
	list->capacity = initialCapacity;
}

/*
 * initNodeList:
 *	Initialize a linked list head to NULL.
 */
void initNodeList(ptr *head) {
	*head = NULL;
}

/*
 * freeMacroList:
 *	Free all dynamically allocated memory within a MacroList.
 */
void freeMacroList(MacroList *list) {
	int i;
	if (list->macros != NULL) {
		for (i = 0; i < list->count; i++) {
			free(list->macros[i].macroName);
			free(list->macros[i].macroContent);
		}
		free(list->macros);
		list->macros = NULL;
		list->count = 0;
		list->capacity = 0;
	}
}

/*
 * freeDCList:
 *	Free all dynamically allocated memory within a structDCList.
 */
void freeDCList(structDCList *list) {
	int i;
	if (list->structs != NULL) {
		for (i = 0; i < list->count; i++) {
			free(list->structs[i].info);
		}
		free(list->structs);
		list->structs = NULL;
		list->count = 0;
		list->capacity = 0;
	}
}

/*
 * freeNodeList:
 *	Free all nodes in a linked list and their dynamically allocated fields.
 */
void freeNodeList(ptr *head) {
	ptr current = *head;
	ptr tmp;
	while (current != NULL) {
		tmp = current->Next;
		free(current->Lable);
		free(current->type);
		free(current);
		current = tmp;
	}
	*head = NULL;
}

/*
 * safeSprintf:
 *	Safe sprintf that ensures the destination buffer is null-terminated.
 */
void safeSprintf(char *dest, size_t size, const char *format, const char *arg) {
	if (size == 0)
		return;
	sprintf(dest, format, arg);
	dest[size - 1] = '\0';
}

/*
 * main:
 *	Entry point of the assembler.
 *	For each input file:
 *		1. Generate file names for .as, .am, .ob, .ext, and .ent files.
 *		2. Free and reinitialize data structures (DC list, macros, label list).
 *		3. Perform macro expansion, first pass, and second pass.
 *	After processing all files, free all allocated memory.
 */
int main(int argc, char *argv[]) {
	int i;

	if (argc <= 1) {
		printf("Usage: %s file1 [file2 ... fileN]\n", argv[0]);
		return 1;
	}

	for (i = 1; i < argc; i++) {
		char asFile[MEMORY_SIZE];
		char amFile[MEMORY_SIZE];
		char objectFile[MEMORY_SIZE];
		char externFile[MEMORY_SIZE];
		char entryFile[MEMORY_SIZE];

		safeSprintf(asFile, sizeof(asFile), "%s.as", argv[i]);
		safeSprintf(amFile, sizeof(amFile), "%s.am", argv[i]);
		safeSprintf(objectFile, sizeof(objectFile), "%s.ob", argv[i]);
		safeSprintf(externFile, sizeof(externFile), "%s.ext", argv[i]);
		safeSprintf(entryFile, sizeof(entryFile), "%s.ent", argv[i]);

		/* Free previous data structures */
		freeDCList(&dcList);
		freeMacroList(&macroList);
		freeNodeList(&head);

		/* Reset counters */
		dcCounter = 0;
		icCounter = 0;
		lineCounter = 1;

		/* Initialize data structures */
		initStructDCList(&dcList, INITIAL_CAPACITY);
		initMacroList(&macroList, INITIAL_CAPACITY);
		initNodeList(&head);

		/* Macro expansion */
		if (!findMacro(asFile, amFile)) {
			printf("Macro expansion failed for file: %s\n", asFile);
			continue;
		}

		/* First pass */
		if (!firstPass(amFile)) {
			printf("First pass failed for file: %s\n", amFile);
			continue;
		}

		/* Second pass */
		if (!secondPass(amFile, objectFile, externFile, entryFile)) {
			printf("Second pass failed for file: %s\n", amFile);
			continue;
		}
	}

	/* Free all allocated memory at the end */
	freeDCList(&dcList);
	freeMacroList(&macroList);
	freeNodeList(&head);

	return 0;
}

