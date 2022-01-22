#include "rtc.h"
#define ADDRESSW  0xD0
#define ADDRESSR  0xD1

void ds3232Init(){
   i2c_start();
   /*D0 is DS3232 Write Address*/
   i2c_write(ADDRESSW);
   /*Select Control Register*/
   i2c_write(0x0E);
   /*Enable SQWE bit blinks at 1 Hz*/
   i2c_write(1<<5);
   i2c_stop();
}

void ds3232Read(){
   /*Second Register*/
   i2c_start();
   i2c_write(ADDRESSW);
   /*Select Second register*/
   i2c_write(0x00);
   i2c_stop();
   int i;
      for(i=0;i<19;i++){
	i2c_start();
	i2c_write(ADDRESSR);		//Dirección de lectura del RTC		
	ds3232Data[i]=i2c_read();
	i2c_stop();
   }
		
}
// Función para obtener los datos del RTC
char* getData(char* result){
   strcpy(result, ds3232Data);
   return result;
}