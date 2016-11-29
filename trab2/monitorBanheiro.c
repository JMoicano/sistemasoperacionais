#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <pthread.h>

int fila[20]={2,1,1,2,1,2,1,2,1,2,1,2,1,1,1,1,2,2,1,2};

 // erro  int pthread_cond_init(pthread_cond_t * cond, const pthread_cond_attr *attr);
	/*Initialization of cond. Usually attr is initialized to 
        the default, represented as NULL.*/

   int pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex);
	/*When this command is executed the executing thread goes to sleep
	on cond and simultaneously mutex is unlocked, thus allowing another
	thread to execute past a lock on mutex. */

   int pthread_cond_timedwait(pthread_cond_t * cond, pthread_mutex_t * mutex, const struct timespec *abstime);
       /* Same as the wait command, but now we have an absolute time
        timeout.*/

   int pthread_cond_signal(pthread_cond_t * cond);
	/*This command is null when no thread is asleep on cond. Otherwise
	a thread is released from cond. When a thread is released
	from waiting on a condition variable, its mutex is implicitly locked
        again [of course if at that time the mutex is already locked, the 
        thread will have to wait].*/

   int pthread_cond_broadcast(pthread_cond_t * cond);
      /*  This is like signal, but now all threads waiting on the condition
        are released and compete for the mutex lock.*/

   int pthread_cond_destroy(pthread_cond_t * cond);

#define  FALSE 0
#define  TRUE  1


// Declaracao das variaveis de condicao:
pthread_mutex_t  mutex;

// Condições
//pthread_cond_t cond_queue_empty, cond_queue_full;

//Buffer
#define  BUFFERVAZIO 0
#define  BUFFERCHEIO 1
int buffer;
int estado = BUFFERVAZIO;


/* funcoes do monitor */
void homemQuerEntrar()  //Monitor deve imprimir na tela o número de pessoas de cada sexo no banheiro
{
	int i = 0;
	int contador =0;

	for(i=0;i<20;i++){
		if(fila[i] == 2) //bloqueia: mulher na fila de espera ( preferencia );
		contador++;
	}
	if(contador >= 3) //bloqueia: banheiro cheio

	for(i=0;i<3;i++){
		if(fila[i] == 0){
			//desbloqueia
			fila[i] == 1;
		}
	}	
}
void homemSai()
{
	int i = 0;

	for(i=0;i<10;i++){
		if(fila[i] == 1){
			fila[i] = 0;
			//desbloqueia
		}
	}

}
void mulherQuerEntrar() //Monitor deve imprimir na tela o número de pessoas de cada sexo no banheiro
{
	int i = 0;
	int contador =0;
	for(i=0;i<3;i++){
		if(fila[i] == 1)//bloqueia: homem no banheiro

		contador++;
	}
	if(contador >= 3)//bloqueia: banheiro cheio

	for(i=0;i<3;i++){
		if(fila[i] == 0){
			//desbloqueia
			fila[i] == 2;
		}
	}	

}
void mulherSai()
{
	int i = 0;

	for(i=0;i<10;i++){
		if(fila[i] == 2){
			fila[i] = 0;
			// desbloqueia
		}
	}

}

void imprimeFila(){
int i=0,h=0,m=0;

	for(i=0;i<10;i++){
		if(fila[i] == 1) h++;

		if(fila[i] == 2) m++;
	}

	printf("Existem %d homens e %d mulheres no banheiro\n",h,m);

}



