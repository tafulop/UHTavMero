#include "pushbutton_driver.h"
#include "led_driver.h"
#include "segment_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "asm.h"

// pefifériák inicializálása
void init_periph(); 

void timer0_init();	// 10usec széles impulzus generálásához
void timer2_init();	// 7seg meghajtásához
void timer3_input_capture_init();	// impulzus szélességének méréséhez

void set_uh_ports();	// Trigger és echo lábak beállítása	

// Mérés függvényei
void send_trigger();
void calc_distance();



int main (void){


	init_periph();
	

	while(1){
		if(PING == 1){
			send_trigger();
			while(PING != 0){}
		}	
	}	

}

void init_periph(){

	pushbutton_init();	// mérés starthoz
	//led_init();		// debug infóhoz
	asmLEDInit();
	set_uh_ports();	// Echo és Trigger beállítása
	segment_init();	// 7 szegmens init
	timer0_init();	
	timer2_init();
	timer3_input_capture_init();
	
	sei(); // global interrupt enable

}

void set_uh_ports(){
	DDRD |= 1;	// trigger kimenet
	DDRD &= 0b11111101;	// echo bemenet
}

void timer0_init(){
	
	OCR0 = 159;	// 10 usec széles trigger jelhez
	TCCR0 |= (1<<3); // CTC mode hogy pontos legyen a trigger
	TIMSK |= (1<<1);	// interrupt enable
}

// Timer a 7seg meghajtásához
void timer2_init(){
	
	TCCR2 |= 3; 
	TIMSK |= (1<<6);	// interrupt
}

// impulzus szélességének mérése
void timer3_input_capture_init(){
	TCNT3 = 0;
	ETIMSK |= 1<<5;	// input capture interrupt enable
	TCCR3B |= (1<<7)|(1<<6);	// noise filter + rising edge + 64 prescale
}

// trigger küldése, leállítani a timer 0 fogja
void send_trigger(){
	TCNT0 = 0;	// timer számláló regiszter nullázás
	TCCR0 |= 1;	// timer 0 indítás
	PORTD |= 1;
	led_out(0);
}


// ez állítja le a trigger impulzust
ISR(TIMER0_COMP_vect)
{	
	PORTD &= 0b11111110;	// 10 usec impulzus vége, PORTD D0 ->0
	led_out(2);	
	TCCR3B |= 3;		// elindítjuk a Timer 3-at, ami méri az impulzus szélességét	
}

// Ide fut be, amint érkezik válasz a modultól (2x is ide fut egy mérés során)
ISR(TIMER3_CAPT_vect){
	
	static char i = 1;
	unsigned int res = 0;
	static unsigned int i1 =0;
	static unsigned int i2 =0;

	// ebbe fut bele elõször, mert 1/2 maradéka 1
	if(i%2){
		i1 = ICR3;
		TCCR3B &= ~(1<<6);	// falling edge
		i++;
	// ebbe másodszor, mert 2/2 maradéka 0		
	}else{
		TCCR3B |= 1<<6;		// rising edge
		i2 = ICR3;
		i = 1;
		res = i2-i1;
	 	// eredetileg 58-cal kell osztani, de mi 4usec pontossággal mérünk, ezért 58/4=14.5-tel kell osztani
		segment_put_int((int)res/14.5);
	}

	// ha egy mérés meg volt, akkor leállítjuk a timert 
	if(i > 2)
	{
		TCCR3B &= 0b11111000; // clock source off
		TCNT3 = 0;		// timert számláló regiszter nullázása
		i = 1;	// i beállítása következõ méréshez.
	}

}

// beállítja a Timer 3 Input Capture módját Rising Edge-re
void select_rising_edge(){
	EICRA |= (1<<2)|(1<<3);
}

// beállítja a Timer 3 Input Capture módját Rising Edge-re
void select_falling_edge(){
	EICRA = 0;
	EICRA |= (1<<3);
}


// 7seg meghajtása
ISR(TIMER2_OVF_vect){
	segment_display();
} 





