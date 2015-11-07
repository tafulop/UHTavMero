#include "led_driver.h"

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
