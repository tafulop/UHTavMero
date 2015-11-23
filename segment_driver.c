#include "segment_driver.h"


volatile unsigned char segment_data[4] = {0};


void segment_display()
{
static unsigned char iterator = 0, data_address = 3;

PORTA = 0;	// en | _ _ _ | data
PORTA |= 0x80|(iterator++<<4)|segment_data[data_address--]; 

	if(iterator >= 	4 || iterator < 1)
	{
		iterator = 0;
		data_address = 3;
	}
}



void segment_put_int(int data)
{
	if(data <= 9999 && data >= 0)
	{
		unsigned char temp;
		temp = (data/1000)%10;
		segment_data[0] = temp;		
		temp = (data/100)%10;
		segment_data[1] = temp;
		temp = (data/10)%10;
		segment_data[2] = temp;
		temp = data%10;
		segment_data[3] = temp;
	}
	else
	{
		return;
	}
}



int segment_get_int()
{
	int data = 0;
	
	data = 	segment_data[0]*1000;
	data += segment_data[1]*100;
	data += segment_data[2]*10;
	data += segment_data[3];
	
return data;
}
