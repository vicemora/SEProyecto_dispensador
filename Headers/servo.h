#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

 void init_servo();		//Inicio de configuraci�n del servo
 void girarServo();		//Funci�n para girar el servo un �ngulo positivo de 180�
 void regresarServo();		//Funci�n para girar el servo un �ngulo negativo de 180�