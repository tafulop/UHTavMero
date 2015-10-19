#ifndef __PUSHBUTTON_DRIVER__
#define __PUSHBUTTON_DRIVER__

#include <avr/io.h>


void pushbutton_init();

void pushbutton_init()
{
	DDRG |= 0x1F;
}


#endif
