#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char *variableList[1000]; int vIndex = 0;
int integerList[1000] = { 0 }; int iIndex = 0;

bool isKeyword(char* str) {

	if (!strcmp(str, "int") ||
		!strcmp(str, "move") || !strcmp(str, "to") || !strcmp(str, "add") ||
		!strcmp(str, "sub") || !strcmp(str, "from") ||
		!strcmp(str, "loop") || !strcmp(str, "times") ||
		!strcmp(str, "out") ||  !strcmp(str,"newline")) {
		return true;
	}

	return false;
}

bool isInteger(char* str) {           // Function that checks the correctness of the entered integer value
    FILE* fw=fopen("myscript.lx","w");
                                     
	int len = strlen(str); 

	if (len > 100) {
		fprintf(fw,"Error! Integer unacceptable.");
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (i > 0) {
			if (str[i] == '-' || len == 1) {
				fprintf(fw,"Error! Integer unacceptable.");
				return false;
			}
		}
		if (!(isdigit(str[i]) || str[i] == '-')) {
			return false;
		}
	}

	return true;
}

bool isIntegerVariable(char* str) {    

	for (int i = 0; i < vIndex; i++) {
		if (!strcmp(str, variableList[i])) {
			return true;
		}
	}

	return false;
}

bool isVariable(char* str)                                                                                           
{   
    FILE* fw=fopen("myscript.lx","w");
	int len = strlen(str);

	if (len > 20) {
		fprintf(fw,"Error! Unacceptable String \n ");     // Based on the project document string which can be accepted by language can't be longer than 20 letters.
		return false;
	}

	for (int i = 0; i < len; i++) {
		if (str[0] == '_') {
			fprintf(fw,"Error! Unacceptable String \n ");  // First letter of string can't be "_".
			return false;
		}
		                                                    // if else condition which is controls letter design.
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] == '_'))) {
			fprintf(fw,"Error! Undefined String Token  \n");
			return false;
		}
	}

	return true;
}

void parse(char* code) {
	FILE* fw=fopen("myscript.lx","w");                    //  We have designed our code to split the specified ".ba" into lines after the ".(Line End)" sign. 
	int lineCount = 0;
	char *line[1000];
	char *splitCode = strtok(code, ".");
	while (splitCode != NULL) {
		line[lineCount++] = splitCode;
		splitCode = strtok(NULL, ".");
	}

	for (int i = 0; i < lineCount; i++) {
		int wordCount = 0;
		char *word[1000];
		char *splitLine = strtok(line[i], " \n\r[]\"");
		while (splitLine != NULL) {
			word[wordCount++] = splitLine;
			splitLine = strtok(NULL, " \n\r");
		}

		if (wordCount == 0) { return; }

		//Int => Identifier 
		if (!strcmp(word[0], "int")) {              //If the first word of a line is "int", the if condition will be entered here.
			fprintf(fw,"Keyword int \n");

			if (isVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]); //Afterwards, we examined the patterns in bigAdd Language. As a result of the determined pattern, the line starting with int should continue with an identifier.

				variableList[vIndex++] = word[1];
				integerList[iIndex++] = 0;
			}
			else {;
				return 0;
			}

			if (wordCount == 2) {
				fprintf(fw,"EndofLine\n");
			}
		}


		//Move => Identifier  => to =>Identifier
		//Move => InsConstant => to =>Identifier 
		    
		else if (!strcmp(word[0], "move")) {        //If first letter of the line is "move". That code continious here.
			fprintf(fw,"Keyword move \n");

			int moveInt = 0;
			if (isInteger(word[1])) {
				fprintf(fw,"IntConstant %s \n", word[1]);  // IntConstant or Identifier may come after the "move" keyword, and branches are provided accordingly

				moveInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						moveInt = integerList[i];
						break;
					}
				}
			}
			else {
				return 0;
			}

			if (!strcmp(word[2], "to")) {      //The 3rd command of the "Move" function must be the "to" keyword. Accordingly, the controls were provided.
				fprintf(fw,"Keyword to \n");
			}
			else {;
				return 0;
			}

			if (isIntegerVariable(word[3])) {
				fprintf(fw,"Identifier %s \n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] = moveInt;
						break;
					}
				}
			}
			else {
				return 0;
			}

			if (wordCount == 4) {
				fprintf(fw,"EndofLine\n");  //End of line with "End of Line" when the specified number of words is reached.
			}
			else {
				return 0;
			}
		}



		//Add => IntConstant => to => InsConstant
	    //Add => IntConstant => to => Identifier
	    //Add => Identifier  => to => InsConstant 
	    //Add => Identifier  => to => Identifier
	    
		else if (!strcmp(word[0], "add")) {         // The branches of the "Add" command are given above. Alternatives were developed according to all branching possibilities.
			fprintf(fw,"Keyword add \n");

			int addInt = 0;
			
			if (isInteger(word[1])) {
				fprintf(fw,"IntConstant %s.\n", word[1]);

				addInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						addInt = integerList[i];
						break;
					}
				}
			}
		
			if (!strcmp(word[2], "to")) {
				fprintf(fw,"Keyword to \n");
				
			}
				
			if (isIntegerVariable(word[3])) {
				fprintf(fw,"Identifier %s \n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] += addInt;
						break;
					}
				}
			}
			if (isInteger(word[3])){
				fprintf(fw,"IntConstant %s \n", word[3]);
				
			}

			if (wordCount == 4) {
				fprintf(fw,"EndofLine\n");
			}
		}


		//Sub => IntConstant => from => InsConstant
	    //Sub => IntConstant => from => Identifier
	    //Sub => Identifier  => from => InsConstant 
	    //Sub => Identifier  => from => Identifier
	    
		else if (!strcmp(word[0], "sub")) {               //Similar to the Add method, alternatives have been developed for branches.
			fprintf(fw,"Keyword sub \n");

			int subInt = 0;
			if (isInteger(word[1])) {
				fprintf(fw,"IntConstant %s \n", word[1]);

				subInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						subInt = integerList[i];
						break;
					}
				}
			}
			else {
				return 0;
			}

			if (!strcmp(word[2], "from")) {
				fprintf(fw,"Keyword from \n");
			}
			else {
				return 0;
			}

			if (isIntegerVariable(word[3])) {
				fprintf(fw,"Identifier %s \n", word[3]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[3])) {
						integerList[i] -= subInt;
						break;
					}
				}
			}
			else if (isInteger(word[3])){
				fprintf(fw,"IntConstant %s \n", word[3]);
			}
			else {
				return 0;
			}

			if (wordCount == 4) {
				fprintf(fw,"EndofLine\n");
			}

		}

		
		else if (word[0][0] == ("%d",']')){
			fprintf(fw,"CloseBlock \n");
		}
        //Out => IntConstant
        //Out => StringConstant 
		//Out => Identifier => Seperator => Keyword newline
		//Out => Keyword newline => Seperator => StringConstant => Seperator => Identifier
		
		else if (!strcmp(word[0], "out")) {                 //Out bigAdd language's "print" function can therefore have many possible uses.
			fprintf(fw,"Keyword out \n");

			//Integer, Variable Or String
			if (isInteger(word[1])) {
				fprintf(fw,"IntConstant %s \n", word[1]);    //We have developed their branching according to different situations.
			}
			else if (isIntegerVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]);
				
				if(!strcmp(word[2],",")){
					fprintf(fw,"Seperator \n");
					
					if(!strcmp(word[3],"newline")){
						fprintf(fw,"Keyword newline \n");
						
						if(wordCount==4){
							fprintf(fw,"EndofLine \n");
						}
					}
				}

			}
			else if (!strcmp(word[1],"newline")){
				fprintf(fw,"Keyword newline \n");
				
				if(!strcmp(word[2],",")){
					fprintf(fw,"Seperator \n");
					fprintf(fw,"StringConstant %s %s",word[3],"\n");
					
					if(!strcmp(word[4],",")){
						fprintf(fw,"Seperator \n");
						
						if(isVariable(word[5])){
							fprintf(fw,"Identifier %s \n",word[5]);
							
							if(wordCount == 6 ){
								fprintf(fw,"EndofLine \n");
							}
						}
					}
				}
			}
			else {
				fprintf(fw,"StringConstant %s\n", word[1]);
			}

		}
        
		//Loop =>IntConstant => times => CodeBlock
		//Loop =>Identifier  => times => CodeBlock
		
		else if (!strcmp(word[0], "loop")) { //If the first word of a split line is "loop", the if condition captures this condition.
			fprintf(fw,"Keyword loop \n");

			int loopInt = 0;
			if (isInteger(word[1])) {
				fprintf(fw,"IntConstant %s \n", word[1]);

				loopInt = atoi(word[1]);
			}
			else if (isIntegerVariable(word[1])) {
				fprintf(fw,"Identifier %s \n", word[1]);

				for (int i = 0; i < vIndex; i++) {
					if (!strcmp(variableList[i], word[1])) {
						loopInt = integerList[i];
						break;
					}
				}
			}
			else {
				return 0;
			}

			//Times
			if (!strcmp(word[2], "times")) {
				fprintf(fw,"Keyword times \n");
				
				if(word[3][0]==("%d",'[')){
					fprintf(fw,"OpenBlock \n");
					
					if(!strcmp(word[4],"out")){
						fprintf(fw,"Keyword out \n");
						
						if(isInteger(word[5])){
							fprintf(fw,"IntConstant %s \n",word[5]);
							seperatorControl(word[5]);
						}
						else if (isIntegerVariable(word[5])){
							fprintf(fw,"Identifier %s \n", word[5] );
							
							if(!strcmp(word[6],",")){
								fprintf(fw,"Seperator \n");
								
								if(!strcmp(word[7],"newline")){
									fprintf(fw,"Keyword newline\n");
									
									if (wordCount == 8){
										fprintf(fw,"EndofLine\n");
										
									}
								}
							}
						}
					}
					else if(!strcmp(word[4],"add")){
						fprintf(fw,"Keyword add \n");
						int addInt = 0 ; 
						if(isInteger(word[5])){
							fprintf(fw,"IntConstant %s \n",word[5]);
							addInt = atoi(word[5]);
						}
						else if (isIntegerVariable(word[5])){
							fprintf(fw,"Identifier %s \n" , word[5]);
							for(int i = 0 ; i < vIndex ; i++){
								if(!strcmp(variableList[i],word[5])){
									addInt = integerList[i];
									break;
								}
							}
						}
						
						if (!strcmp(word[6],"to")){
							fprintf(fw,"Keyword to \n");
						}
						
						if(isIntegerVariable(word[7])){
							fprintf(fw,"Identifier %s \n",word[7]);
							for(int i = 0 ; i<vIndex ; i++){
								if(!strcmp(variableList[i],word[3])){
									integerList[i] += addInt;
									break;
								}
							}
						}
						if(isInteger(word[7])){
							fprintf(fw,"IntConstant %s \n",word[7]);
						}
						if(wordCount == 8){
							fprintf(fw,"EndofLine \n");
						}
						
					}
					else if (!strcmp(word[4],"sub")){
						fprintf(fw,"Keyword sub  \n");
						int subInt = 0 ;
						if(isInteger(word[4])){
							fprintf(fw,"IntConstant %s",word[4]);
							subInt = atoi(word[1]);
						}
						else if (isIntegerVariable(word[4])){
							fprintf(fw,"Identifier %s \n",word[4]);
							for (int i = 0 ; i<vIndex ; i++){
								if(!strcmp(variableList[i],word[4])){
									subInt = integerList[i];
									break;
								}
							}
						}
						else {
							return 0 ;
						}
						if(!strcmp(word[5],"from")){
							fprintf(fw,"Keyword from \n");
						}
						else{
							return 0 ;
						}
						
						if(isIntegerVariable(word[6])){
							fprintf(fw,"Identifier %s \n",word[6]);
							for(int i = 0 ; i<vIndex ; i++){
								if(!strcmp(variableList[i],word[6])){
									integerList[i] -= subInt;
									break;
								}
							}
						}
						else if (isInteger(word[6])){
							fprintf(fw,"IntConstant %s \n",word[6]);
						}
						
						if(wordCount == 7){
							fprintf(fw,"EndofLine \n");
						}

					}

					
					

				}
				else if(!strcmp(word[3],"out")){
					fprintf(fw,"Keyword out \n");
					if(isInteger(word[4])){
						fprintf(fw,"IntConstant %s \n",word[4]);
					}
					else if(isIntegerVariable(word[4])){
						fprintf(fw,"Identifier %s \n",word[4]);
					}
					else {
						fprintf(fw,"StringConstant %s \n",word[4]);
					}
					if(wordCount==4){
						fprintf(fw,"EndofLine \n");
					}
				}
					
				

			}
			else {
				return 0;
		 }

			//CodeBlock
			for (int i = 0; i < loopInt; i++) {  //Since a new CodeBlock can be created in the loop, a recursive function to analyze the resulting CodeBlock
				char newCode[100] = "";
				for (int i = 3; i < wordCount; i++) {
					strcat(newCode, " ");
					strcat(newCode, word[i]);
				}
				strcat(newCode, ".");
				parse(newCode);
			}

		}
	}
	fclose(fw);
}

char commentLine[100]; char a[100]; char b[100];  // The part that detects the comment line in bigAdd Language and allows that part of the code to be "ignore
void isComment(char* str) {
	FILE* fw=fopen("myscript.lx","w");
	bool trueComment = false;
	int j;
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '{') {
			for (j = i; j < strlen(str); j++) {
				if (str[j] == '}') {
					substring(str, i + 1, j - i - 1, commentLine);
					substring(str, 0, i, a);
					substring(str, j + 1, strlen(str), b);
					strcat(a, b);
					strcpy(str, a);

					trueComment = true;

					isComment(str);
				}
			}
			if (!trueComment) {
				exit(0);
			}
		}
	}
	fclose(fw);
}
int substring(char *source, int from, int n, char *target) {
	int length, i;
	for (length = 0; source[length] != '\0'; length++);
	if (from > length) {
		return 1;
	}
	if ((from + n) > length) {
		n = (length - from);
	}
	for (i = 0; i < n; i++) {
		target[i] = source[from + i];
	}
	target[i] = '\0';
	return 0;
}

int seperatorControl(char word[]){     // A function that detects if a string has a "," at the end, cancels the last comma and ensures that the word is understood correctly
	int uzunluk ;
	if(word[(strlen(word)-1)]==','){
		word[(strlen(word)-1)]='\0';
	}
	return word;	
}


int main() {
	FILE *fp;
	long lSize;
	char *code;
	
	char command[40];
	char fileName[40];
	char data[40];
	int i = 0 ; 
	char* words[100];
	
	printf("Enter Your Command : ");             // Lexical analyze process in the project document is started with the "la myscript" command, according to this request.
	gets(data);
	char *token = strtok(data," ");
	
	while(token != NULL){                        // The two words entered are divided into two parts according to the space between them and assigned to the indices of a defined array.
		
		words[i++]=token;
		token=strtok(NULL," ");
	}
		    
    strcpy(command,words[0]);                   //Then the words in these indices are assigned to the command and fileName variable.       
    strcpy(fileName,strcat(words[1],".ba"));    //Adding .ba extension to file name
    
    
    
    if(!strcmp(command,"la")){                  // If the word in the first index is the correct "la" command, it starts processing with the file name.
    	fp = fopen(fileName,"rb");
    	if (!fp) perror(fileName), exit(1);
	}
	else{
		printf("Incorrect start command \n");
	}
	
	fseek(fp, 0L, SEEK_END);
	lSize = ftell(fp);
	rewind(fp);

	code = calloc(1, lSize + 1);
	if (!code) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

	if (1 != fread(code, lSize, 1, fp)) {
		fclose(fp), free(code), fputs("entire read fails", stderr), exit(1);
	}
	isComment(code);
	parse(code);
	
	printf("  \n");
	printf("Analyze Completed \n");
}
