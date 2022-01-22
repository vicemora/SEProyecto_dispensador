#include <avr/io.h>
#include "lcd_i2c.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>


void init_distancia();		//Configuración inicial del dispositivo
void interrup_distancia();	//Interrupción necesaria para procesar la variable a medir

