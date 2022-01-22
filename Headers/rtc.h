#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

char ds3232Data[19];		//Arreglo para guardar la informaci�n del perif�rico

void ds3232Init();		//Inicializar el dispositivo
void ds3232Read();		//Lectura de la hora y dem�s variables que permite el perif�rico
char* getData(char* result);	//Retorna la informaci�n en un arreglo.
