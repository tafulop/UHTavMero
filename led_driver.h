#ifndef __LED_DRIVER__
#define __LED_DRIVER__

#include <avr/io.h>

void led_init();
void led_out(unsigned char data);


/*

----------
HOW TO USE
----------

1.	-	Call the led_init() function
2.	-	Call the led_out() function to diplay the given data on the LEDs.

*/




void led_init()
{
	DDRB |= 0xF0;	// PORTB out
	DDRD |= 0xF0;	// PORTD out
}

void led_out(unsigned char data)
{
	PORTD &= 0x0F;
	PORTB &= 0x0F;
	
	PORTD |= (data & 0xF0);
	PORTB |= (data & 0x0F)<<4;
}


#endif	__LED_DRIVER__
