#include "pushbutton_driver.h"

void pushbutton_init()
{
	DDRG |= 0x1F;
}

int readbuttons(){
	return PING;
}
