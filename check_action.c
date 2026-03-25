#include "check_action.h"

#define NUM_OF_MAT_INDEX 2
#define REGISTER_ARE_IN_ONE_LINE 2
#define NUM_LINE_FOR_MAT 2
#define MIN_CHAR_SIZE 2
#define NUM_DEFULT_ADD_BYTES 4
#define MAX_REG_NUM 7

#define NOT_NULL if (token == NULL || token[0] == '\0') continue;

#define SKIP_SPACE while(line[i] != '\0' && isspace(line[i])) i++;
#define CHECK_MAT_NO_NULL if(mat[currentIndex]=='\0'){ \
		printf("missing mat placing"); \
		printf(", the error is in line %d\n", lineCounter);\
		lineCounter++;\
		return; \
	}


#define VALID_MAT \
    if (word[k] == '\0') { \
        printf("Unexpected end of string"); \
	printf(", the error is in line %d\n", lineCounter);\
				lineCounter++;\
        return false; \
    }

/**
 * Adds a new data constant (DC) to the given structDCList.
 * Allocates memory for info and expands the array if needed.
 */
void addDC(structDCList *list, int location, char *info) {
	structDC *newDC;

	/* Expand array if capacity reached */
	if (list->count == list->capacity) {
		int newCapacity = list->capacity * DOUBLE_THE_ZISE;
		structDC *newArr = realloc(list->structs, newCapacity * sizeof(structDC));
		if (newArr == NULL) {
			printf("allocation failed\n");
			return;
		}
		list->structs = newArr;
		list->capacity = newCapacity;
	}

	newDC = &list->structs[list->count];
	newDC->location = location;

	/* Allocate memory for info string */
	newDC->info = malloc(strlen(info) + NUM_DEFULT_ADD_BYTES);
	if (newDC->info == NULL) {
		printf("memory allocation failed\n");
		return;
	}

	strcpy(newDC->info, info);
	list->count++;
}

/**
 * Checks if the input string is a valid code label.
 * Must start with a letter, end with ':', and not be a command.
 */
bool codeLable(char input[]) {
	int i = 0;
	while (input[i] != '\0' && isspace((unsigned char)input[i])) {
		i++;
	}
	if (input[i] == '\0' || !isalpha((unsigned char)input[i])) {
		return false;
	}
	while (input[i] != '\0' && isalpha((unsigned char)input[i])) {
		i++;
	}
	if (input[i] != ':') {
		return false;
	}
	i++;
	while (input[i] != '\0' && isspace((unsigned char)input[i])) {
		i++;
	}
	if (input[i] == '.') {
		return false;
	}
	return true;
}

/**
 * Duplicates a string using dynamic memory allocation.
 * Returns pointer to new string or NULL on failure.
 */
char *strdup(const char *s) {
	size_t len;
	char *dup;
	size_t i;

	if (s == NULL) return NULL;

	len = strlen(s) + 1;
	dup = (char *)malloc(len);
	if (dup == NULL) return NULL;

	for (i = 0; i < len; i++) {
		dup[i] = s[i];
	}

	return dup;
}


/**
 * Validates a label string.
 * Checks length, colon presence, allowed characters, and reserved keywords.
 * If needColon is true, label must end with ':'.
 */
bool validLabel(char word[], bool needColon) {
	int len;
	int i = 0;

	/* Ensure single word */
	if(!dontHaveSpaceBetweenChar(word)) {
		printf("invalid label, not one word, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	removeBlanks(word);
	len = strlen(word);

	if (needColon) {
		if (len > MAX_LABLE_SIZE + 1) {
			printf("label too long (max %d chars plus colon), the error is in line %d\n", MAX_LABLE_SIZE, lineCounter);
			lineCounter++;
			return false;
		}
		if (word[len - 1] != ':') {
			printf("label must end with a colon, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		word[len - 1] = '\0';
		len--;

		if (isCommand(word)) {
			printf("label is a saved word, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
	} else {
		if (len > MAX_LABLE_SIZE) {
			printf("label too long (max %d chars), the error is in line %d\n", MAX_LABLE_SIZE, lineCounter);
			lineCounter++;
			return false;
		}
		for (i = 0; i < len; i++) {
			if (word[i] == ':') {
				printf("colon not allowed in label, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
		}
		if (isCommand(word)) {
			printf("label is a saved word, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
	}

	/* First character must be a letter */
	if (!isalpha((unsigned char)word[0])) {
		printf("label must start with a letter, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	i = 0;
	while (i < len && isalpha((unsigned char)word[i])) {
		i++;
	}

	/* Remaining characters must be digits */
	while (i < len) {
		if (!isdigit((unsigned char)word[i])) {
			printf("invalid label, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		i++;
	}

	return true;
}


/* 
 * removeSpacesBeforeBracket - Removes any whitespace characters immediately 
 * before an opening square bracket '[' in the input string.
 *
 * @str: The input string to be modified in-place.
 *
 * This function iterates over the input string and copies characters to a 
 * temporary buffer. Whenever it encounters a '[', it removes any spaces 
 * immediately preceding it before copying the '[' itself. Finally, it 
 * copies the result back to the original string.
 */
void removeSpacesBeforeBracket(char *str) 
{
	int i, j = 0;
	char result[MAX_SIZE_FOR_LINE];

	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == '[') {
			/* Remove any whitespace before '[' */
			while (j > 0 && isspace((unsigned char)result[j - 1])) {
				j--;
			}
		}
		result[j++] = str[i];
	}
	result[j] = '\0';
	strcpy(str, result);
}


/* 
 * hasOnlyOneOperand - Checks whether the input string contains exactly one operand.
 *
 * @operands: The string containing operands separated by commas and/or spaces.
 *
 * Returns true if there is exactly one operand, false otherwise.
 *
 * This function removes spaces before brackets, then iterates through the string 
 * while tracking words, commas, and operand count. It prints errors for:
 *  - too many operands
 *  - too few operands
 *  - two consecutive commas
 *  - extra comma at the end
 */
bool hasOnlyOneOperand(char *operands) 
{
	int i = 0;
	bool inWord = false;
	bool foundComma = false;
	bool firstOperand = false;
	bool hasOperand = false;
	bool extraComma = false;
	bool prevComma = false;
	char line[MAX_SIZE_FOR_LINE];
	
	strcpy(line, operands);
	removeSpacesBeforeBracket(line);

	while (line[i] != '\0') {
		if (!isspace((unsigned char)line[i]) && line[i] != ',') {
			if (!inWord) {
				inWord = true;
				if (!firstOperand) {
					firstOperand = true;
					hasOperand = true;
				} else if (!foundComma) {
					printf("too many operands, the error is in line %d\n", lineCounter);
					lineCounter++;
					return false;
				}
			}
			prevComma = false;
		} else if (line[i] == ',') {
			if (prevComma) {
				printf("two consecutive commas, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			prevComma = true;

			if (foundComma && !inWord) {
				extraComma = true;
			}
			foundComma = true;
			inWord = false;
		} else if (isspace((unsigned char)line[i])) {
			inWord = false;
		}
		i++;
	}

	if (!hasOperand) {
		printf("too few operands, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	if (extraComma) {
		printf("extra comma at the end, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	return true;
}



/** 
 * Checks if the input string contains exactly two operands separated by a comma.
 * Returns true if valid, false otherwise. Prints errors with line number when invalid.
 */
bool hasTwoOperands(char *operands) 
{
	int i = 0;
	bool inWord = false;
	bool foundComma = false;
	bool firstOperand = false;
	bool secondOperand = false;
	bool hasOperand = false;
	bool prevComma = false;
	bool extraComma = false;
	char line[MAX_SIZE_FOR_LINE];

	strcpy(line, operands); 
	removeSpacesBeforeBracket(line);  /** remove spaces before any '[' character */

	while (line[i] != '\0') 
	{
		if (!isspace(line[i]) && line[i] != ',') 
		{
			if (!inWord) 
			{
				if (firstOperand && !foundComma) 
				{
					/** two words without comma */
					printf("missing comma between operands, the error is in line %d\n", lineCounter);
					lineCounter++;
					return false;
				}
				inWord = true;
				if (!firstOperand) 
				{
					firstOperand = true;
				} 
				else if (!secondOperand && foundComma) 
				{
					secondOperand = true;
				}
				hasOperand = true;
			}
			prevComma = false;
		} 
		else if (line[i] == ',') 
		{
			if (!firstOperand) 
			{
				printf("comma but no first operand, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			if (prevComma) 
			{
				printf("two consecutive commas, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			prevComma = true;

			if (secondOperand) 
			{
				extraComma = true; /** extra comma after second operand */
			}
			foundComma = true;
			inWord = false;
		} 
		else if (isspace(line[i])) 
		{
			inWord = false; /** end of current operand */
		}
		i++;
	}

	if (!hasOperand) 
	{
		printf("no operands where required, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	if (extraComma) 
	{
		printf("extra comma at the end, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	if (foundComma && !secondOperand) 
	{
		printf("missing operand after comma, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	if (!foundComma && firstOperand && !secondOperand) 
	{
		/** single operand without comma */
		printf("too few operands, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	return true; /** exactly two operands with valid comma separation */
}





/**
 * Checks if the given macro content is unique in the global macroList.
 * Returns true if no other macro has the same content, false otherwise.
 */
bool isUniqueMacroContent(char *content) 
{
	int i;
	for (i = 0; i < macroList.count; i++) 
	{
		/** compare each existing macro's content with the given content */
		if (strcmp(macroList.macros[i].macroContent, content) == 0) 
			return false; /** content already exists in macroList */
	}
	return true; /** content is unique */
}




/**
 * Checks if the given string represents a positive integer.
 * Returns true if the string is a valid positive integer, false otherwise.
 */
bool isPositiveIntegerString(const char *str) 
{
	int i = 0;

	/** check for null or empty string */
	if (!str || !*str) {
		return false;
	}

	/** skip leading spaces */
	while (isspace((unsigned char)str[i])) {
		i++;
	}

	/** first non-space character must be a digit */
	if (!isdigit((unsigned char)str[i])) {
		return false;
	}

	/** leading zero followed by other digits is invalid */
	if (str[i] == '0' && isdigit((unsigned char)str[i+1])) {
		return false;
	}

	/** check remaining characters */
	for (; str[i] != '\0'; i++) {
		if (isspace((unsigned char)str[i])) {
			/** skip trailing spaces */
			while (isspace((unsigned char)str[i])) {
				i++;
			}
			break;
		}
		if (!isdigit((unsigned char)str[i])) {
			return false;
		}
	}

	/** return true only if the rest of the string is null (end) */
	return str[i] == '\0';
}


/**
 * Checks if the input string contains only one word (no spaces between characters).
 * Returns true if the string is a single word, false if it contains more than one word.
 */
bool dontHaveSpaceBetweenChar(char *str) 
{
	int i = 0;
	bool in_word = false;
	int word_count = 0;

	while (str[i] != '\0') 
	{
		if (!isspace((unsigned char)str[i])) 
		{
			if (!in_word) 
			{
				in_word = true;
				word_count++;
				/** if more than one word is found, return false */
				if (word_count > 1) 
				{
					return false;
				}
			}
		} 
		else 
		{
			in_word = false;
		}
		i++;
	}

	return true;
}


/**
 * Checks if the input string represents a valid number.
 * Supports optional leading '+' or '-' and a single decimal point.
 * Returns true if the string is a valid number, false otherwise.
 */
bool isValidNumber(char input[])
{
	int i = 0;
	bool has_digit = false;
	bool has_dot = false;

	/** skip leading spaces */
	while (isspace((unsigned char)input[i])) 
	{
		i++;
	}

	/** check optional sign */
	if (input[i] == '-' || input[i] == '+') 
	{
		i++;
	}

	/** check digits and optional single dot */
	while (input[i] != '\0' && !isspace((unsigned char)input[i])) 
	{
		if (isdigit((unsigned char)input[i])) 
		{
			has_digit = true;
		} 
		else if (input[i] == '.') 
		{
			if (has_dot)
				return false; /** multiple dots invalid */
			has_dot = true;
		} 
		else 
		{
			return false; /** invalid character */
		}
		i++;
	}

	/** skip trailing spaces */
	while (isspace((unsigned char)input[i])) 
	{
		i++;
	}

	/** if extra characters exist after number, invalid */
	if (input[i] != '\0') 
	{
		return false;
	}

	return has_digit; /** must have at least one digit */
}



/**
 * Removes all whitespace characters from the input string.
 * The result is stored back in the original string.
 */
void removeBlanks(char s[])
{
	int i;
	int j = 0;
	int len = strlen(s);
	char *ss; 
	ss = (char*)malloc(len + 1);

	/** Check if memory allocation failed */
	if (ss == NULL) {
		fprintf(stderr, "memory allocation failed in removeBlanks!\n");
		return;
	}

	/** Copy non-space characters to new buffer */
	for (i = 0; i < len; i++)
	{
		if (!isspace((unsigned char)s[i]))
		{
			ss[j] = s[i];
			j++;
		}
	}

	ss[j] = '\0';

	/** Copy the result back to the original string */
	strcpy(s, ss);

	free(ss);
}


/**
 * Processes the operands for the 'lea' instruction.
 * Validates source and destination operands, checks for matrices, labels, and registers.
 * Updates the instruction counter (icCounter) accordingly.
 */
bool lea(char operands[])
{
	int i = 0, n = 0, l = 0, numReg = 0;
	char *token;
	char operand[MAX_SIZE_FOR_LINE] = {0};
	char subInput[MAX_SIZE_FOR_LINE] = {0};
	bool checked = false, secondOperand = false; 

	/** Ensure exactly two operands */
	if(!hasTwoOperands(operands))
		return false;

	strcpy(subInput, operands);
	token = strtok(subInput, ",");

	for (i = 0; i < NUM_OF_OPERANDS_IN_ACTION; i++) 
	{
		/** Skip leading spaces */
		while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
			token++;

		/** Check missing argument */
		if (token == NULL || token[0] == '\0') {
			printf("missing argument, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		checked = false;

		if (!secondOperand) {
			/** Process source operand */
			if (isalpha((unsigned char)token[0])) {
				bool close1 = false, close2 = false, start = false;
				char matName[MAX_SIZE_FOR_LINE] = {0};
				int b = 0;
				n = l = 0;

				/** Separate matrix name and indices */
				while (token[n] != '\0' && !close2) {
					if (token[n] == '[') start = true;
					if (!start) {
						matName[b++] = token[n];
					} else {
						operand[l++] = token[n];
					}
					if (token[n] == ']') {
						if (!close1) close1 = true;
						else close2 = true;
					}
					operand[l] = token[n];
					n++;
				}
				operand[l] = '\0';
				matName[b] = '\0';

				/** Validate matrix or label */
				if (close1) {
					if (!validLabel(matName, false)) return false;
					if (!isValidMat(operand)) return false;
					checked = true;
					icCounter += NUM_LINE_FOR_MAT;
				} else {
					if (!validLabel(token, false)) return false;
					checked = true;
					icCounter++;
				}
			} else {
				printf("invalid source operand (not matrix or label), error in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
		} else {
			/** Process destination operand */
			if (token[0] == 'r' && !checked) 
			{
				int regNum;
				token++;
				if (token[0] != '\0' && isdigit((unsigned char)token[0]))
				{
					regNum = token[0] - '0';
					if (regNum >= 0 && regNum <= MAX_REG_NUM)
					{
						token++;
						if (token[0] == '\0' || token[0] == ',' || isspace((unsigned char)token[0]))
						{
							checked = true;
							icCounter++;
							numReg++;
						}
						token--;
					}
				}
				token--;
			}

			if (isalpha((unsigned char)token[0]) && !checked) {
				bool close1 = false, close2 = false, start = false;
				char matName[MAX_SIZE_FOR_LINE] = {0};
				int b = 0;
				n = l = 0;

				while (token[n] != '\0' && !close2) {
					if (token[n] == '[') start = true;
					if (!start) matName[b++] = token[n];
					else operand[l++] = token[n];

					if (token[n] == ']') {
						if (!close1) close1 = true;
						else close2 = true;
					}
					n++;
				}

				operand[l] = '\0';
				matName[b] = '\0';

				if(close2)
				{
					if(validLabel(matName, false)){
						if(isValidMat(operand)){
							checked = true;
							icCounter += NUM_LINE_FOR_MAT;
						} else return false;
					} else return false;
				}
				else if(!close1){
					if(validLabel(token, false)){
						checked = true;
						icCounter++;
					} else return false;
				}
			}

			/** Invalid destination operand */
			if (!checked) {
				printf("invalid destination operand, error in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
		}

		secondOperand = true;
		token = strtok(NULL, ",");
	}

	/** Adjust instruction counter if both operands are registers */
	if (numReg == REGISTER_ARE_IN_ONE_LINE) {
		icCounter--;
	}

	/** Check for extraneous text after operands */
	if (token != NULL) {
		removeBlanks(token);
		if (token[0] != '\0') {
			printf("Extraneous text after end of command, error in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
	}

	return true;
}


/**
 * Processes the operands for the 'cmp' instruction.
 * Validates each operand: can be a number (prefixed by #), register (r0–r7), or a matrix/label.
 * Updates the instruction counter (icCounter) for each valid operand.
 * Detects errors such as missing arguments, invalid formats, or extraneous text.
 */
bool cmp(char operands[])
{
	int i = 0, n = 0, l = 0, numReg = 0;
	char *token;
	char operand[MAX_SIZE_FOR_LINE] = {0};
	char subInput[MAX_SIZE_FOR_LINE] = {0}; /* Buffer for parsing arguments after command name */
	bool checked = false; 

	/** Ensure there are exactly two operands */
	if (!hasTwoOperands(operands))
		return false;

	/** Copy operands to avoid modifying original string */
	strcpy(subInput, &operands[0]); 
	token = strtok(subInput, ",");

	for (i = 0; i < NUM_OF_OPERANDS_IN_ACTION; i++)
	{
		/** Skip leading spaces */
		while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
			token++;

		/** Check for missing argument */
		if (token == NULL || token[0] == '\0')
		{
			printf("missing argument, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** Check for number operand (prefixed by #) */
		if (token != NULL && token[0] == '#' && !checked)
		{
			checked = true;
			token++;
			if ((token[0] != '\0' && isspace((unsigned char)token[0])) || token[0] == '\0') {
				printf("missing number after #, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			if (token[0] != '\0' && !isValidNumber(token)) {
				printf("invalid number, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			token--;
			icCounter++;
		}

		/** Check for register operand (r0–r7) */
		if (token != NULL && token[0] == 'r' && !checked)
		{
			int regNum;
			token++;
			if (token[0] != '\0' && isdigit((unsigned char)token[0]))
			{
				regNum = token[0] - '0';
				if (regNum >= 0 && regNum <= MAX_REG_NUM)
				{
					token++;
					if (token[0] == '\0' || token[0] == ',' || isspace((unsigned char)token[0]))
					{
						checked = true;
						icCounter++;
						numReg++;
					}
					token--;
				}
			}
			token--;
		}

		/** Check for matrix/label operand */
		if (token != NULL && isalpha((unsigned char)token[0]) && !checked)
		{
			bool close1 = false, close2 = false, start = false;
			char matName[MAX_SIZE_FOR_LINE] = {0};
			int b = 0;
			n = l = 0;

			while (token[n] != '\0' && !close2)
			{
				if (token[n] == '[') start = true;
				if (!start) matName[b++] = token[n];
				else operand[l++] = token[n];

				if (token[n] == ']') {
					if (!close1) close1 = true;
					else close2 = true;
				}
				n++;
			}

			operand[l] = '\0';
			matName[b] = '\0';

			if (close2)
			{
				if (validLabel(matName, false) && isValidMat(operand)) {
					checked = true;
					icCounter += NUM_LINE_FOR_MAT;
				} else return false;
			}
			else if (!close1)
			{
				if (validLabel(token, false)) {
					checked = true;
					icCounter++;
				} else return false;
			}
		}

		/** Number without # prefix is invalid */
		if ((token[0]=='-' || token[0] == '+' || isdigit((unsigned char)token[0])) && !checked)
		{
			printf("missing # in the start of the number, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** If operand still not recognized */
		if (!checked)
		{
			printf("invalid value, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** Get the next token */
		token = strtok(NULL, ","); 
		checked = false;
		memset(operand, 0, sizeof(operand));
		n = 0;
		l = 0;
	}

	/** Adjust IC if both operands are registers */
	if (numReg == REGISTER_ARE_IN_ONE_LINE)
	{
		icCounter--;
	}

	/** Remove blanks and check for extraneous text */
	if (token != NULL) removeBlanks(token);

	if (token != NULL && token[0] != '\0')
	{
		printf("Extraneous text after end of command, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	return true;
}



/**
 * Processes the operand for the 'prn' instruction.
 * Validates that there is exactly one operand which can be:
 *   - A number (prefixed by #)
 *   - A register (r0–r7)
 *   - A label or matrix
 * Updates the instruction counter (icCounter) accordingly.
 * Detects errors like missing operand, extra operands, invalid formats, or extraneous text.
 */
bool prn(char operands[]) 
{
	int n = 0, l = 0;
	char *token;
	char subInput[MAX_SIZE_FOR_LINE] = {0}; /* Buffer to avoid modifying original string */
	bool checked = false;

	/** Ensure there is only one operand */
	if (!hasOnlyOneOperand(operands))
		return false;

	strcpy(subInput, operands);
	token = strtok(subInput, " \t");

	/** Skip leading spaces */
	while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
		token++;

	/** Check for missing operand */
	if (token == NULL || token[0] == '\0') {
		printf("Missing argument, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check that there are no commas (more than one operand) */
	if (strchr(operands, ',') != NULL) {
		printf("More than one operand was given, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
		token++;

	/** Check for number operand (prefixed by #) */
	if (token != NULL && token[0] == '#' && !checked) 
	{
		checked = true;
		token++;
		if ((token[0] != '\0' && isspace((unsigned char)token[0])) || token[0] == '\0') {
			printf("Missing number after #, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}	
		if (token[0] != '\0' && !isValidNumber(token)) {
			printf("Invalid number, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		token--;
		icCounter++;	
	}

	/** Number without # prefix is invalid */
	if ((token[0]=='-' || token[0] == '+' || isdigit((unsigned char)token[0])) && !checked) {
		printf("Missing # in the start of the number, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check for register operand */
	if (token[0] == 'r' && !checked) 
	{
		int regNum;
		token++;
		if (token[0] != '\0' && isdigit((unsigned char)token[0])) {
			regNum = token[0] - '0';
			if (regNum >= 0 && regNum <= MAX_REG_NUM) {
				token++;
				if (token[0] == '\0' || token[0] == ',' || isspace((unsigned char)token[0])) {
					checked = true;
					icCounter++;
				}
				token--;
			}
		}
		token--;
	}

	/** Check for label or matrix operand */
	if (isalpha((unsigned char)token[0]) && !checked) {
		char label[MAX_SIZE_FOR_LINE] = {0};
		n = l = 0;

		while (token[n] != '\0')
			label[l++] = token[n++];
		label[l] = '\0';

		token = strtok(NULL, "\n");

		if (token == NULL) {
			if (validLabel(label, false)) {
				checked = true;
				icCounter++;
			} else {
				return false;
			}
		} else {
			if (validLabel(label, false)) {
				if (isValidMat(token)) {
					checked = true;
					icCounter += NUM_LINE_FOR_MAT;
				} else return false;
			} else return false;
		}
	}

	/** If operand still not recognized */
	if (!checked) {
		printf("Invalid value, error in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check for extraneous text after operand */
	token = strtok(NULL, " \t");
	if (token != NULL) {
		removeBlanks(token);
		if (token[0] != '\0') {
			printf("Extraneous text after end of command, error in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
	}

	return true;
}

/**
 * Validates and processes a two-operand instruction.
 * Each operand can be:
 *   - A number (prefixed by #, only for the first operand)
 *   - A register (r0–r7)
 *   - A label
 *   - A matrix (e.g., label[...])
 * Updates the instruction counter (icCounter) according to operand type.
 * Detects errors like missing operands, invalid formats, or extraneous text.
 */
bool twoOperandAction(char operands[]) 
{
	int i = 0, n = 0, l = 0, numReg = 0;
	char *token;
	char operand[MAX_SIZE_FOR_LINE] = {0};
	char subInput[MAX_SIZE_FOR_LINE] = {0}; /* Buffer for parsing operands */
	bool checked = false, isMat = false, secondOperand = false;

	/** Ensure there are exactly two operands */
	if (!hasTwoOperands(operands))
		return false;

	strcpy(subInput, &operands[0]); /* Avoid modifying original string */
	token = strtok(subInput, ",");

	for (i = 0; i < NUM_OF_OPERANDS_IN_ACTION; i++) 
	{
		/** Skip leading spaces */
		while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
			token++;

		/** Check for missing operand */
		if (token == NULL || token[0] == '\0') {
			printf("Missing argument, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** Numbers are invalid as second operand */
		if (secondOperand && token != NULL && token[0] == '#' && !checked) {
			printf("Invalid destination operand, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** Check for number operand (first operand only) */
		if (token != NULL && token[0] == '#' && !checked) 
		{
			checked = true;
			token++;
			if ((token[0] != '\0' && isspace((unsigned char)token[0])) || token[0] == '\0') {
				printf("Missing number after #, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}	
			if (token[0] != '\0' && !isValidNumber(token)) {
				printf("Invalid number, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
			token--;
			icCounter++;
		}

		/** Check for register operand */
		if (token != NULL && token[0] == 'r' && !checked) 
		{
			int regNum;
			token++;
			if (token[0] != '\0' && isdigit((unsigned char)token[0])) {			
				regNum = token[0] - '0';
				if (regNum >= 0 && regNum <= MAX_REG_NUM) {	
					token++;
					if (token[0] == '\0' || token[0] == ',' || isspace((unsigned char)token[0])) {
						checked = true;
						icCounter++;
						numReg++;
					}
					token--;			
				}
			}	
			token--;
		}

		/** Check for matrix operand */
		if (token != NULL && isalpha((unsigned char)token[0]) && !checked)	
		{
			bool close1 = false, close2 = false, start = false;
			char matName[MAX_SIZE_FOR_LINE] = {0};
			int b = 0;

			while (token[n] != '\0' && !close2) {
				if (token[n] == '[')
					start = true;
				if (!start) {
					matName[b++] = token[n];
				} else {
					operand[l++] = token[n];
				}
				if (token[n] == ']') {
					if (!close1)
						close1 = true;
					else
						close2 = true;
				}
				n++;
			}
			operand[l] = '\0';
			matName[b] = '\0';

			if (close1) {
				if (!validLabel(matName, false))
					return false;	
				if (!isValidMat(operand))
					return false;
				checked = true;
				isMat = true;
				icCounter += 2; /* Matrix occupies two instruction lines */
			}
			memset(matName, 0, sizeof(matName));
			b = 0;
		}

		/** Check for label operand (non-matrix) */
		if (token != NULL && isalpha((unsigned char)token[0]) && !checked && !isMat)	
		{
			if (!validLabel(token, false))
				return false;
			icCounter++;
			checked = true;
		}

		/** Number without # prefix is invalid */
		if ((token[0] == '-' || token[0] == '+' || isdigit((unsigned char)token[0])) && !checked) {
			printf("Missing # in the start of the number, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** If operand still not recognized */
		if (!checked) {
			printf("Invalid value, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/** Reset for next operand */
		isMat = false;
		token = strtok(NULL, ","); /* Get next operand */
		checked = false;
		memset(operand, 0, sizeof(operand));
		n = 0;
		l = 0;
		secondOperand = true;
	}

	/** Adjust for two registers in one instruction */
	if (numReg == REGISTER_ARE_IN_ONE_LINE) {
		icCounter--;
	}

	/** Check for extraneous text after operands */
	if (token != NULL) {
		removeBlanks(token);
	}
	if (token != NULL && token[0] != '\0') {
		printf("Extraneous text after end of command, error in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	return true;
}




/**
 * Validates and processes a one-operand instruction.
 * The operand can be:
 *   - A register (r0–r7)
 *   - A label
 *   - A matrix (e.g., label[...])
 * Numbers (prefixed by #) are invalid as operands here.
 * Updates the instruction counter (icCounter) according to operand type.
 * Detects errors like missing operand, invalid format, or extraneous text.
 */
bool oneOperandAction(char operands[]) 
{
	int n = 0, l = 0;
	char *token;
	char subInput[MAX_SIZE_FOR_LINE] = {0};
	bool checked = false;

	/** Ensure there is only one operand */
	if (!hasOnlyOneOperand(operands))
		return false;

	strcpy(subInput, operands);
	token = strtok(subInput, " \t");

	/** Skip leading spaces */
	while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
		token++;

	if (token == NULL || token[0] == '\0') {
		printf("Missing argument, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Skip any remaining leading spaces */
	while (token != NULL && token[0] != '\0' && isspace((unsigned char)token[0]))
		token++;

	/** Only one operand is allowed */
	if (strchr(operands, ',') != NULL) {
		printf("More than one operand was given, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check for invalid number operand */
	if (token != NULL && token[0] == '#' && !checked) 
	{
		checked = true;
		token++;
		if ((token[0] != '\0' && isspace((unsigned char)token[0])) || token[0] == '\0') {
			printf("Invalid value, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		if (token[0] != '\0' && isValidNumber(token)) {
			printf("Invalid destination operand, cannot be a number, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		token--;
	}

	/** Numbers without # prefix are invalid */
	if ((token[0] == '-' || token[0] == '+' || isdigit((unsigned char)token[0])) && !checked) {
		printf("Invalid value, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check for register operand */
	if (token[0] == 'r' && !checked) 
	{
		int regNum;
		token++;
		if (token[0] != '\0' && isdigit((unsigned char)token[0])) {
			regNum = token[0] - '0';
			if (regNum >= 0 && regNum <= MAX_REG_NUM) {
				token++;
				if (token[0] == '\0' || token[0] == ',' || isspace((unsigned char)token[0])) {
					checked = true;
					icCounter++;
				}
				token--;
			}
		}
		token--;
	}

	/** Check for label or matrix operand */
	if (isalpha((unsigned char)token[0]) && !checked) {
		char label[MAX_SIZE_FOR_LINE] = {0};
		n = l = 0;
		while (token[n] != '\0')
			label[l++] = token[n++];
		label[l] = '\0';

		token = strtok(NULL, "\n");
		if (token == NULL) {
			if (validLabel(label, false)) {
				checked = true;
				icCounter++;
			} else
				return false;
		} else {
			if (validLabel(label, false)) {
				if (isValidMat(token)) {
					checked = true;
					icCounter += NUM_LINE_FOR_MAT;
				} else
					return false;
			} else
				return false;
		}
	}

	/** Operand still not recognized */
	if (!checked) {
		printf("Invalid value, error in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	/** Check for extraneous text after operand */
	token = strtok(NULL, " \t");
	if (token != NULL) {
		removeBlanks(token);
		if (token[0] != '\0') {
			printf("Extraneous text after end of command, error in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
	}

	return true;
}

/**
 * Checks a zero-operand instruction.
 * Returns true if no operand is given, false otherwise.
 * Prints an error message if an operand is present.
 */
bool zeroOperandAction(char operands[])
{
	if (operands == NULL || operands[0] == '\0') {
		return true;
	} else {
		printf("This action does not take any operands");
		printf(", the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}
}

/**
 * Checks if a string represents a valid register.
 * Valid registers are 'r0' to 'r7', possibly followed by spaces or a comma.
 */
bool isValidReg(char mat[])
{
	int regNum, i = 0;

	/** Must start with 'r' */
	if (mat[i] != 'r') {			
		return false;
	}
	i++;

	/** Must have a digit after 'r' */
	if (mat[i] == '\0' || !isdigit((unsigned char)mat[i])) {
		return false;
	}

	regNum = mat[i] - '0';
	if (regNum < 0 || regNum > MAX_REG_NUM) {
		return false;
	}
	i++;

	/** After the digit, only space, comma, or end of string is allowed */
	if (mat[i] == '\0' || isspace((unsigned char)mat[i]) || mat[i] == ',') {
		return true;
	}

	return false;
}


/**
 * Checks if a string represents a valid matrix access.
 * A valid matrix has exactly NUM_OF_MAT_INDEX indices inside square brackets, 
 * each index being a valid register without spaces.
 * Returns true if valid, false otherwise.
 */
bool isValidMat(char word[])
{
	int m = 0, d = 0, k = 0;
	char operand[MAX_SIZE_FOR_LINE] = {0};
	bool start = false;

	for (m = 0; m < NUM_OF_MAT_INDEX; m++) 
	{
		/* Skip leading spaces */
		while (word[k] != '\0' && isspace((unsigned char)word[k]))
			k++;

		if (word[k] == '\0') {
			printf("Missing information, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/* Expect opening bracket */
		if (word[k] != '[') {
			printf("Missing [, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}
		k++;

		/* Skip spaces after '[' */
		while (word[k] != '\0' && isspace((unsigned char)word[k]))
			k++;

		d = 0;
		/* Collect content inside brackets */
		while (word[k] != '\0' && word[k] != ']') {
			operand[d++] = word[k++];
		}
		operand[d] = '\0';

		if (!dontHaveSpaceBetweenChar(operand)) {
			printf("Invalid matrix content, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		/* Expect closing bracket */
		if (word[k] != ']') {
			printf("Missing ], the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		if (!isValidReg(operand)) {
			printf("Invalid matrix content, the error is in line %d\n", lineCounter);
			lineCounter++;
			return false;
		}

		k++;

		/* Check that consecutive indices have no extra characters between them */
		if (!start) {
			if (word[k] != '[') {
				printf("Invalid space between indexes, the error is in line %d\n", lineCounter);
				lineCounter++;
				return false;
			}
		}
		start = true;
	}

	/* After processing all indices, only spaces or end-of-string are allowed */
	if (!isspace((unsigned char)word[k]) && word[k] != '\0') {
		printf("Extraneous text after end of matrix, the error is in line %d\n", lineCounter);
		lineCounter++;
		return false;
	}

	return true;
}


int checkDC(char line[])
{
	/** Initialize counters and buffers */
	int i = 0, j = 0, k = 0, m = 0, d = 0, a = 0;
	int startCountDc = dcCounter;  /* Save initial dcCounter value */
	char word[MAX_SIZE_FOR_LINE] = {0}, num[MAX_SIZE_FOR_LINE] = {0};
	char *token, temp[MIN_CHAR_SIZE] = {0}, subInput[MIN_CHAR_SIZE] = {0};
	bool foundDCNameType = false;

	SKIP_SPACE  /* Skip initial spaces */

	i++; /* Skip the '.' character */

	/** Read DC type (string, data, mat) */
	while(line[i] != '\0' && !isspace(line[i]))
	{
		word[j] = line[i];
		j++;
		i++;
	}
	word[j] = '\0';

	/** Check if line ended without information */
	if(line[i] == '\0'){
		printf("missing information");
		printf(", the error is in line %d\n", lineCounter);
		lineCounter++;
		return -1;
	}

	/** Handle .string type */
	if((strcmp(word, "string") == 0)){
		foundDCNameType = true;
		memset(word, 0, sizeof(word));
		SKIP_SPACE
		j = 0;
		while(line[i] != '\0' && !isspace(line[i])) 
		{
			word[j] = line[i];
			j++;
			i++;
		}
		word[j] = '\0';

		/** Empty string case "" */
		if (strcmp(word, "\"\"") == 0){
			temp[0] = '\0';
			addDC(&dcList, dcCounter, temp);
			dcCounter++;
			return startCountDc;
		}

		/** Check for missing quotation marks */
		if((word[0] != '"') || (word[j-1] != '"')){
			printf("missing quotation marks");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}

		/** Validate string content: only letters */
		for(k = 1; k < j - 1; k++)
		{
			if(!isalpha(word[k])){
				printf("the given string is not only letters");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;	
			}
		}

		/** Add each character to dcList */
		for(k = 1; k < j - 1; k++) 
		{
			temp[0] = word[k];
			temp[1] = '\0';    
			addDC(&dcList, dcCounter, temp);
			dcCounter++;
		}

		/** Add string termination '\0' */
		temp[0] = '\0';
		dcCounter++;
		addDC(&dcList, dcCounter, temp);
	}

	/** Handle .data type */
	if((strcmp(word, "data") == 0)){
		foundDCNameType = true;
		memset(word, 0, sizeof(word));
		SKIP_SPACE
		j = 0;
		while (line[i] != '\0') {
			word[j] = line[i];
			j++;
			i++;
		}
		word[j] = '\0';

		removeBlanks(word);

		/** Check for missing data */
		if(word[0] == '\0'){
			printf("no information given");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}

		/** Check for leading comma */
		if(word[0] == ','){
			printf("the information starts with comma");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}

		/** Check for consecutive commas */
		for (a = 0; word[a] != '\0'; a++) 
		{
			if (word[a] == ','  && word[a + 1] != '\0' && word[a + 1] == ',') {
				printf("Multiple consecutive commas in .data, the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}
		}

		strcpy(subInput, word);

		/** Add each number to dcList */
		token = strtok(subInput, ",");
		while (token != NULL) 
		{
			if (!isValidNumber(token)) {
				printf("Invalid information in .data, not all chars are numbers");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}
			addDC(&dcList, dcCounter, token);
			dcCounter++;
			token = strtok(NULL, ",");
		}
	}

	/** Handle .mat type */
	if(strcmp(word, "mat") == 0)
	{ 
		int numDigits = 1;
		foundDCNameType = true;
		memset(word, 0, sizeof(word));
		SKIP_SPACE
		j = 0;

		while(!isspace(line[i])) 
		{
			word[j] = line[i];
			j++;
			i++;
		}

		word[j] = '\0';

		if(word[0] == '\0'){
			printf("no information given");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}

		/** Process each matrix index */
		for(m = 0; m < NUM_OF_MAT_INDEX; m++)
		{
			VALID_MAT
			memset(num, 0, sizeof(num));

			if(word[k] != '['){
				printf("missing [");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}
			k++;

			VALID_MAT
			d = 0;
			while(word[k] != '\0' && word[k] != ']'){
				num[d] = word[k];
				k++;
			}

			if(word[k] != ']'){
				printf("missing ]");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}

			if(!isPositiveIntegerString(num)){
				printf("number include objects that are not digits");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			} else
				numDigits = numDigits * atoi(num);

			k++;
		}

		/** Read matrix content */
		j = 0;	
		memset(word, 0, sizeof(word));
		while(line[i] != '\0') 
		{
			word[j] = line[i];
			j++;
			i++;
		}

		removeBlanks(word);

		/** Validate each character */
		for(d = 0; d < strlen(word); d++)
		{
			temp[0] = word[d];
			temp[1] = '\0'; 
			if(!isValidNumber(temp) && (temp[0] != ',')){
				printf("invalid information");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}
		}

		if(temp[0] == ','){
			printf("Extraneous comma after end of command");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}

		/** Insert matrix numbers one by one */
		token = strtok(word, ",");
		while(token != NULL)
		{
			numDigits--;
			if (token[0] == '\0'){
				printf("Multiple consecutive commas in string of digits");
				printf(", the error is in line %d\n", lineCounter);
				lineCounter++;
				return -1;
			}
			addDC(&dcList, dcCounter, token);
			dcCounter++;
			token = strtok(NULL, ",");
		}

		/** Fill remaining numbers with 0 if not enough */
		if(numDigits > 0){
			for(d = 0; d < numDigits; d++){
				addDC(&dcList, dcCounter, "0");
				dcCounter++;
			}	
		}

		/** Error if too many numbers */
		if(numDigits < 0){
			printf("Extraneous text after end of command");
			printf(", the error is in line %d\n", lineCounter);
			lineCounter++;
			return -1;
		}
	}

	/** If no valid DC type was found */
	if(!foundDCNameType) {
		printf("Not valid DC command");
		printf(", the error is in line %d\n", lineCounter);
		lineCounter++;
		return -1;
	}

	return startCountDc;  /* Return starting dcCounter */
}
