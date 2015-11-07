#define F_CPU 16000000UL
#include <avr/io.h>

#define BAUD 9600					
#define MYUBRR F_CPU/16/BAUD-1		// aszinkron normál mód 173. oldal data sheet
#include <avr/interrupt.h>
#include <D:\Programozas\Headers\T-bird\7segment_delay.h>
#include <D:\Programozas\Headers\T-bird\tastatura.h>

int segment_show(int);
void segment_blink(void);

int main (void) 
{
// *********************************** PORTOK  ************************************ //

//DDRA=0xFF;				// 7 szegmens
DDRB=0xF0;				// Felsõ nibble LED
DDRC=0b11111000;		// 0-2 billmátrix olvas,  6-3 bill mátrix címzés, 7 RGB LED
DDRD=0b11111011;		// d2 - RX, d3 TX, felsõ nibble LED
DDRG=0;					// gombsor bemenet
DDRF = 0b00001110; 		// LCD init
DDRE = 0b11111100;		// LCD init, kék LED init, zöld init

unsigned char hour,minute;


while(1)
{
for(hour=12;hour>0;hour--)
	{
	for(minute=60;minute>0;minute--)
		{
		segment_show(hour*100+minute);		
		}
	}
if(!hour & !minute)break;
}

while(1)
	{
	PORTC = PORTC^0xFF;
	PORTE = PORTE^0xFF;
	_delay_ms(10);
	}


return 0;
}

