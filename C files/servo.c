#include "servo.h"
 
 void init_servo(){
  DDRC |= 0x01; //Makes Pin C0 output pin
 }

 ////Funci�n para girar el servo un �ngulo positivo de 180�
void girarServo(){
   PORTC = 0x01;
    _delay_us(2000);		//Se genera una se�al de 2 ms
    PORTC = 0x00;
    _delay_ms(1000);
}

////Funci�n para girar el servo un �ngulo negativo de 180�
void regresarServo(){
    PORTC = 0x01;
    _delay_us(999);		//Se genera una se�al de 1 ms
    PORTC = 0x00;
    _delay_ms(1000);
}
