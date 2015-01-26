#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "error.h"

#define MAX_BUFFER 5									//Tamaño del buffer
#define TOTAL_PRODUCCIONES 50							//Numero de producirducciones a crear (Cambiar el deseado)

//Semaforos

sem_t producir;											//Para despertar al productor.
sem_t consumir;											//Para despertar al consumidor.
sem_t sc;												//Para acceder a la sección critica.

//producirtotipos de funciones

void * productor (void * arg);
void * consumidor (void * arg);

//Variables globales

int buffer[MAX_BUFFER];									//Buffer, sección crítica
int datos[TOTAL_PRODUCCIONES];							//Datos que serán pasados al buffer para que sean consumidos por el consumidor.

//Programa principal

int main () {

int i=0;
int error=0;

//inicializamos los hilos

error=sem_init(&sc,0,1);								//Inicialmente se pueden producir T_BUFFER elementos
validaSemInit(error);
error=sem_init(&consumir,0,0);							//Inicialmente no se puede consumir nada.
validaSemInit(error);
error=sem_init(&producir,0,MAX_BUFFER);					//Sólo 1 acceso a la vez a la sección crítica.
validaSemInit(error);


pthread_t * hilos = (pthread_t *) malloc (sizeof(pthread_t)*2);

	//Inicializamos el array con los datos: 1 .. N_DATOS

	for(i=0;i<TOTAL_PRODUCCIONES;i++){

		datos[i]=i+1;

	}

	//Creamos los hilos

	for(i=0;i<2;i++){

		if(i==0){

			//Productor
			error = pthread_create( &(hilos[i]), NULL, (void *) productor, NULL);
			validaCreate(error);

		}
		else{

			//Consumidor
			error = pthread_create( &(hilos[i]), NULL, (void *) consumidor, NULL);
			validaCreate(error);

		}	
		
	}

	//Esperamos a que finalicen los hilos.

	for (i=0;i<2;i++){

		error=pthread_join(hilos[i], NULL);
		validaJoin(error);

	}

	//Destruimos los semaforos

	error=sem_destroy(&sc);
	validaSemDestroy(error);
	error=sem_destroy(&producir);
	validaSemDestroy(error);	
	error=sem_destroy(&consumir);
	validaSemDestroy(error);

	exit(EXIT_SUCCESS);
}

//funciones

void * productor(void * arg){

int error;
int elemento;
int unidades_producidas=0;

	while(unidades_producidas!=TOTAL_PRODUCCIONES){

		error=sem_wait(&producir);
		validaSemWait(error);

		error=sem_wait(&sc);
		validaSemWait(error);

		elemento=datos[unidades_producidas];
		buffer[unidades_producidas%MAX_BUFFER] = elemento;
		unidades_producidas++;
		printf("\nSe produce el dato: %d\n",elemento);
		error=sem_post(&consumir);	
		validaSemPost(error);
		
		error=sem_post(&sc);
		validaSemPost(error);

	}

	printf("\n*** Todas las unidades han sido pruducidas. ***\n\n");
	pthread_exit(NULL);

}

void * consumidor (void * arg){

int error;
int elemento;
int unidades_consumidas=0;

	while(unidades_consumidas!=TOTAL_PRODUCCIONES){

		//Si no hemos consumirsumido todas las unidades...
		error=sem_wait(&consumir);			
		validaSemWait(error);

		error=sem_wait(&sc);	
		validaSemWait(error);

		elemento=buffer[unidades_consumidas% MAX_BUFFER];
		buffer[unidades_consumidas % MAX_BUFFER]=-1;
		unidades_consumidas++;
		printf("\nSe consume el dato : %d\n",elemento);	
		error=sem_post(&producir);		
		validaSemPost(error);

		error=sem_post(&sc);
		validaSemPost(error);

	}

		printf("\n*** Todas las unidades han sido consumidas. ***\n\n");
		pthread_exit(NULL);

}