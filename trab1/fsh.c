#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include "list.h"

char* waitInput(char* str);

void handleSigChld(int sig);

void getChild(int sig);

int makeargv(const char *s, const char *delimiters, char ***argvp);

int exec(char** argv);


void pwd();

void cd(char* dir);

void waitz();

void exitFSH();


char pidList[MAX_CANON];
pid_t children[2];
List p1List;
sigset_t intmask;

int main(int argc, char const *argv[]){

	char line[MAX_CANON];
	char** commands;
	while(1){
		waitInput(line);


		int numCommands = makeargv(line, "@\n", &commands);
		char** temp;
		makeargv(commands[0], " ",&temp);
		pid_t fshPID = getpid();
		sprintf(pidList, "%d", fshPID);

//*****************************************************************
		if(numCommands == 0){
			
			continue;

		}else if(strcmp(temp[0],"cd") == 0 ){
			
			cd(temp[1]);

		}else if(strcmp(temp[0],"pwd") == 0 ){
			
			pwd();

		}else if(strcmp(temp[0], "waitz") == 0){
			
			waitz();

		}else if(strcmp(temp[0], "exit") == 0){

			exitFSH();

		}else{
//*****************************************************************
			pid_t pid = fork();

			if(pid < 0){
				perror("Erro na criação do processo!");
				exit(1);

			}else if(pid == 0){
				setsid();
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
			}else{
				sigemptyset(&intmask);
				sigaddset(&intmask, SIGINT);
				sigprocmask(SIG_BLOCK, &intmask, NULL);
				signal(SIGCHLD, getChild);
			}
		}
		sleep(1);
			
	}
	return 0;
}

void exitFSH(){
	while (!isEmpty()){
		pid_t pid = getData(deleteFirst());
		kill(pid, SIGCHLD);
	}
	exit(0);
}

void waitz(){
	pid_t pid = waitpid(-1, NULL, WNOHANG);
	while (pid > 0){
		printf("Processo com PID: %d liberado.\n", pid);
	}
	printf("Todos os processos liberados.\n");
}

void getChild(int sig){
	pid_t pid = waitpid(-1, NULL, WNOHANG);
	delete(pid);
	if (isEmpty()){
		sigprocmask(SIG_UNBLOCK, &intmask, NULL);
	}
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

pid_t exec(char** argv){
	int tratador=0;
	pid_t pid = fork();
	if(pid == 0){
		tratador = execvp(argv[0], argv);
		if( tratador == -1){ 
			printf("Não foi possivel iniciar processo, comando inválido.\n");
			abort();
		}
	}
	return pid;
}

//*****************************************************************FUNCOES PARA TRATAR CD E PWD

void pwd(){
	char diretorio[100];

	if(getcwd(diretorio, MAX_CANON) != NULL)
		printf("%s\n", diretorio);
	else
		perror("Não foi possível obter o diretorio atual:");
}

void cd(char* dir){
	int ret;

	if (dir[strlen (dir) - 1] == '\n')
        dir[strlen (dir) - 1] = '\0';

	ret = chdir(dir);

	if (ret != 0){
		perror("Nao foi possivel alterar o diretorio:");
	}
}
//*****************************************************************

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
