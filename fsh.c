#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>

char* waitInput(char* str);

int processCommands(const char* s, char*** commands); //TODO: atribui a commands a lista de comandos lida em s e retorna a quantidade de comandos

void handleSigChld(int a);

int makeargv(const char *s, const char *delimiters, char ***argvp);

int numChild = 0;

int main(int argc, char const *argv[]){
	char line[MAX_CANON];
	char** commands;
	while(1){
		waitInput(line);
		int numCommands = processCommands(line, &commands);
		int pid = fork();
		if(pid == 0){
			signal(SIGCHLD, handleSigChld);
			for (int i = 0; i < numCommands; ++i){
				pid = fork();
				if(pid != 0){
					signal(SIGCHLD, handleSigChld);
					char** argv;
					int numTokens = makeargv(commands[i], " ", &argv);
					execve(argv[0], &argv[0], NULL);
				}
			}
			break;	
		}
		
	}
	return 0;
}

char* waitInput(char* str){
	printf("fsh> ");
	return fgets(str,MAX_CANON,stdin);
}

void handleSigChld(int a){
	exit(0);
}

int processCommands(const char* s, char*** commands){
	return makeargv(s, "@", commands);
}

int makeargv(const char *s, const char *delimiters, char ***argvp) {
	int error;
	int i;
	int numtokens;
	const char *snew;
	char *t;
	if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
		errno = EINVAL;
		return -1;
	}

	*argvp = NULL;
	snew = s + strspn(s, delimiters);
	/* snew is real start of string */
	if ((t = malloc(strlen(snew) + 1)) == NULL)
		return -1;
		strcpy(t, snew);
		numtokens = 0;
	if (strtok(t, delimiters) != NULL)
	/* count the number of tokens in s */
	for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ;
	/* create argument array for ptrs to the tokens */
	if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
		error = errno;
		free(t);
		errno = error;
		return -1;
		}
	/* insert pointers to tokens into the argument array */
	if (numtokens == 0)
		free(t);
		else {
			strcpy(t, snew);
			**argvp = strtok(t, delimiters);
			for (i = 1; i < numtokens; i++)
				*((*argvp) + i) = strtok(NULL, delimiters);
		}
	*((*argvp) + numtokens) = NULL;
	/* put in final NULL pointer */
return numtokens;
}