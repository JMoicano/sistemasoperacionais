#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
// #include <sys/type.h>

char* waitInput(char* str);

int processCommands(const char* s, char*** commands); //TODO: atribui a commands a lista de comandos lida em s e retorna a quantidade de comandos

void handleSigChld(int sig);

int makeargv(const char *s, const char *delimiters, char ***argvp);

int exec(char** argv);

pid_t children[2];

char pidList[MAX_CANON];
int main(int argc, char const *argv[]){
	char line[MAX_CANON];
	char** commands;
	while(1){
		waitInput(line);
		int numCommands = processCommands(line, &commands);
		pid_t fshPID = getpid();
		sprintf(pidList, "%d", fshPID);
		if(/*CHAMADA DO SISTEMA*/){
			//TRATA CHAMADAS DE SISTEMA
			continue;
		}
		pid_t pid = fork();
		if(pid == 0){
			for (int i = 0; i < numCommands; ++i){
				char** argv;
				int numTokens = makeargv(commands[i], " \t\n", &argv);
				children[1] = exec(argv);
				sprintf(pidList, "%s -> %d", pidList, children[1]);
				printf("%s\n", pidList);
				int flag = (i == numCommands-1);

				if(!flag){
					children[0] = fork();
				}

				signal(SIGCHLD, handleSigChld);

				if(flag || children[0] > 0){
					wait(NULL);
					break;
				}

			}
			break;	
		}
		sleep(1);
		
	}
	return 0;
}

char* waitInput(char* str){
	printf("fsh> ");
	return fgets(str,MAX_CANON,stdin);
}

void handleSigChld(int sig){
	kill(children[0], SIGCHLD);
	kill(children[1], SIGKILL);
	exit(0);
}

int processCommands(const char* s, char*** commands){
	return makeargv(s, "@", commands);
}

pid_t exec(char** argv){
	pid_t pid = fork();
	if(pid == 0){
		execvp(argv[0], argv);
	}
	return pid;
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