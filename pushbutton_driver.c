#ifndef __PUSHBUTTON_DRIVER__
#define __PUSHBUTTON_DRIVER__

#include "pushbutton_driver.h"

void pushbutton_init()
{
	DDRG |= 0x1F;
}

#endif