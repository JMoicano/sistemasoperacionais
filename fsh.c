#include <limits.h>

char* waitInput(char* str);

int processCommands(const char* s, char*** commands); //TODO: atribui a commands a lista de comandos lida em s e retorna a quantidade de comandos

void handleSigChld(int a);

int numChild = 0;

int main(int argc, char const *argv[]){
	char line[MAX_CANON];
	char** commands;
	while(true){
		waitInput(line);
		int numCommands = processCommands(line, &commands);
		int pid = 0;
		for (int i = 0; i < numCommands; ++i){
			if (pid == 0){
				signal(SIGCHLD, handleSigChld);//TODO: ver como faz pra não instalar no fsh
				pid = fork();
				if(pid != 0){
					execve(commands[i][0], &commands[i][0], NULL);//TODO: verificar se eh esse exec nessa parte tambem e se vai ser assim
				}
			}
		}
	}
	return 0;
}

char* waitInput(char* str){
	printf("fsh> ");
	return fgets(command,MAX_CANON,stdin)
}

void handleSigChld(int a){
	exit(0);
}