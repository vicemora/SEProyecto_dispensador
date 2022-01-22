#include "funciones.h"

//Se declaran las variables para la información a guardar en la EPROM
uint8_t EEMEM hora1;
uint8_t EEMEM minutos1;
uint8_t EEMEM hora2;
uint8_t EEMEM minutos2;
uint8_t EEMEM factor_actividad;
uint8_t EEMEM factor_raza;
uint8_t EEMEM inicio;			//Esta variable determina si el sistema ya había sido iniciado (0 cuando no necesita configuracion inicial)

//Se declaran variable globales a utilizar en el sistema
int dispenso = 0;			//Guarda si se ha dispensado o no la comida en el minuto dado
volatile unsigned short pulse;	//Guarda el pulso que se genera en la señal enviada por el sensor de distancia (tiempo de recepción)
unsigned char distanciaSensor = 0;		//Guarda la distancia sensada
static volatile int i = 0;		//Detecta el estado actual de la interrupción del sensor de distancia
char horariosGuardados[4][6] = { {"hh:mm "},				//Array para guardar los horarios
						  {"hh:mm "},
						  {"hh:mm "},
						  {"hh:mm "} };
int factor_act=0; // factor correspondiente a la actiidad fisica de la mascota
int factor_r=0; // factor correspondiente al tipo de raza de la mascota
int nHorarios = 2;	//Número de horarios de alimentación de la mascota

/**Se manejará una interrupción para el sensor de distancia, esto debido a que el TRIGGER
envía una señal que el ECHO detecta, esa detección se realizará mediante un interrupción
en el pin D2 o INT0**/
ISR( INT0_vect ){
   if(i == 0){
      TCCR1B |= 0b01;
      i = 1;
   }
   else{
      TCCR1B = 0;
      pulse = TCNT1;
      TCNT1 = 0;
      i = 0;
   }
}

/**
   En esta función se emite un pulso para medir la distancia por parte del sensor
**/
void leer_dist(){
   PORTD |= 1<<PIND4;
   _delay_us(15);						//Se genera un pulso para que el TRIGGER emita una señal
   PORTD &= ~(1<<PIND4);
   distanciaSensor= (pulse*2)/115;	//result= pulse/58 	//Según la hoja de datos debería dividirse para 58
}


/**
Permite al usuario seleccionar el tipo de actividad física de su mascota y selecciona un factor en virtud de este nivel*/
void configurarFactor(){

   //Presentacion de instrucciones
  lcd_cursor(0,0);
  lcd_msg("********************");
  lcd_cursor(1,0);
  lcd_msg("Seleccionar nivel de");
  lcd_cursor(2,0);
  lcd_msg("  actividad fisica");
  lcd_cursor(3,0);
  lcd_msg("    de la mascota");

  _delay_ms(2000);  // delay que permite al usuario observar el mensaje completamente

  lcd_clear();
  //Presenta al usuario menú de selección de nivel de actividad física de la mascota
  lcd_cursor(0,0);
  lcd_msg("Escoja una opcion: ");
  lcd_cursor(1,0);
  lcd_msg("1.Baja ");
  lcd_cursor(2,0);
  lcd_msg("2.Normal ");
  lcd_cursor(3,0);
  lcd_msg("3.Alta ");
  
  int i=1; //variable de control que asegura que el menu permanezca hasta que el usuario realice alguna acción
  while(i){

    char key=keyfind(); //check for pressed key
      //verifica el valor de la tecla presionada y asigna el valor a la variable del factor
      switch(key){
        case '1':
        //cuando el nivel de actividad física es bajo el multiplicador asignado es 1
	        factor_act=1;
          i=0; //sale del bucle
	         break;
	 
        case '2':
         //cuando el nivel de actividad física es medio el multiplicador asignado es 2
	        factor_act=2;
          i=0; //sale del bucle
	        break;

        case '3':
         //cuando el nivel de actividad física es alto el multiplicador asignado es 3
	        factor_act=3;  
          i=0; //sale del bucle
	        break;
      
      }
  }
  eeprom_write_byte(&factor_actividad,factor_act);
  lcd_clear(); // Limpia la pantalla antes de salir de la función
}

/**
  Esta función le permite al usuario configurar el tamaño de raza pequeña/mediana/grande, esta información se asigna con un factor para poder calcular la cantidad de alimento dispensada
*/
void configurarTipoRaza(){
  
   //Presentacion de instrucciones
  lcd_cursor(0,0);
  lcd_msg("********************");
  lcd_cursor(1,0);
  lcd_msg("Seleccionar tipo de");
  lcd_cursor(2,0);
  lcd_msg(" raza de la mascota");
  lcd_cursor(3,0);
  lcd_msg("**********************");

  _delay_ms(2000);  // delay que permite al usuario observar el mensaje completamente

  lcd_clear();
  //Presenta al usuario menú de selección de nivel de actividad física de la mascota
  lcd_cursor(0,0);
  lcd_msg("Escoja una opcion: ");
  lcd_cursor(1,0);
  lcd_msg("1.Raza pequena ");
  lcd_cursor(2,0);
  lcd_msg("2.Raza mediana");
  lcd_cursor(3,0);
  lcd_msg("3.Raza grande ");
  
  int i=1; //variable de control que asegura que el menu permanezca hasta que el usuario realice alguna acción
  while(i){

    char key=keyfind(); //check for pressed key
      //verifica el valor de la tecla presionada y asigna el valor a la variable del factor
      switch(key){
        case '1':
        //cuando el tipo de raza es pequeña se la asigna un factor de 1
	        factor_r=1;
          i=0; //sale del bucle
	        break;
	 
        case '2':
         //cuando el tipo de raza es medianase la asigna un factor de 2
	        factor_r=2;
          i=0; //sale del bucle
	        break;

        case '3':
         //cuando el tipo de raza es grande se la asigna un factor de 4
	        factor_r=4;  
          i=0; //sale del bucle
	        break;
      
      }
  }
  eeprom_write_byte(&factor_raza,factor_r);
  lcd_clear(); // Limpia la pantalla antes de salir de la función
}


/** Esta fución permite realizar la dispensación del alimento. El sistema de dispenso de paletas es tal que en un periodo de servo (ida y vuelta) se dipensan 45 g de alimento para perro. La cantidad de alimento dispensado varía según el tipo de raza y la actividad física de la mascota. 
*/
void dipensarAlimento(){
  int n;
  n=(factor_act)*(factor_r) - (factor_act-1)*(factor_r/2); //calculando la cantidad de periodos de servo (se resta un factor de compensación)
  int i; // variable de control de lazo for
  for(i=0;i<n;i++){
    girarServo(); //Servo gira 180 grados
    _delay_ms(500); //pequeña pausa
    regresarServo(); //Servo regresa a su posición original
    _delay_ms(500);//pequeña pausa
  }	   
}



/**
Función para ingresar los horarios de comida de la mascota guardando la información
en la EPROM.
int numHorraio corresponde a las veces a alimentar a la mascota
**/
void configurarHora(int numHorario){
      char numStr[2];
      sprintf(numStr,"%d",numHorario);	
      uint8_t *horaEprom;						//Variable generales para guardar la dirección de memoria de la variable en la EPROM
      uint8_t *minEprom;
      switch(numHorario){						//Se escogen las direcciones según el número de hora a guardar
	 case 1: horaEprom=&hora1; minEprom=&minutos1; break;
	 case 2: horaEprom=&hora2; minEprom=&minutos2;break;
	 case 3: horaEprom=&hora2; minEprom=&minutos2;break;
	 case 4: horaEprom=&hora2; minEprom=&minutos2;break;
	 default: horaEprom=&hora1; minEprom=&minutos1;break;
      }
      lcd_clear();
      lcd_cursor(0,0);
      lcd_msg("  Ingrese Hora ");								//Mostrar mensaje de acción
      lcd_msg(numStr);
      lcd_msg(":   ");
      uint8_t horaActual = eeprom_read_byte(horaEprom);			//Leer hora guardada   
      char horaStr[2];										//Cadena para guardar la hora en String
      sprintf(horaStr,"%.2d",horaActual);							//Pasar hora a string
      lcd_cursor(1,7);
      lcd_msg(horaStr);									//Imprimir hora
      uint8_t minutosActual = eeprom_read_byte(minEprom);			//Leer minutos guardados
      char minutosStr[2];									//Cadena para guardar los minutos en String
      sprintf(minutosStr,"%.2d ",minutosActual);  					//Pasar minutos a string
      lcd_msg(":");
      lcd_msg(minutosStr);									//Imprimir minutos
      int i =0;											//Contador de iteraciones
      char nuevaHora[6]={0,0,':',0,0,0};				//Arreglo para la nueva hora con formato hh:mm
      while(i<5){						//Iterar para guardar la hora
	 if(i==2){
	    lcd_printc(':');
	    i++;				//Saltar los dos puntos
	 }
	 char key=keyfind();		//Se lee la tecla de entrada
	 _delay_ms(50); 
	 if(key){	
	    if(!(key=='*')){			//Mientras no se ingresa '*' se lee la nueva hora
	       lcd_cursor(1,i+7);		//Se ubica el cursor en el número a modificar
	       lcd_printc(key);		//Se imprime el número ingresado
	       nuevaHora[i]=key;	//Se guarda el número en la cadena
	       i++;				//Se pasa al sigueinte número a modificar
	    } else{
	       i=5;				//Si se presiona '*' se termina el bucle while
	    }
	 }  
      }
      lcd_cursor(2,0);
      if(((nuevaHora[0]<'3' && nuevaHora[1]<'4') || (nuevaHora[0]<'2')) && (nuevaHora[3]<'6')){			//Se verifica la hora ingresada entre 0 y 23
	 lcd_cursor(3,0);
	 lcd_msg("  Ingreso Exitoso    ");													//Imprimir Validación
	 _delay_ms(1000); 
	 int enteroHora = ((nuevaHora[0] - '0')*10 + (nuevaHora[1] - '0')) ;					//Se transforma a entero las horas
	 eeprom_write_byte(horaEprom,enteroHora);								//Se actualiza el EPROM con las horas
	 int enteroMinutos =  ((nuevaHora[3] - '0')*10 + (nuevaHora[4] - '0')) ;				//Se transforma a entero los minutos
	 eeprom_write_byte(minEprom,enteroMinutos);								//Se actualiza el EPROM con los minutos
	 strcpy(horariosGuardados[numHorario-1], nuevaHora);							//Se guarda el horario en una variable local para no leer demasiadas veces a la EEPROM
	 }
      else{
	 lcd_cursor(2,0);
	 lcd_msg("   Hora no valida   ");			//Imprimir Validación
	  lcd_cursor(3,0);
	 lcd_msg(" Hora no Ingresada  ");			//No se ha guardado la información
	 _delay_ms(1000); 
      } 
      lcd_clear();
}

/**
   Función que será llamada cuando el usuario requiera cambiar la conffiguración de las horas de comida
**/
void configuracionHorarios(int numHorarios){
   //Presentacion de instrucciones
   lcd_cursor(0,0);
   lcd_msg("********************");
   lcd_cursor(1,0);
   lcd_msg("    Ingreso de los  ");
   lcd_cursor(2,0);
   lcd_msg("horarios de dispenso");
   lcd_cursor(3,0);
   lcd_msg("********************");
   _delay_ms(1500);  // delay que permite al usuario observar el mensaje completamente
   int i;
   for(i=0; i<numHorarios; i++){
      configurarHora(i+1);
   }
}

/**
   Esta función se encarga de cargar los datos guardados en la EPROM a las variables globales
**/
void cargarDatos(){
   int cont;
   for(cont=1;cont<5;cont++){
      uint8_t *horaEprom;						//Variable generales para guardar la dirección de memoria de la variable en la EPROM
      uint8_t *minEprom;
      switch(cont){										//Se escogen las direcciones según el número de hora a guardar
	 case 1: horaEprom=&hora1; minEprom=&minutos1; break;
	 case 2: horaEprom=&hora2; minEprom=&minutos2;break;
	 case 3: horaEprom=&hora2; minEprom=&minutos2;break;
	 case 4: horaEprom=&hora2; minEprom=&minutos2;break;
	 default: horaEprom=&hora1; minEprom=&minutos1;break;}
      uint8_t horaActual = eeprom_read_byte(horaEprom);			//Leer horas guardadas
      char horaStr[2];										//Cadena para guardar la hora en String
      sprintf(horaStr,"%.2d",horaActual);							//Pasar hora a string
      uint8_t minutosActual = eeprom_read_byte(minEprom);			//Leer minutos guardados
      char minutosStr[2];									//Cadena para guardar los minutos en String
      sprintf(minutosStr,"%.2d ",minutosActual);					//Pasar minutosa string
      char nuevaHora[6]={0,0,':',0,0,0};							//Cadena para el formato de la hora
      nuevaHora[0]=horaStr[0];							//Se guarda la información del EPROM en la nueva variable
      nuevaHora[1]=horaStr[1];
      nuevaHora[3]=minutosStr[0];
      nuevaHora[4]=minutosStr[1];
      strcpy(horariosGuardados[cont-1], nuevaHora);			//Se guarda la información en la variable global
   }
   
   factor_act = eeprom_read_byte(&factor_actividad);
   factor_r = eeprom_read_byte(&factor_raza);
   
}

int main(void)
{	
   i2c_init();		//Inicio del i2c
   lcd_init(); 		//Inicio del LCD
   ds3232Init();		//Inicio del RTC
   init_servo();		//Inicio del servomotor
   serial_begin();		//Inicio de la comunicación Serial
   init_distancia();	//Inicio del sensor de distancia
   interrup_distancia();	//Se llama a la declaración de la interrupción para el sensor de distancia
   DDRD |= 0b10000000;	//Se declara el pin D7 como salida para controlar un led
   
   char rtcData[19],rtcDatatring[6]={0,0,0,0,0,0};
   
   char distancia[16];
   
      
   //Mensaje de Bienvenida
      lcd_cursor(0,0);
      lcd_msg("********************");
      lcd_cursor(1,0);
      lcd_msg("     Bienvenido     ");
      lcd_cursor(3,0);
      lcd_msg("********************");
      _delay_ms(2000); 
      lcd_clear();
      
      int activacion = eeprom_read_byte(&inicio);
      if(activacion){
	 eeprom_write_byte(&hora1,10);				//Iniciamos algunas horas de muestra en la EPROM
	 eeprom_write_byte(&minutos1,11);
	 eeprom_write_byte(&hora2,18);
	 eeprom_write_byte(&minutos2,11);
	 
	 configuracionHorarios(nHorarios);				 	//Configurar Horarios
	 configurarTipoRaza();					//Elegir el tipo de raza
	 configurarFactor();						//Configuración del factor de alimentación por actividad física
	 
	 eeprom_write_byte(&inicio,0);				//Se guarda que no necesita configuración inicial
      }
      else{
	 cargarDatos();							//Cargar datos ya existentes de la EPROM al sistema.
      }
     
   while (1) {		

      
      ds3232Read();				//Lectura de la hora por el RTC
      getData(rtcData);				//Actualización de la hora
       
      char key=keyfind();			//Se revisa si se está ingresando a algun modo de configuración por tecla
    
      if(key=='/'){ 								//La tecla '/' inicia el modo configuracion de horario 1
	 configurarHora(1);
	  _delay_ms(50); 
      }
          
      if(key=='*'){ 								//La tecla '*' inicia el modo configuracion de horario 2
	 configurarHora(2);
	  _delay_ms(50); 
      }
      
      if(key=='A'){ 								//La tecla 'A' inicia el modo configuracion de todos horarios
	enviar_nivel(distanciaSensor);
      }

      if(key=='-'){ 								//La tecla '-' inicia el modo elección del tipo de raza
	 configurarTipoRaza();
	  _delay_ms(50); 
      }
      
      if(key=='+'){ 							//La tecla '+' inicia el modo configuracion del factor de alimentación por actividad física
	 configurarFactor();
	  _delay_ms(50); 
      }
      
      if (!distanciaSensor){								//En esta condición se revisa si existe guardado un valor de distancia
	 int verif_dis;									//Si no existe se sensa,en especial al iniciar el sistema
	 for(verif_dis=0;verif_dis<10;verif_dis++){
	    leer_dist();
	 }
      }
      
      lcd_cursor(0,0);
      lcd_msg("Distancia: ");
      lcd_cursor(1,0);
      lcd_msg("Tiempo: ");
      sprintf(rtcDatatring,"%.2x:%.2x",rtcData[2],rtcData[1]);		//Se da formato a la hora y minutos obtenidos del RTC hh:mm
      lcd_msg(rtcDatatring);								//Se muestra la hora
      lcd_cursor(0,12);
      sprintf(distancia,"%d ",distanciaSensor);
      lcd_msg(distancia);								//Se muestra la distancia

      if(!((strcmp(rtcDatatring,horariosGuardados[1])) && (strcmp(rtcDatatring,horariosGuardados[0])) && 
	 (strcmp(rtcDatatring,horariosGuardados[2])) && (strcmp(rtcDatatring,horariosGuardados[3]))) ){		//Aquí se verifica si es la hora de dispenso
	 if(!dispenso){						//Si no ha habido dispenso y ya es la hora
	    dipensarAlimento();	   
	    int e;
	    for(e=0;e<10;e++){
	       leer_dist();						//Cuando se ha dispensado se lee el nuevo nivel de la comida
	       if(distanciaSensor>27){
		  PORTD |= 0b10000000;			//Si la distnacia supera los 27 cm se deja encendido un indicador para hacer acuerdo al usuario.
	       }else{
		  PORTD = PORTD & ~0b10000000;	//Si se ha llenado el contenedor, en la próxima medición se apagará el indicador
	       }
	       enviar_nivel(distanciaSensor);
	    }
	    dispenso = !dispenso;				//Ahora que se ha dispensado se guarda dicho estado en la variable global
	    }
      }else{
	 dispenso = 0;						//Si no es la hora se indica que no se ha dispensado comida para el nuevo horario
      }

    }
    
    _delay_ms(100);
    
}

