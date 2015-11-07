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


#endif
