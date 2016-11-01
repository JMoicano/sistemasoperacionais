#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
/*#include <sys/type.h> */

int makeargv(const char *s, const char *delimiters, char ***argvp);

int main(int argc, char *argv[]){

	if ( argc != 2 ){
		printf("\nusage\n");
		return 0;
	}
	//---------------- variaveis para o makeargv
	char delim[] = "\t";
	char **myargv;
	int numTokens=0;
	//----------------
	int pr_count = 0;
	int pr_limit = atoi(argv[1]);
	char comand[MAX_CANON];

	while(!feof(stdin)){ //OU FAZER UM FOR E DAR EXIT(0) QND CONDICA0 = END-OF-FILE


		//printf("%s\n",comand);

		//printf("%s\n",myargv[0]); 

		
		if(pr_count >= pr_limit){
			wait();
			pr_count--;
		}
		
		fgets(comand,MAX_CANON,stdin);
		numTokens = makeargv(comand, delim, &myargv);

		if (numTokens  == -1){
			perror("Child failed to construct argument array");
			exit(1);
		}	

		if (fork() == 0){
			execvp(*myargv,myargv);//TRATAR EXEC

		}

		pr_count++;
		
		//waitpid TODO: Pesquisar essa funcao			
	/*
		printf("limite = %d, atual = %d\n",pr_limit,pr_count); //TRATAR PR_COUNT E PR_LIMIT
		pr_count++;
	*/
		
	}


	return 0;
}

/*
5. Execute o loop a seguir até que o “end­of­file” seja lido da entrada padrão.
a. Se pr_count for igual a pr_limit, espere o término de um filho e decremente
pr_count.
b. Leia uma linha da entrada padrão (fgets) de no máximo MAX_CANON
c.
d.
caracteres e execute um programa correspondendo à linha de comando
lida criando-se um processo filho (veja função makeargv() no final do
trabalho).
Incremente pr_count para contabilizar o número de processos filhos ativos.
Verifique se algum filho já terminou, sem bloquear caso nenhum filho
tenha terminado. Decremente pr_count para cada filho finalizado.
6. Após a leitura do “end­of­file” da entrada padrão, aguarde que todos os demais
processos filhos terminem e, finalmente, termine o programa.
*/

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
