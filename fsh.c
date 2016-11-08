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

//*****************************************************************

void retiraQuebra(char* s);
int quebraLinhaDeComando(char* linha_de_comando, char** comandos, char* divisor);
void pwd();
void cd(char* dir);

//*****************************************************************
//***************************************************************** FUNCOES PARA BLOQUEAR O SINAL CNTR-C, MAS ESTA APEANS IGNORANDO E QND ELE E DESBLOQUEADO ELE FINALIZA A SHELL
/*
sigset_t intmask;
sigset_t newsigset;
void trataSigint(){
	if ((sigemptyset(&newsigset) == -1) || (sigaddset(&newsigset, SIGINT) == -1))
		perror("Failed to initialize the signal set");
	else if (sigprocmask(SIG_BLOCK, &newsigset,NULL) == -1)
		perror("Failed to block SIGINT");
}

void destrataSigint(){
	if ((sigemptyset(&newsigset) == -1) || (sigaddset(&newsigset, SIGINT) == -1))
		perror("Failed to initialize the signal set");
	else if (sigprocmask(SIG_UNBLOCK, &newsigset,NULL) == -1)
		perror("Failed to block SIGINT");
}



void bloqueiaSigint(){
	sigemptyset(&intmask);
	sigaddset(&intmask, SIGINT);
	sigprocmask(SIG_BLOCK, &intmask, NULL);
}

void desbloqueiaSigint(){
	sigprocmask(SIG_UNBLOCK, &intmask, NULL);
}
//*****************************************************************
*/
char pidList[MAX_CANON];
int main(int argc, char const *argv[]){
int k;
//*****************************************************************
/*
trataSigint();
sleep(5);
destrataSigint();
*/
/*	bloqueiaSigint();
	sleep(5);
	desbloqueiaSigint();
*/	
//*****************************************************************
	char line[MAX_CANON];
	char** commands;
	char** aux; //CRIAR AUXILIAR PARA TRATAR O " " DA FUNCAO CD
	while(1){
		waitInput(line);


		int numCommands = processCommands(line, &commands);
		int test = processCommands(line, &aux); //CRIAR AUXILIAR PARA TRATAR O " " DA FUNCAO CD
		pid_t fshPID = getpid();
		sprintf(pidList, "%d", fshPID);
	//	if(/*CHAMADA DO SISTEMA*/){
	//		//TRATA CHAMADAS DE SISTEMA
	//		continue;
	//	}

//*****************************************************************
		k = quebraLinhaDeComando(line, aux, " "); //TRATA O COMANDO CD PARA ACESSAR O DIRETORIO
		retiraQuebra(aux[k-1]);

		if(strcmp(line,"cd") == 0 ){
			cd(aux[1]);

		}

		else if(strcmp(line,"pwd") == 0 ){
			pwd();

		}

		else {
//*****************************************************************
			pid_t pid = fork();

			if(pid < 0){
				perror("Erro na criação do processo!");
				exit(1);

			}else if(pid == 0){
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
		}
		//sleep(1);
		
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

//*****************************************************************FUNCOES PARA TRATAR CD E PWD , E DUAS FUNCOES PARA SEPARAR O CD E ACESSAR O DIRETORIO

void retiraQuebra(char* s){
	char *p2 = s;
    while(*s != '\0') {
    	if(*s != '\t' && *s != '\n') {
    		*p2++ = *s++;
    	} else {
    		++s;
    	}
    }
    *p2 = '\0';
}
int quebraLinhaDeComando(char* linha_de_comando, char** comandos, char* divisor){

	char* partes;
	int i = 0, j;

	partes = strtok(linha_de_comando, divisor);

	while(partes != NULL && i<10){
    	comandos[i] = (char*)malloc((strlen(partes)+1)*sizeof(char));
    	strcpy(comandos[i], partes);
    	i++;
    	partes = (char*)strtok(NULL, divisor);
  	}

  	return i;
}
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
