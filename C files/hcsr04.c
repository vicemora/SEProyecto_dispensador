#include "hcsr04.h"

//Inicialización del dispositivo
void init_distancia(){
   DDRD |= 0b00010000;		//Pin D4 se declara como salida para controlar el TRIGGER
   PORTD |= 0b00000000;
}

//Se declara una interrupción que permitirá utilizar un reloj para medir el tiempo
//En que la señal de salida del TRIGGER regresa al ECHO
void interrup_distancia(){
   cli();			//Desactivar interrupciones
   EICRA = 0b01;	//Interrupcion por cualquier cambio lógico
   EIMSK =1;		//Interrupción en el pin INT0 (D2)
   TCCR1B=0b01;	//Timmer 1 sin preescalador
   sei();			//Activar interrupciones
}
