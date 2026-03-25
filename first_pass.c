#include "first_pass.h"
#define NUM_START_ADDRESS 100
#define MAX_WORDS_WITHOUT_LABEL 2

/* Array of commands and their corresponding functions */
command commands[] = {
	{"mov", twoOperandAction},
	{"cmp", cmp},
	{"add", twoOperandAction},
	{"sub", twoOperandAction},
	{"lea", lea},
	{"clr", oneOperandAction},
	{"not", oneOperandAction},
	{"inc", oneOperandAction},
	{"dec", oneOperandAction},
	{"jmp", oneOperandAction},
	{"bne", oneOperandAction},
	{"jsr", oneOperandAction},
	{"red", oneOperandAction},
	{"prn", prn},
	{"rts", zeroOperandAction},
	{"stop", zeroOperandAction}
};

/* Checks if any entry label has zero address and prints error */
bool noEntryLabelsWithZeroAddress(node *head)
{
	bool found = false;

	while (head != NULL) {
		if (strcmp(head->type, "entry") == 0 && head->Addres == 0) {
			printf("%s   entry label wasnt declared\n", head->Lable);
			found = true;
		}
		head = head->Next;
	}

	return !found;
}

/* Returns type if the label is already defined, otherwise NULL */
char *isUniqueLabel(node *head, char *labelName)
{
	if (labelName == NULL)
		return NULL;
	while (head != NULL) {
		if (strcmp(head->Lable, labelName) == 0) {
			return head->type;
		}
		head = head->Next;
	}
	return NULL;
}

/* Updates the address of a label */
int updateLabelAddress(node *head, char *labelName, bool isDcCommand, int dcStart)
{
	while (head != NULL) {
		if (strcmp(head->Lable, labelName) == 0) {
			if (isDcCommand) {
				head->Addres = dcStart;
				head->type = strdup("entryOfData");
			} else {
				head->Addres = icCounter;
				head->type = strdup("entryOfCode");
			}
			return 1;
		}
		head = head->Next;
	}
	return 0;
}



/* Updates addresses of labels according to start values */
void updateAddresses(ptr head) {
	ptr current = head;

	while (current != NULL) {
		if (strcmp(current->type, "code") == 0 || strcmp(current->type, "entryOfCode") == 0) {
			current->Addres += NUM_START_ADDRESS;
		}
		else if (strcmp(current->type, "data") == 0 || strcmp(current->type, "entryOfData") == 0) {
			current->Addres += NUM_START_ADDRESS + icCounter;
		}
		current = current->Next;
	}
}

/* Adds a new node to the end of the linked list */
void addToEnd(ptr *head, char *label, int address, char *type)
{
	ptr newNode = (ptr)malloc(sizeof(node));
	if (newNode == NULL) {
		perror("Error allocating memory for new node\n");
		exit(1);
	}

	newNode->Lable = malloc(strlen(label) + 1);
	if (newNode->Lable == NULL) {
		perror("Error allocating memory for label\n");
		free(newNode);
		exit(1);
	}
	strcpy(newNode->Lable, label);

	newNode->type = malloc(strlen(type) + 1);
	if (newNode->type == NULL) {
		perror("Error allocating memory for type\n");
		free(newNode->Lable);
		free(newNode);
		exit(1);
	}
	strcpy(newNode->type, type);

	newNode->Addres = address;
	newNode->Next = NULL;

	if (*head == NULL) {
		*head = newNode;
	} else {
		ptr current = *head;
		while (current->Next != NULL) {
			current = current->Next;
		}
		current->Next = newNode;
	}
}

/* Creates a new node */
ptr createNode(char *label, int addr, char *type)
{
	ptr newNode = (ptr)malloc(sizeof(node));
	if (!newNode) {
		perror("Allocation failed for new node\n");
		exit(1);
	}

	newNode->Lable = strdup(label);
	if (newNode->Lable == NULL) {
		free(newNode);
		perror("Allocation failed for label\n");
		exit(1);
	}

	newNode->type = strdup(type);
	if (newNode->type == NULL) {
		free(newNode->Lable);
		free(newNode);
		perror("Allocation failed for type\n");
		exit(1);
	}

	newNode->Addres = addr;
	newNode->Next = NULL;
	return newNode;
}

/* Checks if string contains only spaces */
bool isOnlySpaces(char s[])
{
	int i = 0;
	bool onlySpaces = true;
	while (s[i] != '\0') {
		if (!isspace(s[i]))
			onlySpaces = false;
		i++;
	}
	return onlySpaces;
}

/* Checks if string contains only lowercase letters */
bool isOnlyLowerLetters(char s[])
{
	int i = 0;
	while (s[i] != '\0') {
		if (!islower((unsigned char)s[i]))
			return false;
		i++;
	}
	return true;
}

bool mustHavelabel(char *line) /* if there are more than 2 words before comma - it starts with label */
{
	int i = 0, j, k = 0;
	int wordCount = 0;
	bool inWord = false;
	char word[MAX_SIZE_FOR_LINE] = {0};

	/* Skip leading spaces */
	while (line[i] != '\0' && isspace(line[i]))
	{
		i++;
	}
	j = i;

	/* Count words before the comma */
	while (line[i] != '\0' && line[i] != ',') 
	{
		if (!isspace(line[i])) 
		{
			if (!inWord) 
			{
				inWord = true;
				wordCount++;
				if (wordCount > MAX_WORDS_WITHOUT_LABEL) 
				{
					return true;
				}
			}
		} 
		else 
		{
			inWord = false;
		}
		i++;
	}

	inWord = false;
	wordCount = 0;

	/* Extract the second word if needed */
	while (line[j] != '\0') 
	{
		if (!isspace(line[j]))
		{
			if (!inWord) {
				inWord = true;
				wordCount++;
			}
			if (inWord){
				if(wordCount == MAX_WORDS_WITHOUT_LABEL){/*in  second word*/
					word[k]=line[j];
					k++;
				}
			}
		} 
		else 
		{
			inWord = false;
		}
		j++;
	}
	word[k] = '\0';

	/* Special case for "stop" or "rts" */
	if (wordCount == MAX_WORDS_WITHOUT_LABEL && (strcmp("stop", word) == 0 || strcmp("rts", word) == 0)) 
	{
		return true;
	}

	return false;
}




bool firstPass(char *filename)
{
	char line[MAX_SIZE_FOR_LINE] = {0}, content[MAX_SIZE_FOR_LINE] = {0}, word[MAX_SIZE_FOR_LINE] = {0};
	char *labelType, labelWord[MAX_SIZE_FOR_LINE] = {0};
	int i = 0, j = 0, k = 0, countDC = 0, temp = 0, wordEnding = 0;
	bool noErrorInFile = true, commandNameFound = false, lineTooLong = false, lineadvanced = false;

	/* Open input file */
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "error: Could not open input file '%s'\n", filename);
		return false;
	}

	/* Read file character by character */
	while ((temp = fgetc(file)) != EOF)
	{
		/* Read a single line */
		while (temp != '\n' && temp != EOF)
		{
			if (sizeof(line) > MAX_SIZE_FOR_LINE)
			{
				/* Line too long */
				printf("line is too over at 81 char(including ending string char\n");
				printf(", the error is in line %d", lineCounter);
				lineCounter++;
				lineTooLong = true;
				noErrorInFile = false;
				i = 0;
				break;
			}
			line[i++] = temp;
			temp = fgetc(file);
		}

		if (lineTooLong)
		{
			memset(line, 0, sizeof(line));
			lineCounter++;
			continue;
		}

		line[i] = '\0';
		i = 0;

		/* Skip comment lines starting with ; */
		if (line[0] == ';')
		{
			memset(line, 0, sizeof(line));
			continue;
		}

		/* Skip lines containing only spaces */
		if (isOnlySpaces(line))
		{
			memset(line, 0, sizeof(line));
			continue;
		}

		SKIP_SPACES

		/* Check for empty label */
		if (line[i] == ':')
		{
			printf("empty label, the error is in line %d\n", lineCounter);
			lineCounter++;
			i = 0;
			noErrorInFile = false;
			memset(line, 0, sizeof(line));
			continue;
		}

		/* Parse first word: label, .extern, or .entry */
		while ((line[i] != '\0') && (!isspace(line[i])))
		{
			word[j++] = line[i];
			i++;
		}
		word[j] = '\0';
		SKIP_SPACES
		wordEnding = j;
		j = 0;

		/* If first character is a letter, check for label */
		if (isalpha(word[0]))
		{
			if (!isCommand(word) && mustHavelabel(line))
			{
				if (!validLabel(word, true))
				{
					noErrorInFile = false;
					i = 0;
					memset(word, 0, sizeof(word));
					memset(line, 0, sizeof(line));
					continue;
				}

				if (codeLable(line))
				{
					/* Trim ':' from label */
					word[wordEnding - 1] = '\0';
					labelType = isUniqueLabel(head, word);
					if (labelType == NULL)
					{
						addToEnd(&head, word, icCounter, "code");
					}
					else
					{
						/* Handle conflicts with existing label */
						if (strcmp(labelType, "external") == 0)
						{
							printf("the label is external, cannot be defined here");
							printf(", the error is in line %d\n", lineCounter);
							lineCounter++;
							i = 0;
							j = 0;
							noErrorInFile = false;
							memset(word, 0, sizeof(word));
							memset(line, 0, sizeof(line));
							continue;
						}
						if (strcmp(labelType, "entry") != 0)
						{
							printf("two identical labels");
							printf(", the error is in line %d\n", lineCounter);
							lineCounter++;
							i = 0;
							j = 0;
							noErrorInFile = false;
							memset(word, 0, sizeof(word));
							memset(line, 0, sizeof(line));
							continue;
						}
						if (strcmp(labelType, "entry") == 0)
						{
							updateLabelAddress(head, word, false, 0);
						}
					}
				}
				strcpy(labelWord, word);
				memset(word, 0, sizeof(word));
				SKIP_SPACES

				/* Parse next word */
				j = 0;
				while ((line[i] != '\0') && (!isspace(line[i])))
				{
					word[j++] = line[i];
					i++;
				}
				word[j] = '\0';
				j = 0;
				SKIP_SPACES
			}

			
		}
		/* Copy the remaining line content */
		while (line[i] != '\0')
		{
			content[j++] = line[i];
			i++;
		}
		content[j] = '\0';

		/* Handle .extern directive */
		if (strcmp(word, ".extern") == 0)
		{
			if (validLabel(content, false))
			{
				labelType = isUniqueLabel(head, word);
				if (labelType == NULL)
					addToEnd(&head, content, 0, "external");
			}
			else
			{
				noErrorInFile = false;
			}

			memset(line, 0, sizeof(line));
			memset(word, 0, sizeof(word));
			memset(labelWord, 0, sizeof(labelWord));
			memset(content, 0, sizeof(content));
			i = 0;
			j = 0;
			lineCounter++;
			continue;
		}

		/* Handle .entry directive */
		if (strcmp(word, ".entry") == 0)
		{
			if (validLabel(content, false))
			{
				labelType = isUniqueLabel(head, word);
				if (labelType == NULL)
					addToEnd(&head, content, 0, "entry");
			}
			else
			{
				noErrorInFile = false;
			}

			memset(line, 0, sizeof(line));
			memset(word, 0, sizeof(word));
			memset(labelWord, 0, sizeof(labelWord));
			memset(content, 0, sizeof(content));
			i = 0;
			j = 0;
			lineCounter++;
			continue;
		}

		/* Handle data directives starting with '.' */
		if (word[0] == '.')
		{
			strcat(word, " ");
			strcat(word, content);
			countDC = checkDC(word);

			if (countDC != -1)
			{
				labelType = isUniqueLabel(head, labelWord);
				if (labelType == NULL)
				{
					addToEnd(&head, labelWord, countDC, "data");
					lineCounter++;
				}
				else
				{
					if (strcmp(labelType, "entry") != 0)
					{
						printf("two identical labels");
						printf(", the error is in line %d\n", lineCounter);
						lineCounter++;
						i = 0;
						j = 0;
						noErrorInFile = false;
						memset(word, 0, sizeof(word));
						memset(line, 0, sizeof(line));
						continue;
					}
					if (strcmp(labelType, "entry") == 0)
					{
						updateLabelAddress(head, labelWord, true, countDC);
					}
				}
			}
			else
			{
				noErrorInFile = false;
			}

			memset(line, 0, sizeof(line));
			memset(word, 0, sizeof(word));
			memset(labelWord, 0, sizeof(labelWord));
			memset(content, 0, sizeof(content));
			i = 0;
			j = 0;
			continue;
		}
		/* Search for commands */
		for (k = 0; k < NUM_ACTION; k++)
		{
			if (strcmp(word, commands[k].name) == 0)
			{
				commandNameFound = true; /* Command recognized */

				if (!commands[k].func(content))
				{
					noErrorInFile = false;
					lineadvanced = true;
				}

				icCounter++;
				break;
			}
		}

		/* Undefined command error */
		if (commandNameFound == false)
		{
			printf("Undefined command name");
			printf(", the error is in line %d\n", lineCounter);
			noErrorInFile = false;
		}

		if (!lineadvanced)
			lineCounter++;
		lineadvanced = false;
		commandNameFound = false;

		memset(line, 0, sizeof(line));
		memset(word, 0, sizeof(word));
		memset(labelWord, 0, sizeof(labelWord));
		memset(content, 0, sizeof(content));
		i = 0;
		j = 0;
	}

	/* Update all addresses after first pass */
	updateAddresses(head);

	/* Check if all entry labels got proper addresses */
	if (!noEntryLabelsWithZeroAddress(head))
		return false;

	fclose(file);
	return noErrorInFile;
}

























