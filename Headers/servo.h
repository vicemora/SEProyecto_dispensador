#ifndef F_CPU
#define F_CPU 8000000UL // 8 MHz clock speed
#endif

#include <avr/io.h>
#include <util/delay.h>

 void init_servo();		//Inicio de configuración del servo
 void girarServo();		//Función para girar el servo un ángulo positivo de 180°
 void regresarServo();		//Función para girar el servo un ángulo negativo de 180°