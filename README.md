# Stepper API
Interfaz de control simple para un motor paso a paso por el puerto serial usando un driver a4988

## Control
El control se realiza mediante un comando separado por comas que tiene la siguiente estructura:

> [TIPO],[DIRECCION(0,1)],[VELOCIDAD(1-6)],[ARGUMENTO],[REPETICIONES],[PAUSA]

  - **TIPO**: Tipo de control, se tienen 3:
    * A: Angulo, controla el angulo que el motor va a girar.
    * T: Tiempo, controla el tiempo que el motor gira.
    * P: Pasos, controla los pasos que el motor gira.
  - **DIRECCION**: 0 = sentido horario, 1 = sentido antihorario.
  - **VELOCIDAD**: 1 = velocidad super lenta, 6 = maxima velocidad.
  - **ARGUMENTO**: Es el argumento de la funcion:
    * Angulo: es el numero de grados que el motor va a girar (ejemplo: 180 para una media vuelta).
    * Tiempo: el numero de milisegundos que el motor va a girar (ejemplo: 3000 para que gire 3 segundos continuos).
    * Pasos: el numero de pasos que el motor gira (ejemplo, 40 pasos).
  - **REPETICIONES**: el numero de veces que se repetira el comando.
  - **PAUSA**: el tiempo de espera entre repeticiones.

## Ejemplos

Si queremos que el motor se mueva *90* grados en sentido *horario* a velocidad maxima:

> A,0,6,90

para mover el motor *60* grados en sentido *antihorario*, *4 veces* a velocidad media con pausas de *medio segundo*:

> A,1,4,60,4,500

Si queremos mover el motor en sentido horario durante 4 segundos con la velocidad minima:

> T,0,1,4000

Si queremos mover el motor en sentido horario a velocidad media durante 1 segundo por 6 veces con pausas de 1 segundo:

> T,0,4,1000,6,1000

Si queremos mover el motor 350 pasos a la derecha a velocidad maxima:

> P,0,6,350



