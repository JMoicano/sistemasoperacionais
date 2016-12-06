#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define  FALSE 0
#define  TRUE  1

// Declaracao das variaveis de condicao:
pthread_mutex_t  mutex;
int banheiro_cheio = FALSE;
int mulher_no_banheiro = FALSE;
int homem_no_banheiro = FALSE;

int mulheres_na_fila = 0;
int contador = 0; //contador de quantas pessoas tem no banheiro

/* funcoes do monitor */
void homemQuerEntrar()
{
	int aguenta = FALSE; //Se tem que esperar na fila
	do{
		pthread_mutex_lock(&mutex); //entra na regiao critica
        aguenta = banheiro_cheio || mulher_no_banheiro || (!homem_no_banheiro && mulher_na_fila != 0); //Se tem alguma condição que o homem nao possa entrar, ele aguenta
        if(aguenta) {
            pthread_mutex_unlock(&mutex); //se ele ainda esta na fila, sai da regiao critica
        }
    } while(aguenta); //espera na fila
 
    homem_no_banheiro = TRUE; //conseguiu entrar
    contador++;
    if (contador == 3){
        banheiro_cheio = TRUE;
    }
    printf("Nesse momento há %d homens no banheiro\n", contador);
    pthread_mutex_unlock(&mutex); //sai da regiao critica
}

void homemSai()
{
	pthread_mutex_lock(&mutex); //entra na regiao critica
	contador--;

	if(contador == 2){ //verifica se, antes desse homem sair, o banheiro estava cheio
		banheiro_cheio = FALSE;
	}
	if(contador == 0){ //verifica se o banheiro esta vazio agora
		homem_no_banheiro = FALSE;
	}
	pthread_mutex_unlock(&mutex);

}
void mulherQuerEntrar()
{
	mulheres_na_fila++; //Uma mulher entrou na fila
	int aguenta = FALSE; //Se tem que esperar na fila
	do{
		pthread_mutex_lock(&mutex); //entra na regiao critica
        aguenta = banheiro_cheio || homem_no_banheiro; //Se tem alguma condição que a mulher nao possa entrar, ela aguenta
        if(aguenta) {
            pthread_mutex_unlock(&mutex); //se ela ainda esta na fila, sai da regiao critica
        }
    } while(aguenta); //espera na fila
 
 	mulheres_na_fila--; //mulher entrou no banheiro, ou seja, saiu da fila
    mulher_no_banheiro = TRUE; //conseguiu entrar
    contador++;
    if (contador == 3){
        banheiro_cheio = TRUE;
    }
    printf("Nesse momento há %d mulheres no banheiro\n", contador);
    pthread_mutex_unlock(&mutex); //sai da regiao critica
}
void mulherSai()
{
	thread_mutex_lock(&mutex); //entra na regiao critica
	contador--;

	if(contador == 2){ //verifica se, antes dessa mulher sair, o banheiro estava cheio
		banheiro_cheio = FALSE;
	}
	if(contador == 0){ //verifica se o banheiro esta vazio agora
		mulher_no_banheiro = FALSE;
	}
	pthread_mutex_unlock(&mutex);

}
