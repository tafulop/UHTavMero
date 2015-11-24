// Definitions from m128def.inc

#define DDRD 	0x11
#define DDRB 	0x17

#define PORTD 	0x12
#define PORTB 	0x18

#define TCCR0 	0x33
#define OCR0	0x31

#define TIMSK	0x37

#define TCCR2	0x25
#define TCNT2	0x24
#define OCR2	0x23

#define DDRG	0x64
#define DDRA	0x1a
#define DDRD	0x11



// LED init asm function
.global asmLEDInit
asmLEDInit:
	
	in	r24, DDRD
	ori	r24, 0xF0	
	out	DDRD, r24	; felsõ 4 bit

	in	r24, DDRB	
	ori	r24, 0xF0	
	out	DDRB, r24	; alsó 4 bit

	ret

	
// Timer 0 init
.global asmTimer0Init
asmTimer0Init:
	ldi	r16, 0x9F
	out	OCR0, r16
	

	in	r16, TCCR0 	; r24 beolvas, hogy megtartsuk az értékét
	ori	r16, 0x08	; 0x08 || r24
	out	TCCR0, r16
	
	in	r16, TIMSK	; interruptot engedélyezzük úgy, hogy minden más marad a régi	
	ori	r16, 0x02	; 
	out	TIMSK, r16	; 
	
	ret


// Timer 2 init, hasonlo logika mint a timer 0
.global asmTimer2Init
asmTimer2Init:

	in	r16, TCCR2	
	ori	r16, 0x03	
 	out	TCCR2, r16	
	
	in	r16, TIMSK	; interrupt engedélyezés
   	ori	r16, 0x40	
	out	TIMSK, r16

	ret


// Összes gomb bemenet
.global asmPushButtonInit
asmPushButtonInit:

	ldi r16, DDRG
	ori r16, 0x1F
	sts DDRG, r16

	ret

// 7 segmens inicializálás
.global asmSegmentInit
asmSegmentInit:

	ldi	r24, 0xFF	; 255
	out	DDRA, r24	; 26

	ret

// UH modulnak portbeállítás
.global asmSetUHPorts
asmSetUHPorts:

	sbi	DDRD, 0	; TRIGGER kimenet

	cbi	DDRD, 1	; ECHO bemenet

	ret


