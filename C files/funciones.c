#include "funciones.h"

//Espacio disponible para funciones extras

/**
Envia la distancia o el nivel a la raspberry para la comunicación con Ubidots
/*/
void enviar_nivel( unsigned char c ){
  char dist[20];
	sprintf(dist,"%i", c);
	serial_println_str(dist);
	_delay_ms(1000);
 
}
