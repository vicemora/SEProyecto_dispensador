#include "hcsr04.h"

//Inicializaci�n del dispositivo
void init_distancia(){
   DDRD |= 0b00010000;		//Pin D4 se declara como salida para controlar el TRIGGER
   PORTD |= 0b00000000;
}

//Se declara una interrupci�n que permitir� utilizar un reloj para medir el tiempo
//En que la se�al de salida del TRIGGER regresa al ECHO
void interrup_distancia(){
   cli();			//Desactivar interrupciones
   EICRA = 0b01;	//Interrupcion por cualquier cambio l�gico
   EIMSK =1;		//Interrupci�n en el pin INT0 (D2)
   TCCR1B=0b01;	//Timmer 1 sin preescalador
   sei();			//Activar interrupciones
}
