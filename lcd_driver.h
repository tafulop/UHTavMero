#ifndef __LCD_DRIVER__
#define __LCD_DRIVER__

#define F_CPU 16000000UL

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


#define LCD_DATA_DDR		DDRE
#define LCD_CMD_DDR			DDRF

#define LCD_DATA_PORT		PORTE
#define LCD_CMD_PORT		PORTF

#define LCD_RS				PF1
#define LCD_RW				PF2
#define LCD_EN				PF3


/* 
 
----------
HOW TO USE
----------

1.	-	Call lcd_init() function

2.	-	Use a function listed below

-	lcd_put_data(unsigned char) // Writes a character to to the current position of the cursor.
		
-	lcd_put_string(&string)		//Writes a string to the lcd from the current position of the cursor.

-	lcd_goto_yx(char,char)		// Moves the cursors to the given coordinate

-	lcd_put_int(integer 16 bit)	// Writes an integer to the LCD from the current position of the cursor

-	lcd_put_bin(8-bit binary)	// Writes the given binary to the LCD from the current position of the cursor

-	CreateCGram					// Creates a special character


*/



void lcd_init();
void lcd_put_data(uint8_t data);
void lcd_cmd(uint8_t cmd);
void lcd_clock();
void lcd_put_string(uint8_t *string);
void CreateCGram();
void lcd_goto_yx(uint8_t y, uint8_t x);
void lcd_put_int(uint16_t num);
void lcd_put_freq(uint16_t num);
void lcd_put_bin(uint8_t number);
void lcd_clr_line(unsigned char);
void lcd_clr();



void lcd_init()
{
	LCD_DATA_DDR |= 0xF0;
	LCD_CMD_DDR |= ((1<<LCD_RS)
	|(1<<LCD_RW)
	|(1<<LCD_EN));

	LCD_CMD_PORT &= ~(1<<LCD_RW);
	//konfig

	lcd_cmd(0x30);
	_delay_ms(5);
	lcd_cmd(0x30);
	_delay_ms(1);
	lcd_cmd(0x30);
	_delay_ms(1);
	lcd_cmd(0x20);
	_delay_ms(1);
	lcd_clock();
	lcd_clock();
	lcd_clock();

	//LCD konf
	lcd_cmd(0x28);
	lcd_cmd(0x0C);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
	lcd_cmd(0x02);
}


void lcd_clr_line(unsigned char line)
{
unsigned char i;

lcd_goto_yx(line,0);

	for(i=0;i<15;i++)
	{
		lcd_put_data(' ');
	}
}


void lcd_clr()
{
	lcd_cmd(0x01);
}

void lcd_put_data(uint8_t data)
{
	LCD_CMD_PORT |= (1<<LCD_RS); //adatot küldünk
	LCD_CMD_PORT &= ~(1<<LCD_RW);
	LCD_CMD_PORT &= ~(1<<LCD_EN);

	LCD_DATA_PORT &= 0x0F;
	LCD_DATA_PORT |= (data&0xF0);
	lcd_clock();
	LCD_DATA_PORT &= 0x0F;
	LCD_DATA_PORT |= ((data<<4)&0xF0);
	lcd_clock();
}

void lcd_cmd(uint8_t cmd)
{
	LCD_CMD_PORT &= ~(1<<LCD_RS); //parancsot küldünk
	LCD_CMD_PORT &= ~(1<<LCD_RW);
	LCD_CMD_PORT &= ~(1<<LCD_EN);

	LCD_DATA_PORT &= 0x0F;
	LCD_DATA_PORT |= (cmd&0xF0);
	lcd_clock();
	LCD_DATA_PORT &= 0x0F;
	LCD_DATA_PORT |= ((cmd<<4)&0xF0);
	lcd_clock();
}

void lcd_clock()
{
	LCD_CMD_PORT |= (1<<LCD_EN);
	_delay_ms(1);
	LCD_CMD_PORT &= ~(1<<LCD_EN);
	_delay_ms(1);
}

void lcd_put_string(uint8_t *p)
{
unsigned char counter = 0;

	while(*p)
	{
		lcd_put_data(*p++);

		counter++;
		
		//lcd_goto_yx(2,0);
		
		switch(counter)
		{
		case 15: 	lcd_goto_yx(1,0);
					break;
		case 31: 	lcd_goto_yx(2,0);
					break;
		case 47: 	lcd_goto_yx(3,0);
					break;
		case 63: 	lcd_goto_yx(0,0);
					break;
		}	
	}


}

void lcd_put_bin(uint8_t number)
{

unsigned char iterator;
unsigned char bin_string[8];


	for(iterator = 0;iterator<=7;iterator++)
	{
		if(number & (128>>iterator))
		{
			lcd_put_data('1');
		}		
		else
		{
			lcd_put_data('0');
		}
	}

}


void lcd_put_int(uint16_t num)
{
	if(num>9999)lcd_put_data('0'+num/10000%10);
	if(num>999)lcd_put_data('0'+num/1000%10);
	if(num>99)lcd_put_data('0'+num/100%10);
	if(num>9)lcd_put_data('0'+num/10%10);
	lcd_put_data('0'+num%10);
}

void lcd_put_freq(uint16_t num)
{
	if(num>9999)lcd_put_data('0'+num/10000%10);
	if(num>999)lcd_put_data('0'+num/1000%10);
	if(num>99)lcd_put_data('0'+num/100%10);
	lcd_put_data('0'+num/10%10);
	lcd_put_data('.');
	lcd_put_data('0'+num%10);
}

void lcd_goto_yx(uint8_t y, uint8_t x)
{
	switch(y)
	{
		case 0: {y= 0x00 + 0x80 + x;break;}
		case 1: {y= 0x40 + 0x80 + x;break;}
		case 2: {y= 0x10 + 0x80 + x;break;}
		case 3: {y= 0x50 + 0x80 + x;break;}

		default: break;
	}
	lcd_cmd(y);
}

void CreateCGram()
{
	lcd_cmd(0x40+(0<<3));	//címek : 44..

	lcd_put_data(0b00001010);
	lcd_put_data(0b00011111);
	lcd_put_data(0b00011111);
	lcd_put_data(0b00001110);
	lcd_put_data(0b00000100);
	lcd_put_data(0b00000000);
	lcd_put_data(0b00000000);
	lcd_put_data(0b00000000);

	lcd_cmd(0x80);

	lcd_cmd(0x40+(1<<3));

	lcd_put_data(0b00001100);
	lcd_put_data(0b00011110);
	lcd_put_data(0b00001100);
	lcd_put_data(0b00011110);
	lcd_put_data(0b00001100);
	lcd_put_data(0b00001100);
	lcd_put_data(0b00010010);
	lcd_put_data(0b00011011);

	lcd_cmd(0x80);
}



#endif
