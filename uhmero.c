#include "pushbutton_driver.h"
#include "led_driver.h"
#include "segment_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "asm.h"

// pefif�ri�k inicializ�l�sa
void init_periph(); 
void timer3_input_capture_init();	// impulzus sz�less�g�nek m�r�s�hez

// M�r�s f�ggv�nyei
void send_trigger();



int main (void){


	init_periph();
	
	// gombra indul a m�r�s
	while(1){
		if(PING == 1){
			send_trigger();
			while(PING != 0){}
		}	
	}	

}

void init_periph(){

	
	asmPushButtonInit();	// m�r�s starthoz
	
	asmLEDInit();

	asmSetUHPorts();	// Echo �s Trigger be�ll�t�sa
	
	asmSegmentInit();	// 7 szegmens init
	
	asmTimer0Init();	// trigger jelhez 10usec impulzus
	asmTimer2Init();	// 7 seg drive

	timer3_input_capture_init();
	
	sei(); // global interrupt enable

}


// impulzus sz�less�g�nek m�r�se
void timer3_input_capture_init(){
	TCNT3 = 0;
	ETIMSK |= 1<<5;	// input capture interrupt enable
	TCCR3B |= (1<<7)|(1<<6);	// noise filter + rising edge
}

// trigger k�ld�se, le�ll�tani a timer 0 fogja
void send_trigger(){
	TCNT0 = 0;	// timer sz�ml�l� regiszter null�z�s
	TCCR0 |= 1;	// timer 0 ind�t�s
	PORTD |= 1;
	led_out(0);
}


// ez �ll�tja le a trigger impulzust
ISR(TIMER0_COMP_vect)
{	
	PORTD &= 0b11111110;	// 10 usec impulzus v�ge, PORTD D0 ->0
	led_out(2);	
	TCCR3B |= 3;		// elind�tjuk a Timer 3-at, ami m�ri az impulzus sz�less�g�t	
}

// Ide fut be, amint �rkezik v�lasz a modult�l (2x is ide fut egy m�r�s sor�n)
ISR(TIMER3_CAPT_vect){
	
	static char i = 1;
	unsigned int res = 0;
	static unsigned int i1 =0;
	static unsigned int i2 =0;

	// ebbe fut bele el�sz�r, mert 1/2 marad�ka 1
	if(i%2){
		i1 = ICR3;
		TCCR3B &= ~(1<<6);	// falling edge
		i++;
	// ebbe m�sodszor, mert 2/2 marad�ka 0		
	}else{
		TCCR3B |= 1<<6;		// rising edge
		i2 = ICR3;
		i = 1;
		res = i2-i1;
	 	// eredetileg 58-cal kell osztani, de mi 4usec pontoss�ggal m�r�nk, ez�rt 58/4=14.5-tel kell osztani
		segment_put_int((int)res/14.5);
	}

}

// 7seg meghajt�sa
ISR(TIMER2_OVF_vect){
	segment_display();
} 