/**
 * @file util.h
 * @brief Utility function declarations.
 *
 * The purpose of this header file is to declare general-purpose functions 
 * that can be reused across multiple source files in the project.
 * These functions provide common checks and string manipulations 
 * needed in different modules.
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

bool isOnlySpaces(char s[]);
void removeBlanks(char s[]);
bool isCommand(char *currentName);
#endif


