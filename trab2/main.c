#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <sys/wait.h>
#include "funcionarios.h"
#include "monitorBanheiro.h"
int TAM = 10;


//void criaIdHomem(int *id);

//void criaIdMulher(int *id);

int main(int argc, char* argv[]){


	pthread_t homem;
	pthread_t mulher;

	int x=1;
	int y=2;


	pthread_create(&homem, NULL, (void*) thread_homem,(void*)&x);
	pthread_create(&mulher, NULL, (void*) thread_mulher,(void*)&y);


	pthread_join(mulher,NULL);
	pthread_join(homem,NULL);


return 0;
}




