#ifndef __SEGMENT_DRIVER__
#define __SEGMENT_DRIVER__

#include <avr/io.h>

void segment_init();
void segment_display();
int  segment_get_int();
void segment_put_int(int data);

volatile unsigned char segment_data[4] = {0};

/*

------------
HOW IT WORKS
------------

ATTENTION: 	This header uses the Timer0 to handle the 7 segments on T-bird 2

segment_init()		-	Sets the data direction register to make it able to drive 7segment display

segment_display()	-	Display one array element on each function call 

segment_put_int()	-	Puts an integer (0 - 9999) to the data array

segment_get_int()	-	Returns the data array as integer


-----------
HOW TO USE
-----------

1.	-	Run segment_init() once
2.	-	Create an unsigned char array with 4 elements	( volatile unsigned char segment_data[4];	)
3.	- 	Put the desired integer into the array with segment_put_int() function
4.	-	Call the segment_display() function in every millisecond

DO NOT FORGET sei();


Example:


volatile unsigned char segment_data[4];

int main()
{
	// anything you want...
}


ISR(TIMER0_OVF_vect)
{
	segment_out();
}

*/



void segment_init()
{
	DDRA |= 0xFF;
}


void segment_display()
{
static unsigned char iterator,data_address;

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



#endif __SEGMENT_DRIVER__
