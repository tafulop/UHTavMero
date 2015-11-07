#include "lcd_driver.h"
#include "pushbutton_driver.h"
#include "led_driver.h"
#include "segment_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void init_periph(); // pefif�ri�k inicializ�l�sa
void timer0_init();
void impulse_start();
void impulse_stop();
void timer2_init();
void timer2_stop();

void send_trigger();

void ext_int_init();
void calc_distance();
int *getCnt();


void timer1_input_capture_init();

void set_uh_ports();

void measure_echo();
void measure_echo_stop();
unsigned int blabla = 0;

unsigned int i1 =0;
unsigned int i2 =0;

int main (void){

	

	init_periph();
	
	timer1_input_capture_init();

	
	//impulse_start();
	
	

	while(1){
		if(PING == 1){
			send_trigger();
			while(PING != 0){}
		}	
	}	

	return;
}


ISR(TIMER2_COMP_vect)
{	
/*	if(blabla < 65000){
		blabla = blabla + 10;
	} else {
		lcd_put_string("Out of range.");
	}
	*/
	blabla++;
}


void init_periph(){

//	lcd_init();
	//pushbutton_init();
	led_init();
	set_uh_ports();
	segment_init();
	timer0_init();
	timer2_init();
	ext_int_init();
	
	sei(); // global interrupt enable

}

void timer0_init(){
	
	OCR0 = 159;	// 10 usec sz�les trigger jelhez
	TCCR0 |= (1<<3); // CTC mode hogy pontos legyen a trigger
	TIMSK |= (1<<1);	// interrupt enable
}

void set_uh_ports(){
	DDRD |= 1;	// trigger kimenet
	DDRD &= 0b11111101;	// echo bemenet
}

void ext_int_init(){
	select_rising_edge();
	EIMSK |= 2;
}



void send_trigger(){
	TCNT0 = 0;	// timer sz�ml�l� regiszter null�z�s
	TCCR0 |= 1;	// timer 0 ind�t�s
	PORTD |= 1;
	led_out(0);
}

ISR(TIMER0_COMP_vect)
{	
	PORTD &= 0b11111110;	// 10 usec impulzus v�ge, PORTD D0 ->0
	led_out(2);
	TCCR3B |= 3;	// elind�tjuk a Timer 3-at, ami m�ri az impulzus sz�less�g�t
	//measure_echo();	// m�r�s ind�t�s
		
}

void impulse_stop(){
	//TCCR0 &= 0b11111110;
	TIMSK &= 0b11111101;	// timer 0 CTC interrupt disable
//	PORTD &= 0xFE;	// trigger impulse 0
	//led_out(16);
}

void measure_echo(){
	
	TCCR2 |= 2;	// timer 2 ind�t�s
	//led_out(3);
	
}


ISR(INT1_vect){
	
	static unsigned int run = 1;
	EIFR = 0;


	if(run%2){
		measure_echo();	
		select_falling_edge();
		led_out(run++);
	}else{
		timer2_stop();
		select_falling_edge();
		led_out(run++);
		calc_distance();	
	}

	
}

void select_rising_edge(){
	EICRA |= (1<<2)|(1<<3);
}

void select_falling_edge(){
	EICRA = 0;
	EICRA |= (1<<3);
}



//	Ez a f�ggv�ny t�rolja azt a countert, amivel sz�moljuk az UH �rz�kel� trigger �s echo k�z�tti id�tartamot.
//
//	static: 
//
//
int* getCnt(){	
	static unsigned int cnt = 0; // 16 bites
	return &cnt;
}


void calc_distance(){
	
	//int* cnt = getCnt();
	//int result = 0xff;//*cnt/58;
	//led_out(blabla);

	//led_out(6);
	print_result();
}

void timer2_init(){
	
	
	TCCR2 |= 3; 
	TIMSK |= (1<<6);	// interrupt
}

void timer2_stop(){
	//TIMSK = 0;
	TCCR2 = 0;
	//led_out(4);
}


void print_result(){
	
	lcd_goto_yx(0,0);
	lcd_put_int(blabla);

}

void timer1_input_capture_init(){
	TCNT3 = 0;
	ETIMSK |= 1<<5;	// input capture interrupt enable
	TCCR3B |= (1<<7)|(1<<6);	// noise filter + rising edge + 64 prescale
}

ISR(TIMER3_CAPT_vect){
	
	static char i = 1;
	unsigned int res = 0;

	// ebbe fut bele el�sz�r, mert 1/2 marad�ka 1
	if(i%2){
		i1 = ICR3;
		TCCR3B &= ~(1<<6);	// falling edge
		i++;
		led_out(i1);
	// ebbe m�sodszor, mert 2/2 marad�ka 0		
	}else{
		TCCR3B |= 1<<6;		// rising edge
		i2 = ICR3;
		i = 1;
		res = i2-i1;
		led_out(res);	
		//res = res / 58;
		float test = res / 14.5;
		segment_put_int((int)test);
	}

	// ha egy m�r�s meg volt, akkor le�ll�tjuk a timert 
	if(i > 2)
	{
		TCCR3B &= 0b11111000; // clock source off
		TCNT3 = 0;
		i = 1;	// i be�ll�t�sa k�vetkez� m�r�shez.
	}

}


ISR(TIMER2_OVF_vect){
	segment_display();
}













