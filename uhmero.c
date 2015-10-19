#include "lcd_driver.h"
#include "pushbutton_driver.h"
#include "led_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void init_periph(); // pefifériák inicializálása
void init_uh();
void timer0_init();
void impulse_start();
void impulse_stop();
void timer1_init();
void measure();

int main (void){


	init_periph();

	char test[]  = "teszt";
	lcd_put_string(test);
	
	timer0_init();
	impulse_start();
	impulse_stop();

	while(1){
	
	
	}	

	return;
}


void init_periph(){

	lcd_init();
	pushbutton_init();
	timer0_init();
	led_init();
	init_uh();

	sei(); // global interrupt enable


}

void timer0_init(){
	
	OCR0 = 159;
	TCCR0 = (1<<3); // CTC mode + no prescale
	TCCR0 |= 7;
	TIMSK |= (1<<1);
}

void impulse_start(){
	TCNT0 = 0;
	TCCR0 |= 1;
}

void impulse_stop(){
	TCCR0 ^= 1;
}


ISR(TIMER0_COMP_vect)
{
	
	impulse_stop();
	
	/*static int vmi = 1;
	
	if(vmi%2){
		led_out(0xff);
		vmi++;
	}
	else{
		led_out(0);
	}*/

}

void init_uh(){
	
	DDRD &= 0b11111110; // echo
	DDRD |= 0b00000010; // trigger
	
}

void measure(){

	impulse_start();

	

}

void timer1_init(){
	
	

}
