#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

void serial_begin();						//Inicio de la comunicación Serial
unsigned char serial_read_char();			//Lectura de un caracter
void serial_print_char(unsigned char caracter);	//Escritura de un caracter
void serial_print_str(char* cadena);			//Escritura de una cadena
void serial_println_str(char *cadena);			//Escritura de una cadena con salto de línea
char* get_RX_buffer();						//Función para obtener buffer
bool is_data_ready();						//Verifica el estado de la comunicación
