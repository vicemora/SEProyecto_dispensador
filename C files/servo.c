#include "servo.h"
 
 void init_servo(){
  DDRC |= 0x01; //Makes Pin C0 output pin
 }

 ////Función para girar el servo un ángulo positivo de 180°
void girarServo(){
   PORTC = 0x01;
    _delay_us(2000);		//Se genera una señal de 2 ms
    PORTC = 0x00;
    _delay_ms(1000);
}

////Función para girar el servo un ángulo negativo de 180°
void regresarServo(){
    PORTC = 0x01;
    _delay_us(999);		//Se genera una señal de 1 ms
    PORTC = 0x00;
    _delay_ms(1000);
}
