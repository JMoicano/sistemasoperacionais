//Código das threads dos homens e mulheres
// cada homem e mulher terá um identificador de 1 a 10
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/wait.h>
#include "monitorBanheiro.h"
int true = 1;
int false = 0;

void thread_homem(int *id)
{
	while (true){

		printf ("Eu sou o homem-%d:...Estou apertado! Vou no banheiro!\n",*id);
			homemQuerEntrar();
		printf ("Eu sou homem-%d: ... UFA! Entrei no banheiro!\n",*id);
			sleep(3);
			homemSai();
		printf ("Eu sou homem-%d: ... Estou aliviado! Vou trabalhar!\n",*id);
			sleep(5);
	//	break;//teste id
	}
}

void thread_mulher(int *id)
{
	while (true){

		printf ("Eu sou a mulher-%d:...Estou apertada! Vou no banheiro!\n",*id);
			mulherQuerEntrar();
		printf ("Eu sou mulher-%d: ... UFA! Entrei no banheiro!\n",*id);
			sleep(3);
			mulherSai();
		printf ("Eu sou mulher-%d: ... Estou aliviada! Vou trabalhar!\n",*id);
			sleep(5);
	//	break;//teste id
	}
}






