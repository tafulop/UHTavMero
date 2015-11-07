
// LED addresses

#define DDRD 0x11
#define DDRB 0x17

#define PORTD 0x12
#define PORTB 0x18



// LED init asm function
.global asmLEDInit
asmLEDInit:
	
	in	r24, DDRD
	ori	r24, 0xF0	
	out	DDRD, r24	;PORTD

	in	r24, DDRB	
	ori	r24, 0xF0	
	out	DDRB, r24	;PORTB

	ret

	


