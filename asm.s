// Definitions from m128def.inc

#include <avr/io.h>

// LED init asm function
.global asmLEDInit
asmLEDInit:

	in	r24, _SFR_IO_ADDR(DDRD)
	ori	r24, 0xF0	
	out	_SFR_IO_ADDR(DDRD), r24	; fels� 4 bit

	in	r24, DDRB	
	ori	r24, 0xF0	
	out	DDRB, r24	; als� 4 bit

	ret

	
// Timer 0 init
.global asmTimer0Init
asmTimer0Init:
	ldi	r16, 0x9F
	out	_SFR_IO_ADDR(OCR0), r16
	

	in	r16, _SFR_IO_ADDR(TCCR0) 	; r24 beolvas, hogy megtartsuk az �rt�k�t
	ori	r16, 0x08	; 0x08 || r24
	out	_SFR_IO_ADDR(TCCR0), r16
	
	in	r16, _SFR_IO_ADDR(TIMSK)	; interruptot enged�lyezz�k �gy, hogy minden m�s marad a r�gi	
	ori	r16, 0x02	; 
	out	_SFR_IO_ADDR(TIMSK), r16	; 
	
	ret


// Timer 2 init, hasonlo logika mint a timer 0
.global asmTimer2Init
asmTimer2Init:

	in	r16, _SFR_IO_ADDR(TCCR2)	
	ori	r16, 0x03	
 	out	_SFR_IO_ADDR(TCCR2), r16	
	
	in	r16, _SFR_IO_ADDR(TIMSK)	; interrupt enged�lyez�s
   	ori	r16, 0x40	
	out	_SFR_IO_ADDR(TIMSK), r16

	ret


// �sszes gomb bemenet
.global asmPushButtonInit
asmPushButtonInit:

	ldi r16, _SFR_IO_ADDR(DDRG)
	ori r16, 0x1F
	sts _SFR_IO_ADDR(DDRG), r16

	ret

// 7 segmens inicializ�l�s
.global asmSegmentInit
asmSegmentInit:

	ldi	r24, 0xFF	; 255
	out	_SFR_IO_ADDR(DDRA), r24	; 26

	ret

// UH modulnak portbe�ll�t�s
.global asmSetUHPorts
asmSetUHPorts:

	sbi	_SFR_IO_ADDR(DDRD), 0	; TRIGGER kimenet

	cbi	_SFR_IO_ADDR(DDRD), 1	; ECHO bemenet

	ret


