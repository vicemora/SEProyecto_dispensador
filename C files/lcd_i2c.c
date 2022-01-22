#include "lcd_i2c.h"
#define RS	0	// 2^0 = 1; se asigna el pin 1como RS
#define EN	1	// 2^1 = 2; se asigna el pin 2 como EN
				// el pin R/W va a tierra
#define ADDRESS  0x70


void lcd_cmd_hf(char v1)		//configurar de modo 8bits a 4bits de lcd
{

	 char x = v1 & ~(1<<RS);		//RS debe ser cero para que lcd reciba comandos
	  i2c_write(x | (1<<EN));
	 _delay_ms(15);
	  i2c_write(x & ~(1<<EN));
}

void lcd_cmd(char v2)			//enviar comandos de configuracion ya en 4 bits
{

	char x = (v2 & 0xF0) & ~(1<<RS);		//envio 4 bits mas significativos (p4-p7)
	 i2c_write(x | (1<<EN));
	 _delay_ms(2);
	  i2c_write(x & ~(1<<EN));
	
	x = (v2 <<4) & ~(1<<RS);	//envio 4 bits menos significativos desplazados (p4-p7)
	i2c_write( x | (1<<EN));
	 _delay_ms(2);
	  i2c_write( x & ~(1<<EN));

}

void lcd_dwr(char v3)
{

	char x = (v3 & 0xF0) | (1<<RS);
	 i2c_write(x | (1<<EN));
	 _delay_us(50);
	  i2c_write(x & ~(1<<EN));
	
	x = (v3<<4) | (1<<RS);
	i2c_write( x | (1<<EN));
	 _delay_us(50);
	  i2c_write( x & ~(1<<EN));
}

void lcd_init()
{
	i2c_start();
	i2c_write(ADDRESS);
   
        lcd_cmd_hf(0x30);	      //-----Sequence for initializing LCD
	lcd_cmd_hf(0x30);       //-----     "            "              "               "
	lcd_cmd_hf(0x30);       //-----     "            "              "               "
	lcd_cmd_hf(0x20);       //-----     "            "              "               "
   
	lcd_cmd(0x28);          //-----Selecting 16 x 2 LCD in 4Bit mode
	lcd_cmd(0x0C);          //-----Display ON Cursor OFF
	lcd_cmd(0x01);          //-----Clear display
	lcd_cmd(0x06);          //-----Cursor Auto Increment
   
	i2c_stop();
}

void lcd_msg(char *c)
{
	i2c_start();
	i2c_write(ADDRESS);
   
	while(*c != 0)      //----Wait till all String are passed to LCD
	    lcd_dwr(*c++);		//----Send the String to LCD
	
	i2c_stop();
}

void lcd_rig_sh()
{
	i2c_start();
	i2c_write(ADDRESS);
	lcd_cmd(0x1C);      //----Command for right Shift
	i2c_stop();
}

void lcd_lef_sh()
{
	i2c_start();
	i2c_write(ADDRESS);
	lcd_cmd(0x18);      //----Command for Left Shift
	i2c_stop();
}

void lcd_cursor(char x, char y){
        if(y>=0 && y<40){
	   i2c_start();
	   i2c_write(ADDRESS);
	   if(x == 0 || x ==1){
	    lcd_cmd(0x80 | (x*64+y));
	   }else if(x == 2 || x ==3){
	    lcd_cmd(0x80 | ((x-2)*64+y+20));
	   }   
	    i2c_stop();

	}
	
}

void lcd_printc(char c){
   
      char str[2];
      sprintf(str,"%c", c);
      lcd_msg(str);
   
 }
 
 void lcd_clear(){        //Función para limpiar la pantalla del LCD
   i2c_start();
   i2c_write(ADDRESS);
   lcd_cmd(0x01);          //-----Clear display
   i2c_stop();
}
 
 
 