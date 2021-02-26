/*
 * lab0.asm
 *
 *  Created: 2019-11-06 13:19:32
 *   Author: sebgr273, vidsi889
 */


 ; Lab0

start:
  .def	num = r20
	.def	numm= r22
	.def	key = r21

	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	call	init
	clr		num
	clr		numm
FOREVER:
	call	GET_KEY
LOOP:
	cpi		key,0
	breq	FOREVER
	out		PORTD,num
	out		PORTB,numm
	call	DELAY
	inc		num
	cpi		num,10
	brne	NOT_10
	clr num
	inc numm
	cpi numm, 10
	brne NOT_10
	clr numm
NOT_10:
	call	GET_KEY
	jmp		LOOP
GET_KEY:
	clr		key
	sbic	PINA,0
	dec		key
	ret
INIT:
	clr		r16
	out		DDRA,r16
	ldi		r16,$0F
	out		DDRB,r16
	out		DDRD,r16
	ret
DELAY:
	ldi		r18,3
D_3:
	ldi		r17,0
D_2:
	ldi		r16,0
D_1:
	dec		r16
	brne	D_1
	dec		r17
	brne	D_2
	dec		r18
	brne	D_3
	ret
