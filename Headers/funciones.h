#include <avr/eeprom.h> //archivo de cabecera para la utilización de la EEPROM AVR
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "lcd_i2c.h"
#include "uart.h"
#include "rtc.h"
#include "servo.h"
#include "hcsr04.h"
#include "keypad.h"


void enviar_nivel( unsigned char c ); 	//Función de comunicación Serial
