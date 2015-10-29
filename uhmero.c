#include "lcd_driver.h"
#include "pushbutton_driver.h"
#include "led_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void init_periph(); // pefifériák inicializálása
void timer0_init();
void impulse_start();
void impulse_stop();
void timer1_init();
void timer1_stop();

void timer2_init();
void timer2_stop();

void ext_int_init();
void calc_distance();
int *getCnt();

void set_uh_ports();

void measure_echo();
void measure_echo_stop();
unsigned int blabla = 0;

int main (void){

	

	init_periph();

	
	impulse_start();
	

	while(1){
		if(PING == 1){impulse_start();}
	}	

	return;
}


ISR(TIMER2_COMP_vect)
{	
	blabla++;
}

/*
ISR(TIMER2_OVF_vect){
	led_out(6);
}*/

void init_periph(){

	lcd_init();
	pushbutton_init();
	led_init();
	set_uh_ports();

	timer0_init();
	timer2_init();
	ext_int_init();
	
	sei(); // global interrupt enable

}

void timer0_init(){
	
	OCR0 = 159;
	TCCR0 = 0;
	TCCR0 |= (1<<3); // CTC mode 
	TIMSK |= (1<<1);	// interrupt enable
}

void timer1_init(){
	
	//static unsigned int cnt = 0;

	TCCR1B |= (1<<3);	// CTC mode, match on OCR1A
	TIMSK |= (1<<4);
	OCR1A = 7;
}

void set_uh_ports(){
	DDRD |= 1;	// trigger kimenet
	DDRD &= 0b11111101;	// echo bemenet
}

void ext_int_init(){
	EICRA |= (1<<2)|(1<<3);
	EIMSK |= 2;
}


void impulse_start(){
	//TCNT0 = 0;	// timer számláló regiszter nullázás
	TCCR0 |= 1;	// timer 0 indítás
//	PORTD |= 1; // trigger impulse 1
	led_out(1);
}


ISR(TIMER0_COMP_vect)
{	
	impulse_stop();	// 10 usec impulzus vége
	led_out(2);
	//measure_echo();	// mérés indítás
		
}

void impulse_stop(){
	//TCCR0 &= 0b11111110;
	TIMSK &= 0b11111101;	// timer 0 CTC interrupt disable
	PORTD &= 0xFE;	// trigger impulse 0
	//led_out(16);
}

void measure_echo(){
	
	TCCR2 |= 1;	// timer 2 indítás
	led_out(3);
	
}

ISR(TIMER1_COMPA_vect){

//	int* cnt = getCnt();
//	*cnt = *cnt + 1;		// cnt increment cnt++
//blabla++;
	//static int lofasz = 1;
//	led_out(0x55);

}

ISR(INT1_vect){
	
	static int run = 1;
	EIFR = 0;

	if(run%2){
		select_falling_edge();
		measure_echo();	
	}else{
		timer2_stop();
		select_rising_edge();
		calc_distance();	
	}

	run++;
	
}

void select_rising_edge(){
	EICRA |= (1<<2)|(1<<3);
}

void select_falling_edge(){
	EICRA = 0;
	EICRA |= (1<<3);
}



//	Ez a függvény tárolja azt a countert, amivel számoljuk az UH érzékelõ trigger és echo közötti idõtartamot.
//
//	static: 
//
//
int* getCnt(){	
	static unsigned int cnt = 0; // 16 bites
	return &cnt;
}


void timer1_stop(){
	TCCR1B &=  0xFE;	// clock source turned off
	//TIMSK = 0;
}

void calc_distance(){
	
	//int* cnt = getCnt();
	//int result = 0xff;//*cnt/58;
	//led_out(blabla);

	//led_out(6);
	print_result();
}

void timer2_init(){
	
	OCR2 = 7;
//	TCCR2 |= (1<<3); // CTC mode 
	TIMSK |= (1<<7);	// interrupt
}

void timer2_stop(){
	//TIMSK &= ~(1<<7);	// interrupt
	TIMSK = 0;
	//led_out(4);
}



void print_result(){
	
	lcd_goto_yx(0,0);
	lcd_put_int(blabla/58);

}
