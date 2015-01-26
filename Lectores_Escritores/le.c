#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "error.h"

#define TRUE 1
#define MAX 30
#define NUM_LECTORES 5	
#define escrituras 10													//limite de escrituras

//estructura

struct libro {

	int pagina;
	char * hoja[MAX];

};

//prototipo de funciones

void * lectores(void * arg);
void * escritores(void * arg);

//creamos semaforos

pthread_mutex_t semaforo = PTHREAD_MUTEX_INITIALIZER;					//semaforo para seccion critica
pthread_mutex_t semaforo_lectores = PTHREAD_MUTEX_INITIALIZER;			//semaforo para lectores

//variables globales
int lecturas=50;														//Total de lecturas			
int cuenta_lectores=0;													//cuenta el numero de lectores
struct libro l1;													

int main (){

int i=0;
int error;

pthread_t * hilos=(pthread_t *)malloc(sizeof(pthread_t)*NUM_LECTORES+1);		//Reserva para lectores y un escritor

//Creamos los hilos

for(i=0;i<NUM_LECTORES;i++){

	if(i==0){

		error=pthread_create(&hilos[i],NULL, (void *) escritores, NULL);
		validaCreate(error);

	}

	else{

		error=pthread_create(&hilos[i],NULL, (void *) lectores, NULL);
		validaCreate(error);

	}

}

//Esperamos los hilos

for(i=0;i<NUM_LECTORES;i++){

	error=pthread_join(hilos[i],NULL);
	validaJoin(error);

}

	error=pthread_mutex_destroy(&semaforo);
	validaMutexDestroy(error);
	
	error=pthread_mutex_destroy(&semaforo_lectores); 
	validaMutexDestroy(error);

	printf("\n\n\t*** Fin del programa.\n\n");

	exit(EXIT_SUCCESS);
}

void * lectores (void * arg){

int error;
int i=0;

	while (lecturas > 1){

		error=pthread_mutex_lock(&semaforo_lectores); 
		validaLock(error);

		cuenta_lectores++;

		//Se bloquea el acceso al escritor una vez hay un lector

		if(cuenta_lectores == 1){

			error=pthread_mutex_lock(&semaforo);
			validaLock(error);

		}

		//Se desbloquea el semaforo de escritores por si quiere entrar otro más

		error=pthread_mutex_unlock(&semaforo_lectores);
		validaUnlock(error);

		printf("leyendo pagina %d\n",l1.pagina);

		error=pthread_mutex_lock(&semaforo_lectores);
		validaLock(error);

		cuenta_lectores--;

		//Cuando no quedan lectores desbloqueamos el semaforo de los escritores

		if(cuenta_lectores == 0){
		
			error=pthread_mutex_unlock(&semaforo);		
			validaUnlock(error);

		}

		//Desbloqueamos el semaforo de lectores para que puedan seguir entrando o finalizen

		error=pthread_mutex_unlock(&semaforo_lectores);
		validaUnlock(error);

		lecturas--;

	}

	pthread_exit(NULL);

}


void * escritores (void * arg){

int i=0;
int error;

	for(i=0; i<escrituras; i++){

		//bloquea semaforo del escritor

		error=pthread_mutex_lock(&semaforo);
		validaLock(error);

		//Escribe

		printf("Escribiendo...\n");
		l1.pagina++;

		//Desbloqueamos semaforo del escritor

		error=pthread_mutex_unlock(&semaforo);
		validaUnlock(error);

	}
	
	pthread_exit(NULL);	

}