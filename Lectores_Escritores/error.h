#ifndef ERROR_H_
#define ERROR_H_

void validaCreate(int e);//Funcion captadora de errores en creacion de hilos
void validaJoin(int e);//Funcion captadora de errores en recepcion de hilos
void validaLock(int e);//Funcion captadora de errores en bloqueo de semáforo
void validaUnlock(int e);//Funcion captadora de errores en desbloqueo de semaforo
void validaMutexDestroy(int e);
void validaCondDestroy(int e);
void validaCondWait(int e);
void validaCondSignal(int e);

#endif