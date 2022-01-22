#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

char ds3232Data[19];		//Arreglo para guardar la información del periférico

void ds3232Init();		//Inicializar el dispositivo
void ds3232Read();		//Lectura de la hora y demás variables que permite el periférico
char* getData(char* result);	//Retorna la información en un arreglo.
