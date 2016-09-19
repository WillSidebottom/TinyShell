#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void readFile(const char *);
void readInput();
int getSize(const char *, const char[]);
char **parse(const char *, const char[], const int);							//function to parse input. Accepts a command string, a delimiter, and an array size for reference
void print(char **, const int);
void clean(char **, const int);
int execute(char **, const int);

const int bufferSize = 512;

int main(int argc, char * argv[]) {

	if(argc > 1) {															
		char * fileName = argv[1];
		readFile(fileName);
	}
	else {
		readInput();
	}

	return 0;
}

void readFile(const char *fileName) {
	char command[bufferSize];
	char ** tokenArray;
	int tokenArraySize;
	const char semiColon[3] = ";\n";

	FILE *fp = fopen(fileName, "r");

	if(fp == NULL) {
		perror("file could not be opened");
		exit(1);
	}
	else {
		while(1) {
			if(fgets(command, bufferSize, fp) != NULL) {
				tokenArraySize = getSize(command, semiColon);

				tokenArray = parse(command, semiColon, tokenArraySize);

				print(tokenArray, tokenArraySize);
				
				if (execute(tokenArray, tokenArraySize) == 1)
					exit(1);
				
				clean(tokenArray, tokenArraySize);
			}
			else
				fclose(fp);
		}

	}
}

void readInput() {
	int tokenArraySize = 0;
	char command[bufferSize];
	const char semiColon[3] = ";\n";
	char **tokenArray;	

	while(!feof(stdin)) {													//While loop to accept commands
		printf("prompt> ");													//print first prompt statement

		fgets(command, bufferSize, stdin); 									//get input string from stdin

		tokenArraySize = getSize(command, semiColon);
		
		tokenArray = parse(command, semiColon, tokenArraySize);	

		print(tokenArray, tokenArraySize);

		if (execute(tokenArray, tokenArraySize) == 1)
			exit(1);

		clean(tokenArray, tokenArraySize);
	}
}

int getSize(const char *command, const char delim[]) {
	int size = 0;
	char * token;
	char *copy = (char*) calloc(strlen(command) + 1, sizeof(char));
	
	strcpy(copy, command);

	if((token = strtok(copy, delim)) == NULL) {
		fprintf(stderr, "syntax error near unexpected token `;'\n"); exit(1);
	}

	while(token != NULL) {														//while loop to determine array size for dynamic allocation
		token = strtok(NULL, delim);
		(size)++;
	}

	free(copy);

	return size;
}

char **parse(const char *command, const char delim[], const int size) {
	char *token, *copy;															//individual token from command input
	char ** tokenArray;															//Array of strings storing each token
	int tokenLength = 0, index = 0;												//tokenLength will hold individual lengths of tokens, arraySize holds size of tokenArray

	copy = (char*) calloc(strlen(command) + 1, sizeof(char));					//dynamically allocate size to a copy of the command

	strcpy(copy, command);														//makes a copy of command into copy

	tokenArray = (char**) calloc(size, sizeof(char*));							//dynamically allocate array of tokens/strings based on 'size'

	if((token = strtok(copy, delim)) == NULL) {
		fprintf(stderr, "syntax error near unexpected token `;'\n"); exit(1);
	}																			//tokenize copy to copy tokens into array

	while(token != NULL) {														//while the string can be broken into tokens....
		tokenLength = strlen(token) + 1;										//get the string length of current token
		
		tokenArray[index] = (char*) calloc(tokenLength, sizeof(char));			//dynamically allocate space for current index for that token
		
		strcpy(tokenArray[index], token);										//copy token into array
		
		token = strtok(NULL, delim);											//iterate to the next token
		
		index++;																//increment index
	}

	free(copy);																	//clean up copy of command

	return tokenArray;
}

void print(char **array, const int size) {
	int i;

	for(i = 0; i < size; i++) 
		printf("prompt> %s\n", array[i]);
}

void clean(char **array, const int size) {
	int i;

	for(i = 0; i < size; i++)
		free(array[i]);

	free(array);
}

int execute(char** tokenArray, const int tokenArraySize) {
	int i, argArraySize = 0, cstatus, qstatus = 0;
	const char space[3] = " \n";
	char ** argArray;
	pid_t child, c;

	for(i = 0; i < tokenArraySize; i++) {

		argArray = parse(tokenArray[i], space, argArraySize);					//get an array of arguments from current string in tokenArray

		if(strstr(argArray[0], "quit") != NULL)
			qstatus = 1;		
		
		if((child = fork()) == 0 && qstatus != 1) {												//create new process and check if it worked			
			execvp(argArray[0], argArray);

			fprintf(stderr, "Fork failed\n"); exit(1);
		}

		clean(argArray, argArraySize);
	}
	for(i = 0; i < tokenArraySize; i++) {
		if(child == (pid_t)(-1)) {
			fprintf(stderr, "Fork failed\n"); exit(1);
		}
		else {
			c = wait(&cstatus);
			printf("Parent: Child  %ld exited with status = %d\n",
				(long) c, cstatus);
		}
	}

	return qstatus;
}