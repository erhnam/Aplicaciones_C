Prácticas de Sistemas Operativos
=================================

# PROBLEMA DEL PRODUCTOR CONSUMIDOR CON SEMAFOROS BINARIOS Y VARIABLES DE CONDICIÓN

Usando variables de condición junto con mutexs para evitar espera activa. Cuando el consumidor toma un producto notifica al productor que puede comenzar a trabajar nuevamente porque al menos hay un hueco donde poner una producción. En caso contrario si el buffer se vacía, el consumidor se pone a dormir y en el momento en que el productor agrega un producto crea una señal para despertarlo. Consulte la documentación de clases de teoría si lo considera oportuno.

