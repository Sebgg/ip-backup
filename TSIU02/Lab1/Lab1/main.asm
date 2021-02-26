/*
 * lab2.asm
 *
 *  Created: 2019-11-13 11:47:08
 *   Author: sebgr273,vidsi889
*/ 
 	
	.equ	N=36
	.equ    PITCH=10
start:
	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	ldi		r16,$01 ;0000 0001 PA0
	out		DDRA,r16
INIT:
	ldi		ZH,HIGH(MESSAGE*2)
	ldi		ZL,LOW(MESSAGE*2)
MORSE_LOOP:
	ldi		r19,N*3
	rcall	NOBEEP	;NOBEEP(3N)
	rcall	GET_CHAR
	cpi		r18,$00	;subtract before lookup
	breq	INIT
	cpi		r18,$20
	breq	SPACE
	subi	r18,$41
	brmi	SPACE
	cpi		r18,$1A
	brsh	SPACE
	; A-Z
	rcall	LOOKUP
	rjmp	CREATE_BEEP
GET_CHAR:
	lpm		r18,Z+
	ret
LOOKUP:
	push	ZH
	push	ZL
	ldi		ZH,HIGH(BTAB*2)
	ldi		ZL,LOW(BTAB*2)
	add		ZL,r18
	lpm		r18,Z
	pop		ZL
	pop		ZH
	ret
GET_BIT:
	rol		r18
	ret
CREATE_BEEP:
	ldi		r20,$FF
	rcall	GET_BIT
	and		r20,r18
	breq	MORSE_LOOP
	brcs	DAT		;BEEP(3N)
DIT:
	ldi		r19,N
	rcall	BEEP
	rjmp	POST_BEEP
DAT:
	ldi		r19,N*3
	rcall	BEEP
POST_BEEP:
	ldi		r19,N
	rcall	NOBEEP		;NOBEEP(N)
	clc					;Clear before rotate
	rjmp	CREATE_BEEP
SPACE:
	ldi		r19,4*N
	rcall	NOBEEP
	rjmp	MORSE_LOOP
CYCLE:
	sbi		PORTA,0 
	rcall	DELAY
	cbi		PORTA,0
	rcall	DELAY
	ret
NOCYCLE:
	cbi		PORTA,0  
	rcall	DELAY
	cbi		PORTA,0
	rcall	DELAY
	ret
BEEP:
	rcall	CYCLE
	dec		r19
	brne	BEEP
	ret
NOBEEP:
	rcall	NOCYCLE
	dec		r19
	brne	NOBEEP
	ret
DELAY:
	ldi		r16,PITCH  ;Decimal bas
delayYttreLoop:
	ldi		r17,$1F
delayInreLoop:
	dec		r17
	brne	delayInreLoop
	dec		r16
	brne	delayYttreLoop
	ret

MESSAGE:
	.db		"DATORaTEKNIK?YES ",$00
	.org $200
BTAB:
	.db		$60,$88,$A8,$90,$40,$28,$D0,$08,$20,$78,$B0,$48,$E0,$A0,$F0,$68,$D8,$50,$10,$C0,$30,$18,$70,$98,$B8,$C8
