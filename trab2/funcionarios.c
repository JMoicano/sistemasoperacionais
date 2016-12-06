//Código das threads dos homens e mulheres
// cada homem e mulher terá um identificador de 1 a 10
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "monitorBanheiro.h"

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
	}
}


int main(){
	int num_funcionario = 10;

	pthread_t mulheres[num_funcionario];
    pthread_t homens[num_funcionario];

    int id[num_funcionario];

    for (int i = 0; i < num_funcionario; ++i)
    {
    	id[i] = i;
    }

    for (int i = 0, int j = 0; i < num_funcionario; ++i, ++j)
    {
    	pthread_create(&homens[i], NULL, (void*)thread_homem, &id[i]);
    	pthread_create(&mulheres[j], NULL, (void*)thread_mulher, &id[j]);
    }

    for (int i = 0; i < num_funcionario; ++i)
    {
    	pthread_join(mulheres[i], NULL);
    	pthread_join(homens[i], NULL);
    }

    pthread_exit(NULL);
    
    return 0;
}





