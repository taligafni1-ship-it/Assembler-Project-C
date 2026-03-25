#include "check_action_second_pass.h"
char wordBinary[]={0}; 
int wordIndex = 0;


/* 
 * addToWordBinary:
 * Adds the binary representation of 'value' with a specified number of 'bits'
 * to the global wordBinary array, updating wordIndex accordingly.
 */
void addToWordBinary(int value, int bits) {
	/* Add each bit of 'value' to wordBinary starting from the most significant bit */
	int i;
	for (i = bits - 1; i >= 0; i--) {
		wordBinary[wordIndex++] = ((value >> i) & 1) + '0';
	}
	/* Null-terminate the binary string */
	wordBinary[wordIndex] = '\0';
}

/* 
 * movPrint:
 * Encodes the 'mov' instruction into binary and appends to wordBinary.
 * The source and destination addressing modes are provided as booleans.
 */
void movPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'mov' (0) using 4 bits */
	addToWordBinary(0, 4);

	/* Encode source addressing mode (2 bits) */
	if (isSrcImmediate) addToWordBinary(0, 2);
	else if (isSrcDirect) addToWordBinary(1, 2);
	else if (isSrcMatrix) addToWordBinary(2, 2);
	else if (isSrcRegister) addToWordBinary(3, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * cmpPrint:
 * Encodes the 'cmp' instruction into binary and appends to wordBinary.
 * The source and destination addressing modes are provided as booleans.
 */
void cmpPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'cmp' (1) using 4 bits */
	addToWordBinary(1, 4);

	/* Encode source addressing mode (2 bits) */
	if (isSrcImmediate) addToWordBinary(0, 2);
	else if (isSrcDirect) addToWordBinary(1, 2);
	else if (isSrcMatrix) addToWordBinary(2, 2);
	else if (isSrcRegister) addToWordBinary(3, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestImmediate) addToWordBinary(0, 2);
	else if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * addPrint:
 * Encodes the 'add' instruction into binary and appends to wordBinary.
 * Source and destination addressing modes are provided as booleans.
 */
void addPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'add' (2) using 4 bits */
	addToWordBinary(2, 4);

	/* Encode source addressing mode (2 bits) */
	if (isSrcImmediate) addToWordBinary(0, 2);
	else if (isSrcDirect) addToWordBinary(1, 2);
	else if (isSrcMatrix) addToWordBinary(2, 2);
	else if (isSrcRegister) addToWordBinary(3, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * subPrint:
 * Encodes the 'sub' instruction into binary and appends to wordBinary.
 * Source and destination addressing modes are provided as booleans.
 */
void subPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'sub' (3) using 4 bits */
	addToWordBinary(3, 4);

	/* Encode source addressing mode (2 bits) */
	if (isSrcImmediate) addToWordBinary(0, 2);
	else if (isSrcDirect) addToWordBinary(1, 2);
	else if (isSrcMatrix) addToWordBinary(2, 2);
	else if (isSrcRegister) addToWordBinary(3, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * leaPrint:
 * Encodes the 'lea' instruction into binary and appends to wordBinary.
 * Source can only be direct or matrix; destination can be direct, matrix, or register.
 */
void leaPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'lea' (4) using 4 bits */
	addToWordBinary(4, 4);

	/* Encode source addressing mode (2 bits) - only direct or matrix allowed */
	if (isSrcDirect) addToWordBinary(1, 2);
	else if (isSrcMatrix) addToWordBinary(2, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * clrPrint:
 * Encodes the 'clr' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void clrPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'clr' (5) using 4 bits */
	addToWordBinary(5, 4);

	/* Add two bits for source addressing (always 0 for clr) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * notPrint:
 * Encodes the 'not' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void notPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'not' (6) using 4 bits */
	addToWordBinary(6, 4);

	/* Add two bits for source addressing (always 0 for not) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * incPrint:
 * Encodes the 'inc' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void incPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'inc' (7) using 4 bits */
	addToWordBinary(7, 4);

	/* Add two bits for source addressing (always 0 for inc) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}


/* 
 * decPrint:
 * Encodes the 'dec' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void decPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'dec' (8) using 4 bits */
	addToWordBinary(8, 4);

	/* Add two bits for source addressing (always 0 for dec) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * jmpPrint:
 * Encodes the 'jmp' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void jmpPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'jmp' (9) using 4 bits */
	addToWordBinary(9, 4);

	/* Add two bits for source addressing (always 0 for jmp) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * bnePrint:
 * Encodes the 'bne' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void bnePrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'bne' (10) using 4 bits */
	addToWordBinary(10, 4);

	/* Add two bits for source addressing (always 0 for bne) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * jsrPrint:
 * Encodes the 'jsr' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void jsrPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'jsr' (11) using 4 bits */
	addToWordBinary(11, 4);

	/* Add two bits for source addressing (always 0 for jsr) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * redPrint:
 * Encodes the 'red' instruction into binary and appends to wordBinary.
 * Only the destination operand is considered; source operand is unused.
 */
void redPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'red' (12) using 4 bits */
	addToWordBinary(12, 4);

	/* Add two bits for source addressing (always 0 for red) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}

/* 
 * prnPrint:
 * Encodes the 'prn' instruction into binary and appends to wordBinary.
 * Source operand is unused. Destination operand encoding is considered.
 */
void prnPrint(bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister,
			  bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister)
{
	/* Add opcode for 'prn' (13) using 4 bits */
	addToWordBinary(13, 4);

	/* Add two bits for source addressing (always 0 for prn) */
	addToWordBinary(0, 2);

	/* Encode destination addressing mode (2 bits) */
	if (isDestImmediate) addToWordBinary(0, 2);
	else if (isDestDirect) addToWordBinary(1, 2);
	else if (isDestMatrix) addToWordBinary(2, 2);
	else if (isDestRegister) addToWordBinary(3, 2);
}


/* 
 * rtsPrint:
 * Encodes the 'rts' instruction into binary and appends to wordBinary.
 * This instruction has no operands, so both source and destination are ignored.
 */
void rtsPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			  bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'rts' (14) using 4 bits */
	addToWordBinary(14, 4);

	/* Add 4 bits set to 0 since rts has no operands */
	addToWordBinary(0, 4);
}

/* 
 * stopPrint:
 * Encodes the 'stop' instruction into binary and appends to wordBinary.
 * This instruction has no operands, so both source and destination are ignored.
 */
void stopPrint(bool isSrcImmediate, bool isSrcDirect, bool isSrcMatrix, bool isSrcRegister,
			   bool isDestImmediate, bool isDestDirect, bool isDestMatrix, bool isDestRegister)
{
	/* Add opcode for 'stop' (15) using 4 bits */
	addToWordBinary(15, 4);

	/* Add 4 bits set to 0 since stop has no operands */
	addToWordBinary(0, 4);
}


/* 
 * findLabelAddress:
 * Searches the linked list of labels for a given label name.
 * Returns the address of the label if found, otherwise returns -1.
 */
int findLabelAddress(ptr head, char *label) {
	ptr current = head;

	/* Iterate through the linked list */
	while (current != NULL) {
		/* Compare current label with the target label */
		if (strcmp(current->Lable, label) == 0) {
			return current->Addres;  /* Return address if found */
		}
		current = current->Next;  /* Move to next node */
	}

	/* Label not found */
	return -1;
}

/* 
 * checkIfLabelExists:
 * Checks if a label exists in the linked list.
 * Returns 1 if found, 0 otherwise.
 */
int checkIfLabelExists(ptr head, char *label) {
	ptr current = head;

	/* Iterate through the linked list */
	while (current != NULL) {
		/* Compare current label with the target label */
		if (strcmp(current->Lable, label) == 0) {
			return 1;  /* Label exists */
		}
		current = current->Next;  /* Move to next node */
	}

	/* Label does not exist */
	return 0;
}



/*
 * entryOrExternAppears:
 *   Given a line of text, skip leading whitespace, skip the first word,
 *   then read the second word (up to whitespace or end),
 *   and return:
 *     0  if the second word is exactly ".extern"
 *     1  if the second word is exactly ".entry"
 *    -1  otherwise
 */
int entryOrExternAppears(char *line)
{
	char *p = line;
	char word2[MAX_SIZE_FOR_LINE];
	int i = 0;

	if (line == NULL) {
		return -1;
	}

	/* 1) Skip any initial whitespace */
	while (isspace(*p)) {
		p++;
	}

	/* 2) Skip over the first word */
	while (*p && !isspace(*p)) {
		p++;
	}

	/* 3) Skip whitespace before the second word */
	while (isspace(*p)) {
		p++;
	}

	/* If we reached the end—no second word */
	if (*p == '\0') {
		return -1;
	}

	/* 4) Read the second word (max 63 chars) */
	while (*p && !isspace(*p) && i < sizeof(word2) - 1) {
		word2[i++] = *p++;
	}
	word2[i] = '\0';

	/* 5) Compare the second word to ".extern" or ".entry" */
	if (strcmp(word2, ".extern") == 0) {
		return 0;
	}
	if (strcmp(word2, ".entry") == 0) {
		return 1;
	}

	return -1;
}






