#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define N 5 
#define izquierda (num_Filosofos+4)%N  //(num_Filosofos-1)%N
#define derecha (num_Filosofos+1)%N  //(num_Filosofos+1)%N

typedef enum {

    pensando, hambriento, comiendo, terminado

} status;

status estado[N];

sem_t mutex;
sem_t S[N];

int numFilosofos[N]={0,1,2,3,4};

//Prototipo de funciones

void * filosofo(void *num);
void cogerTenedor(int);
void soltarTenedor(int);
void comprobar(int);
 
int main(){

int i;
int salida=0;
pthread_t Hilos[N];

    sem_init(&mutex,0,1);	//inicializamos semáforo

    for(i=0;i<N;i++){		//Inicializamos semaforos s a 0

        sem_init(&S[i],0,0);

    }

    for(i=0;i<N;i++){	//Creamos los hilos

        pthread_create(&Hilos[i],NULL,(void *) filosofo , (void*) &numFilosofos[i]);

        printf("filosofo %d esta pensando\n",i+1);

    }

    for(i=0;i<N;i++){	//esperamos a que los hilos finalicen

        pthread_join(Hilos[i],NULL);
        salida++;

    }

    if(salida==5){

    	printf("Todos los filosofos han terminado de comer.\n");

    }
}
 
void * filosofo (void *num){

	while(estado[ *(int *) num] != terminado){

   		int *i;
        i = (int *) malloc (sizeof(int));
        i=num;
  
        usleep(random()%2000000);
        cogerTenedor(*i);

        if (estado[*i] == terminado){

        	printf("El filosofo %d se ha ido.\n", *i+1 );
			sem_post(&S[*i]);
			pthread_exit(NULL);     

        }

        else{

        	usleep(random()%2000000);
        	soltarTenedor(*i);       

        }       
 	}

}
 
void cogerTenedor(int num_Filosofos){

    sem_wait(&mutex);	//bloquea semaforo.
    estado[num_Filosofos] = hambriento;
    printf("filosofo %d esta hambriento\n",num_Filosofos+1);
    comprobar(num_Filosofos);	//comprueba que puede coger los tenedores.
    sem_post(&mutex);	//desbloquea seccion critica
    sem_wait(&S[num_Filosofos]);	//bloquea al filosofo

}
 
void comprobar(int num_Filosofos){

	//si el filosofo está hambriento y los que están a su lado no estan comiendo, el filósofo se pone a comer.

    if (estado[num_Filosofos] == hambriento && estado[izquierda] != comiendo && estado[derecha] != comiendo){
        estado[num_Filosofos] = comiendo;
	    usleep(random()%2000000);        
        printf("filosofo %d coge tenedor %d y %d\n",num_Filosofos+1,izquierda+1,num_Filosofos+1);
        printf("filosofo %d come y termina.\n",num_Filosofos+1);
        estado[num_Filosofos] = terminado;

    }
}
 
void soltarTenedor(int num_Filosofos){

    sem_wait(&mutex);	//Cerramos semáforo
    estado[num_Filosofos] = pensando;
    printf("filosofo %d suelta tenedor %d y %d\n",num_Filosofos+1,izquierda+1,num_Filosofos+1);
    printf("filosofo %d esta pensando\n",num_Filosofos+1);
    comprobar(izquierda);
    comprobar(derecha);
    sem_post(&mutex);	//Abrimos semáforo

}