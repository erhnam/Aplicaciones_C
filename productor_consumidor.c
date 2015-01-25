#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "error.h"

#define FALSE 0
#define TRUE 1
#define MAX_BUFFER 5									//Tamaño del buffer
#define total_producciones 25							//Numero de producciones a crear (Cambiar el deseado)

//Prototipo de funciones

void * producir (void * arg);
void * consumir (void * arg);

//Variables globales

pthread_cond_t no_vacio = PTHREAD_COND_INITIALIZER; 	//controla el llenado del buffer
pthread_cond_t no_lleno = PTHREAD_COND_INITIALIZER;		//controla el vaciado del buffer
pthread_mutex_t semaforo = PTHREAD_MUTEX_INITIALIZER;	//controla acceso a seccion critica

int buffer[MAX_BUFFER] = {0,0,0,0,0};					//Vector para simular buffer
int unidades_disponibles = 0;							//LLeva cuenta de unidades disponibles

int main (){

srand(time(NULL));										//Semilla para aleatorios	
int error;												//Controla errores
int i;													//Variable para bucle for
pthread_t * hilo; 										//Variable para hilos

hilo = (pthread_t *) malloc (sizeof(pthread_t)*2);

	//Se crean los hilos

	error=pthread_create(&hilo[0], NULL ,(void*) producir, NULL);
	validaCreate(error);

	error=pthread_create(&hilo[1], NULL, (void*) consumir, NULL);
	validaCreate(error);

	//Esperamos a que terminen las hebras

	for (i=0; i<2; i++){
		
		error=pthread_join(hilo[i],NULL);
		validaJoin(error);

	}

	//Destruimos los semaforos

	error=pthread_cond_destroy(&no_lleno);
	validaCondDestroy(error);

	error=pthread_cond_destroy(&no_vacio);
	validaCondDestroy(error);

	error=pthread_mutex_destroy(&semaforo);
	validaMutexDestroy(error);

printf("\n\t*** Fin del programa ***\n\n");

exit(EXIT_SUCCESS);

}

//funciones

void * producir (void * arg){

int error;
int pos=0;
int unidades_totales=total_producciones;

	while (TRUE){

		//Si no hemos producido todas las unidades..

		if(unidades_totales != 0){
		
			//Cerramos semaforo y accedemos al buffer (Bloquear seccion critica)

			error=pthread_mutex_lock(&semaforo);
			validaLock(error);

			//Mientras el buffer este lleno... Espera con variable condicional

			while (unidades_disponibles == MAX_BUFFER){

			//Bloquea al Productor

				printf("\n\n\t...Esperando a que se vacie el buffer...\n");
				error=pthread_cond_wait(&no_lleno,&semaforo);
				validaCondWait(error);		

			}

			//Producimos unidades			

			printf("\nProductor: \n");
			pos = (pos +1) % MAX_BUFFER;
			buffer[pos] = rand() % 10;
			unidades_disponibles++;
			printf("Hay %d unidades disponibles\n",unidades_disponibles);	
			unidades_totales--;

			//Mandamos señal y despertamos al consumidor
		
			error=pthread_cond_signal(&no_vacio);
			validaCondSignal(error);

			//abrimos semaforo
		
			error=pthread_mutex_unlock(&semaforo);
			validaUnlock(error);

		}

		//Si hemos producido todas las unidades...

		else{

			printf("\nSe han producido todas las unidades.");
			pthread_exit(NULL);

		}
	}
}


void * consumir (void * arg){

int error;
int pos;
int unidades_consumidas;

	while (TRUE){

		//Si no hemos consumido todas las unidades...

		if(unidades_consumidas != total_producciones){

			//Cerramos semaforo y accedemos al buffer

			error=pthread_mutex_lock(&semaforo);
			validaLock(error);

			//Mientras el buffer este vacío... Espera con variables condicionales				

			while (unidades_disponibles == 0){
			
				//bloqueamos al consumidor

				printf("\n\n\t...Esperando a que se llene el buffer...\n");
				error=pthread_cond_wait(&no_vacio, &semaforo);
				validaCondWait(error);	
			
			}
			
			//Consume unidades


			printf("\nConsumidor: \n");
			pos = (pos +1) % MAX_BUFFER;
			unidades_disponibles--;
			unidades_consumidas++;
			printf("Consumo 1 unidad.\n");
			printf("Total unidades consumidas = %d\n",unidades_consumidas);	

			
			//Mandamos señal y despertamos al productor
		
			error=pthread_cond_signal(&no_lleno);
			validaCondSignal(error);	

			//abrimos semaforo

			error=pthread_mutex_unlock(&semaforo);
			validaUnlock(error);
		}

		//Si hemos consumido todas las unidades...

		else{

			printf("\n*** Todas las unidades han sido consumidas. ***\n\n");

			//abrimos semaforo

			error=pthread_mutex_unlock(&semaforo);
			validaUnlock(error);

			pthread_exit(NULL);		

		}
	}
}
