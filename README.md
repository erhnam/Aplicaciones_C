Prácticas de Sistemas Operativos
=================================

# PROBLEMA DEL PRODUCTOR CONSUMIDOR CON SEMAFOROS BINARIOS Y VARIABLES DE CONDICIÓN

Usando variables de condición junto con mutexs para evitar espera activa. Cuando el consumidor toma un producto notifica al productor que puede comenzar a trabajar nuevamente porque al menos hay un hueco donde poner una producción. En caso contrario si el buffer se vacía, el consumidor se pone a dormir y en el momento en que el productor agrega un producto crea una señal para despertarlo. Consulte la documentación de clases de teoría si lo considera oportuno.

# PROBLEMA FILOSOFOS COMENSALES

Cinco filósofos viven en una casa, donde hay una mesa preparada para ellos. Básicamente, la vida de cada filósofo consiste en pensar y comer, y después de años de haber estado pensando, todos los filósofos están de acuerdo en que la única comida que contribuye a su fuerza mental son los espaguetis.
Cinco filósofos viven en una casa, donde hay una mesa preparada para ellos. Básicamente, la vida de cada filósofo consiste en pensar y comer, y después de años de haber estado pensando, todos los filósofos están de acuerdo en que la única comida que contribuye a su fuerza mental son los espaguetis. Se presentan las siguientes suposiciones y/o restricciones:
• Debido a su falta de habilidad manual, cada filósofo necesita dos tenedores para comer los espaguetis, cogiendo solo un tenedor a la vez (en cualquier orden, izquierda-derecha o derecha-izquierda).
• La disposición para la comida es simple (ver figura): una mesa redonda en la que está colocado un gran cuenco para servir espaguetis, cinco platos, uno para cada filósofo, y cinco tenedores.
13
￼• Un filósofo que quiere comer utiliza,en caso de que estén libres, los dos tenedores situados a cada lado del plato, retira y come algunos espaguetis.
• Si cualquier filósofo toma un tenedor (solo un tenedor a la vez) y el otro está ocupado, se quedará esperando, con el tenedor en la mano, hasta que pueda tomar el otro tenedor, para luego empezar a comer.
• Si un filósofo piensa, no molesta a sus colegas. Cuando un filósofo ha terminado de comer suelta los tenedores y continua pensando.
• Si dos filósofos adyacentes intentan tomar el mismo tenedor a una vez, se produce una condición de carrera: ambos compiten por tomar el mismo tenedor, y uno de ellos se queda sin comer.
• Si todos los filósofos toman el tenedor que está a su derecha al mismo tiempo (o a su izquierda), entonces todos se quedarán esperando eternamente, porque alguien debe liberar el tenedor que les falta. Nadie lo hará porque todos se encuentran en la misma situación (esperando que alguno deje sus tenedores). Se produce un interbloqueo (deadlock o abrazo de la muerte). Entonces los filósofos se morirán de hambre.
El problema: diseñar un ritual (algoritmo) que permita a los filósofos comer. El algoritmo debe satisfacer la exclusión mutua (no puede haber dos filósofos que puedan utilizar el mismo tenedor a la vez) y evitar el interbloqueo e inanición (en este caso, el término inanición tiene un sentido literal, además de algorítmico). A partir del siguiente esquema de pseudocódigo y del que está disponible en Moodle, además de consultar la Web7 para informarse en profundidad del problema, implemente una solución en C utilizando semáforos que resuelva el problema de sincronización e inanición de la cena de los filósofos.
