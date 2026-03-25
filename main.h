/* Authors: Leah Singer, Avital Gafni
 *
 * Description:
 * This program implements an assembler that processes assembly-like source
 * code and translates it into a special binary representation expressed in
 * base-4 (quaternary).
 *
 * The assembler performs the following main tasks:
 *  1. Macro Processing: Expands macros defined in the source code.
 *  2. First Pass: Scans the code to identify labels, directives, and
 *     instructions while building symbol tables and data structures.
 *  3. Second Pass: Resolves symbol references, validates instruction formats,
 *     and generates the final encoded output.
 *  4. Output Generation: Produces the translated machine code in the
 *     special base-4 format.
 *
 * The goal of this assembler is to provide a simple yet complete
 * implementation of a two-pass assembler, with support for macros, labels,
 * data directives, and instruction encoding.
 *
 * =========================================================================
 */


#ifndef MAIN_H
#define MAIN_H
 

#include "macro.h"
#include "first_pass.h"
#include "second_pass.h"


void initMacroList(MacroList *list, int initialCapacity);
void initStructDCList(structDCList *list, int initialCapacity);
void initNodeList(ptr *head);
void freeMacroList(MacroList *list);
void freeDCList(structDCList *list);
void freeNodeList(ptr *head);
void safeSprintf(char *dest, size_t size, const char *format, const char *arg);

#endif



