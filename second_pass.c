

#include "second_pass.h"

#define SKIP_BRATKETS 4
#define MAX_DIGITIN_BASE_FOUR 32





commandPrint printAction[] = {
	{"mov",  movPrint},  {"cmp",  cmpPrint}, {"add",  addPrint}, {"sub",  subPrint},
	{"lea",  leaPrint},  {"clr",  clrPrint}, {"not",  notPrint}, {"inc",  incPrint},
	{"dec",  decPrint},  {"jmp",  jmpPrint}, {"bne",  bnePrint}, {"jsr",  jsrPrint},
	{"red",  redPrint},  {"prn",  prnPrint}, {"rts",  rtsPrint}, {"stop", stopPrint}
};

/*
 * extractLabelName:
 *	Extracts the label from a string (up to '[' or whitespace) and stores it in dest.
 */
void extractLabelName(char *src, char *dest) {
	int i = 0;
	while (src[i] != '\0' && src[i] != '[' && !isspace(src[i])) {
		dest[i] = src[i];	/* Copy character to destination */
		i++;
	}
	dest[i] = '\0';	/* Null-terminate the extracted label */
}

/*
 * binaryToBase4:
 *	Convert a binary string to a base-4 representation stored in wordBaseFour.
 */
void binaryToBase4(const char binary[BINARY_LENGTH], char wordBaseFour[BASE_FOUR_LENGTH]) {
	int idx;

	for (idx = 0; idx < BASE_FOUR_LENGTH - 1; idx++) {
		int bit1 = binary[idx * 2] - '0';
		int bit2 = binary[idx * 2 + 1] - '0';
		wordBaseFour[idx] = (bit1 * 2 + bit2) + '0';	/* Convert two binary bits to base-4 digit */
	}
	wordBaseFour[BASE_FOUR_LENGTH - 1] = '\0';	/* Null-terminate */
}

/*
 * base4ToLetters:
 *	Convert base-4 digits to letters a-d and print to file.
 */
void base4ToLetters(FILE *file, char arr[BASE_FOUR_LENGTH]) {
	int i;
	for (i = 0; i < BASE_FOUR_LENGTH - 1; i++) {
		switch (arr[i]) {
			case '0': arr[i] = 'a'; break;
			case '1': arr[i] = 'b'; break;
			case '2': arr[i] = 'c'; break;
			case '3': arr[i] = 'd'; break;
		}
	}
	arr[BASE_FOUR_LENGTH - 1] = '\0';
	fprintf(file, "%s\n", arr);	/* Print converted letters to file */
}

/*
 * printDecimalAsLetters:
 *	Convert a decimal number to base-4 letters and print to file.
 */
void printDecimalAsLetters(FILE *file, int num) {
	int digits[MAX_DIGITIN_BASE_FOUR];
	int i = 0;
	int n = num;
	char map[] = {'a', 'b', 'c', 'd'};

	if (num == 0) {
		fprintf(file, "a\n");
		return;
	}

	while (n > 0) {
		digits[i++] = n % 4;	/* Store remainder */
		n /= 4;
	}

	for (i = i - 1; i >= 0; i--) {
		fprintf(file, "%c", map[digits[i]]);	/* Print mapped letter */
	}
}

/*
 * printDCList1:
 *	Print the contents of a DC list for debugging.
 */
void printDCList1(structDCList *list) {
	int i;
	if (list == NULL || list->structs == NULL) {
		printf("DC list is empty or not initialized.\n");
		return;
	}

	printf("DC List (count: %d, capacity: %d):\n", list->count, list->capacity);
	for (i = 0; i < list->count; i++) {
		structDC item = list->structs[i];
		printf("  [%d] location: %d, contentSize: %lu, info: \"%s\"\n",
			   i, item.location, (unsigned long)item.contentSize,
			   item.info ? item.info : "NULL");
	}
}

/*
 * isExternal:
 *	Check if a label is of type "external".
 */
bool isExternal(ptr head, char *labelName) {
	while (head != NULL) {
		if (strcmp(head->Lable, labelName) == 0) {
			return strcmp(head->type, "external") == 0;	/* Return true if type is external */
		}
		head = head->Next;
	}
	return false;
}

/*
 * isEntry:
 *	Check if a label is an entry (of data or code).
 */
bool isEntry(ptr head, char *labelName) {
	while (head != NULL) {
		if (strcmp(head->Lable, labelName) == 0) {
			return (strcmp(head->type, "entryOfData") == 0 || strcmp(head->type, "entryOfCode") == 0);
		}
		head = head->Next;
	}
	return false;
}

/*
 * printEntryLabels:
 *	Print all entry labels and their addresses in base-4 letters to a file.
 */
void printEntryLabels(ptr head, FILE *entryFp) {
	ptr current = head;

	while (current != NULL) {
		if (current->type != NULL && 
			(strcmp(current->type, "entryOfData") == 0 || strcmp(current->type, "entryOfCode") == 0)) {
			fprintf(entryFp, "%s\t", current->Lable);
			printDecimalAsLetters(entryFp, current->Addres);	/* Print label address in letters */
			fprintf(entryFp, "\n");
		}
		current = current->Next;
	}
}

/*
 * isAction:
 *	Check if a given string is a recognized assembler action.
 */
bool isAction(char *currentName) {
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
		strcmp(currentName, "stop") == 0)
	{
		return true;
	}
	return false;
}

/** 
 * This routine encodes each instruction into a special base-4 representation.
 * It processes the input file line by line, converting every instruction and operand
 * into binary, then into this custom base-4, and finally writes it to the object file.
 */

bool secondPass(char *amFile, char *objectFile,char *externFile,char *entryFile)
{
	int w, e, len, t;
	char tempStr[MAX_SIZE_FOR_LINE];
	int pcCounter=PC_START;
	char line[MAX_SIZE_FOR_LINE] = {0};
	char word1[MAX_SIZE_FOR_LINE] = {0};
	char action[MAX_SIZE_FOR_LINE] = {0};
	char word2[MAX_SIZE_FOR_LINE] = {0};
	char operand[MAX_SIZE_FOR_LINE] = {0};
	char wordBaseFour[MAX_SIZE_FOR_WORD] = {0};
	char labelName1[MAX_SIZE_FOR_LINE]= {0};
	char labelName2[MAX_SIZE_FOR_LINE]= {0};
	int i = 0, j = 0, k = 0, m = 0, num = 0, adder = 0;
	int temp;
	bool number1 = false, mat1 = false, label1 = false, reg1 = false,labelIsExtern1=false,labelIsExtern2=false;
	bool number2 = false, mat2 = false, label2 = false, reg2 = false;
	
	FILE *amFp = fopen(amFile, "r");
	FILE *objectFp = fopen(objectFile, "w");
	FILE *externFp = fopen(externFile, "w");
	FILE *entryFp = fopen(entryFile, "w");

	if (amFp == NULL) {
		fprintf(stderr, "Error: Could not open input file '%s'\n", amFile);
		if (amFp) fclose(amFp);
		if (objectFp) fclose(objectFp);
		if (externFp) fclose(externFp);
		if (entryFp) fclose(entryFp);

	    	return false;
	}

	if (objectFp == NULL) {
	    	fprintf(stderr, "Error: Could not open object file '%s'\n", objectFile);
		if (amFp) fclose(amFp);
		if (objectFp) fclose(objectFp);
		if (externFp) fclose(externFp);
		if (entryFp) fclose(entryFp);
		return false;
	}

	if (externFp == NULL) {
	    	fprintf(stderr, "Error: Could not open extern file '%s'\n", externFile);
		if (amFp) fclose(amFp);
		if (objectFp) fclose(objectFp);
		if (externFp) fclose(externFp);
		if (entryFp) fclose(entryFp);
	    	return false;
	}

	if (entryFp == NULL) {
	    	fprintf(stderr, "Error: Could not open entry file '%s'\n", entryFile);
		if (amFp) fclose(amFp);
		if (objectFp) fclose(objectFp);
		if (externFp) fclose(externFp);
		if (entryFp) fclose(entryFp);
	   	return false;
	}


	
	fprintf(objectFp, "  ");
	printDecimalAsLetters(objectFp,icCounter);
	fprintf(objectFp,"   ");
	printDecimalAsLetters(objectFp,dcCounter);
	fprintf(objectFp,"\n");

	while ((temp = fgetc(amFp)) != EOF) {
		i = 0;

		while (temp != '\n' && temp != EOF && i < MAX_SIZE_FOR_LINE - 1) {
			line[i++] = temp;
			temp = fgetc(amFp);
		}
		line[i] = '\0';
		i = 0;

		if (line[0] == ';') continue;
		if (isOnlySpaces(line)) continue;


		SKIP_SPACES
		if (line[i] == '.') {
			i++;
			if (line[i] != '\0' && line[i] == 'e') 
				continue;
		}

		SKIP_SPACES
		while ((line[i] != '\0') && (!isspace(line[i]))) {
			action[j] = line[i];
			j++;
			i++;
		}
		action[j] = '\0';

		SKIP_SPACES
		j = 0;
		if (!isAction(action)) { 
			memset(action, 0, sizeof(action));
			SKIP_SPACES
			j = 0;
			while ((line[i] != '\0') && (!isspace(line[i]))) {
				action[j] = line[i];
				j++;
				i++;
			}
			action[j] = '\0';
		}
		SKIP_SPACES

		while (line[i] != '\0' && line[i] != ',') {
			word1[k] = line[i];
			i++;
			k++;
		}
		word1[k] = '\0';
		removeBlanks(word1);

		k = 0;

		if (word1[k] == '#') 
			number1 = true;
		if(strcmp(word1,"r0") == 0 || strcmp(word1,"r1") == 0 ||
			   strcmp(word1,"r2") == 0 || strcmp(word1,"r3") == 0 ||
			   strcmp(word1,"r4") == 0 || strcmp(word1,"r5") == 0 ||
			   strcmp(word1,"r6") == 0 || strcmp(word1,"r7") == 0)
		{
		   reg1 = true;
		}

		if (isalpha(word1[0]) && !reg1) 
		{

			char word3[MAX_SIZE_FOR_LINE] = {0};
			char cleanLabel[MAX_LABLE_SIZE] = {0};
			int a = 0, k = 0;
	  		while (word1[k] != '\0' && word1[k] != '[') {
				word3[a++] = word1[k++];
		    	}
		    	word3[a] = '\0';

		    	extractLabelName(word3, cleanLabel);
			if(isEntry(head,cleanLabel))
			{
				printf("Label %s is entry type\n",cleanLabel);
				if (amFp)
				{
					fclose(amFp);
	    				remove(amFile);
				}
				if (objectFp)
				{
				 	fclose(objectFp);
    					remove(objectFile);
				}
				if (externFp)
				{
				 	fclose(externFp);
    					remove(externFile);
				}
				if (entryFp)
				{
				 	fclose(entryFp);
    					remove(entryFile);
				}
				return false;
			}
			

		    	if(!checkIfLabelExists(head, cleanLabel)) {

				printf("Label %s not found in the list of labels\n",cleanLabel);
				if (amFp) fclose(amFp);
				if (objectFp) fclose(objectFp);
				if (externFp) fclose(externFp);
				if (entryFp) fclose(entryFp);
				return false;
		    	} 
			else 
			{
				if (word1[k] == '['){
					mat1 = true;
				}
				else{
					if(isExternal(head,cleanLabel)){
						labelIsExtern1 = true;
						strcpy(labelName1, cleanLabel);
					}
					label1 = true;
				}
		    	}
		}

		k = 0;
		while (line[i] != '\0' && line[i] != ',') 
			i++;

		if (line[i] != '\0' && line[i] == ',') {
			i++;
			while (line[i] != '\0') {
				word2[k] = line[i];
				i++;
				k++;
			}
			word2[k] = '\0';
			removeBlanks(word2);

			k = 0;
			while (isspace(word2[k])) 
				k++;
		}

		if (word2[k] == '#') 
			number2 = true;

		if(strcmp(word2,"r0") == 0 || strcmp(word2,"r1") == 0 ||
			   strcmp(word2,"r2") == 0 || strcmp(word2,"r3") == 0 ||
			   strcmp(word2,"r4") == 0 || strcmp(word2,"r5") == 0 ||
			   strcmp(word2,"r6") == 0 || strcmp(word2,"r7") == 0)
		{
			reg2 = true;
		}

		if (isalpha(word2[k]) && !reg2)
		{
			int tempK = k;
			char word3[MAX_SIZE_FOR_LINE] = {0};
			char cleanLabel[MAX_LABLE_SIZE] = {0};
			int a = 0;

			while (word2[tempK] != '\0' && word2[tempK] != '[') {
				word3[a] = word2[tempK];
				a++;
				tempK++;
		        }
			word3[a] = '\0';
			extractLabelName(word3, cleanLabel);
			    
			if(isEntry(head,cleanLabel))
			{
				printf("Label %s is entry type\n",cleanLabel);
				if (amFp)
				{
				 	fclose(amFp);
    					remove(amFile);
				}
				if (objectFp)
				{
				 	fclose(objectFp);
    					remove(objectFile);
				}
				if (externFp)
				{
				 	fclose(externFp);
    					remove(externFile);
				}
				if (entryFp)
				{
				 	fclose(entryFp);
    					remove(entryFile);
				}
				return false;
			 }
				
			 if (!checkIfLabelExists(head, cleanLabel)) {


				printf("Label not found in the list of labels\n");
				if (amFp)
				{
				 	fclose(amFp);
    					remove(amFile);
				}
				if (objectFp)
				{
				 	fclose(objectFp);
    					remove(objectFile);
				}
				if (externFp)
				{
				 	fclose(externFp);
    					remove(externFile);
				}
				if (entryFp)
				{
				 	fclose(entryFp);
    					remove(entryFile);
				}
				return false;
			    } else {
				if (word2[tempK] == '[')
				    	mat2 = true;
				else{
					if(isExternal(head,cleanLabel)){
						strcpy(labelName2, cleanLabel);
						labelIsExtern2 = true;
					}
					label2 = true;
				   }
			    }
			}
			k = 0;

			

			for (k = 0; k < NUM_ACTION; k++) {
				if (strcmp(action, printAction[k].name) == 0) {
					printDecimalAsLetters(objectFp, pcCounter);
					fprintf(objectFp,"\t");
					printAction[k].func(number1, label1, mat1, reg1,number2, label2, mat2, reg2);
					addToWordBinary(0,2);
					binaryToBase4(wordBinary,wordBaseFour);
					base4ToLetters(objectFp, wordBaseFour);
					memset(wordBaseFour, 0, sizeof(wordBaseFour));
					memset(wordBinary, 0, sizeof(wordBinary));
					wordIndex = 0;
					fprintf(objectFp,"\n");
					pcCounter++;
				
				}
			}
		
			if(labelIsExtern1){
				fprintf(externFp, "%s", labelName1);
				fprintf(externFp,"\t");
				printDecimalAsLetters(externFp, pcCounter);
				fprintf(externFp,"\n");
				memset(labelName1, 0, sizeof(labelName1));
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				fprintf(objectFp,"aaaab");
				wordIndex = 0;
				fprintf(objectFp,"\n");
			
			
			}
			if (mat1) {
				char cleanLabel[MAX_LABLE_SIZE];
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				j = 0; k = 0;
				while (word1[k] != '[') {
					operand[j] = word1[k];
					j++;
					k++;
				}
				extractLabelName(operand, cleanLabel);
				adder = findLabelAddress(head, cleanLabel);
				if (adder != -1) {
					addToWordBinary(adder, 8);
						addToWordBinary(2, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
						fprintf(objectFp,"\n");

					printDecimalAsLetters(objectFp,pcCounter);
					fprintf(objectFp,"\t");
				pcCounter++;
				}
			
				k++;
				 k++;
				addToWordBinary(word1[k], 4);
				k += SKIP_BRATKETS;
				addToWordBinary(word1[k], 4);
				addToWordBinary(0, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}
		
			if (number1) {
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				num = atoi(word1 + 1);

				addToWordBinary(num, 8);
				addToWordBinary(0, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}
		
			if (label1&& !labelIsExtern1) {
				char cleanLabel[MAX_LABLE_SIZE];
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp, "\t");
				pcCounter++;
				extractLabelName(word1, cleanLabel);
				adder = findLabelAddress(head, cleanLabel);
				addToWordBinary(adder, 8);
				addToWordBinary(2, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}

			if (reg1 && reg2) {
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				num = atoi(word1 + 1);
				addToWordBinary(num, 4);
				num = atoi(word2 + 1);
				addToWordBinary(num, 4);
				addToWordBinary(0, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");;

			}

			if (reg1 && !reg2) {
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				num = atoi(word1 + 1);
				addToWordBinary(num, 4);
				addToWordBinary(0, 6);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}
			if(labelIsExtern2){
				fprintf(externFp, "%s", labelName2);
				fprintf(externFp,"\t");
				printDecimalAsLetters(externFp, pcCounter);
				fprintf(externFp,"\n");
				memset(labelName2, 0, sizeof(labelName2));
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				fprintf(objectFp,"aaaab");

				wordIndex = 0;
				fprintf(objectFp,"\n");
			}



			if (mat2) {

				char cleanLabel[MAX_LABLE_SIZE];
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				m = 0; k = 0;
				while (word2[k] != '[') {
					operand[m] = word2[k];
					m++;
					k++;
				}
				extractLabelName(operand, cleanLabel);
				adder = findLabelAddress(head, cleanLabel);
				if (adder != -1) {
					addToWordBinary(adder, 8);
					addToWordBinary(2, 2);
					binaryToBase4(wordBinary,wordBaseFour);
					base4ToLetters(objectFp, wordBaseFour);
					memset(wordBaseFour, 0, sizeof(wordBaseFour));
					memset(wordBinary, 0, sizeof(wordBinary));
					wordIndex = 0;
				fprintf(objectFp,"\n");

					printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				}
			
				m = 0;
				memset(operand, 0, sizeof(operand));
				k++; k++;
				addToWordBinary(word2[k], 4);
				k++; k++; k++; k++;
				addToWordBinary(word2[k], 4);
				addToWordBinary(0, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp,wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}

			if (number2) {

				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				num = atoi(word2 + 1);
				addToWordBinary(num, 8);
				addToWordBinary(0, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");

			}

			if (label2 && !labelIsExtern2) {
				char cleanLabel[MAX_LABLE_SIZE];
			
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
				pcCounter++;
				extractLabelName(word2, cleanLabel);
				adder = findLabelAddress(head, cleanLabel);
				addToWordBinary(adder, 8);
				addToWordBinary(2, 2);
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;
				fprintf(objectFp,"\n");
	
			}

			if (reg2 && !reg1) {
				printDecimalAsLetters(objectFp, pcCounter);
				fprintf(objectFp,"\t");
			
				pcCounter++;
				addToWordBinary(0, 4);
					
				num = atoi(word2 + 1);
				addToWordBinary(num, 4);
				addToWordBinary(0, 2);
				/*printf("%s",wordBinary);*/
				binaryToBase4(wordBinary,wordBaseFour);
				base4ToLetters(objectFp, wordBaseFour);
				memset(wordBaseFour, 0, sizeof(wordBaseFour));
				memset(wordBinary, 0, sizeof(wordBinary));
				wordIndex = 0;

				fprintf(objectFp,"\n");
			

			}

			memset(line, 0, sizeof(line));
			memset(word1, 0, sizeof(word1));
			memset(word2, 0, sizeof(word2));
			memset(operand, 0, sizeof(operand));
			j = 0; k = 0; m = 0; num = 0;
			number1 = false;
			mat1 = false;
			label1 = false;
			reg1 = false;

			number2 = false;
			mat2 = false;
			label2 = false;
			reg2 = false;
			labelIsExtern2=false;
			labelIsExtern1=false;



	}

	num = 0;


	for (w = 0; w < dcList.count; w++) 
	{
		if (dcList.structs[w].info[0] == '\0') {
			printDecimalAsLetters(objectFp, pcCounter);
			fprintf(objectFp,"\t");
			fprintf(objectFp,"aaaaa");
			fprintf(objectFp,"\n");
			pcCounter++;	
			continue;
		}

		len = (int)strlen(dcList.structs[w].info);
		if (len == 0)
			continue;

		printDecimalAsLetters(objectFp, pcCounter);
		fprintf(objectFp,"\t");
		
		e = 0;
		while (e < len) {
			while (e < len && (dcList.structs[w].info[e] == ' ' || dcList.structs[w].info[e] == '\t'))
				e++;

			t = 0;
			memset(tempStr, 0, sizeof(tempStr));
			while (e < len && dcList.structs[w].info[e] != ',' &&dcList.structs[w].info[e] != ' ' && dcList.structs[w].info[e] != '\t') 
			{
				if (isprint((unsigned char)dcList.structs[w].info[e])) 
				{
					tempStr[t++] = dcList.structs[w].info[e];
				}
				e++;
			}
			tempStr[t] = '\0';

			if (t > 0) 
			{
				if (isdigit((unsigned char)tempStr[0]) || tempStr[0] == '-') {
					num = atoi(tempStr);
					addToWordBinary(num, 10);
				} else {
					for (t = 0; tempStr[t] != '\0'; t++) {
						addToWordBinary((unsigned char)tempStr[t], 10);
					}
				}
			}

			while (e < len && (dcList.structs[w].info[e] == ',' || dcList.structs[w].info[e] == ' ' ||dcList.structs[w].info[e] == '\t'))
				e++;
		}

		binaryToBase4(wordBinary, wordBaseFour);
		base4ToLetters(objectFp, wordBaseFour);
		memset(wordBaseFour, 0, sizeof(wordBaseFour));
		memset(wordBinary, 0, sizeof(wordBinary));
		wordIndex = 0;
		fprintf(objectFp,"\n");
		pcCounter++;

		if (strchr(dcList.structs[w].info, '"') != NULL) {
			printDecimalAsLetters(objectFp, pcCounter);
			fprintf(objectFp,"\t");
			addToWordBinary(0, 10);
			binaryToBase4(wordBinary, wordBaseFour);
			base4ToLetters(objectFp,wordBaseFour);
			memset(wordBaseFour, 0, sizeof(wordBaseFour));
			memset(wordBinary, 0, sizeof(wordBinary));
			wordIndex = 0;
			fprintf(objectFp,"\n");
			pcCounter++;
		}
	}
	
	printEntryLabels(head,entryFp);
	if (amFp) fclose(amFp);
	if (objectFp) fclose(objectFp);
	if (externFp) fclose(externFp);
	if (entryFp) fclose(entryFp);

	return true;
}

