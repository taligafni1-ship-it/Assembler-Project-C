/**
 * @file types.h
 * @brief This header file declares data types, structures, constants, and global variables 
 *        used throughout the assembler project.
 *
 * The purpose of this file is to provide:
 *   - Type definitions for macros, labels, data structures, and commands.
 *   - Global variables shared across different modules of the assembler.
 *   - Constant definitions for buffer sizes, limits, and action configurations.
 *
 * This centralizes common definitions and ensures consistency across the program.
 */
#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_SIZE_FOR_WORD            11
#define MAX_SIZE_FOR_LINE            80
#define MAX_LABLE_SIZE               30
#define COMMAND_NAME                 5
#define NUM_ACTION                   16
#define NUM_OF_OPERANDS_IN_ACTION    2
#define DOUBLE_THE_ZISE              2

#define SKIP_SPACES while(line[i] != '\0' && isspace(line[i])) i++;

typedef struct {
    	char *macroName;    
	char *macroContent;
    	int contentSize;
} MacroDefinition;

typedef struct {
    	MacroDefinition *macros;
    	int count;
    	int capacity;
} MacroList;

typedef struct {
    	int location;
    	char *info;
    	size_t contentSize;
} structDC;

typedef struct {
    	structDC *structs;
    	int count;
    	int capacity;
} structDCList;

typedef struct {
    	char name[COMMAND_NAME];
    	bool (*func)(char action[]);
} command;

typedef struct node {
    	char *Lable;
    	int Addres;
    	struct node *Next;
    	char *type;
} node;

typedef node* ptr;

extern int icCounter;
extern int dcCounter;


extern structDCList dcList;
extern MacroList macroList;
extern ptr head;
extern int lineCounter;
 

extern char wordBinary [MAX_SIZE_FOR_WORD]; 

extern int wordIndex;

#endif

