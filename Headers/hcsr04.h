#include <avr/io.h>
#include "lcd_i2c.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>


void init_distancia();		//Configuraci�n inicial del dispositivo
void interrup_distancia();	//Interrupci�n necesaria para procesar la variable a medir

