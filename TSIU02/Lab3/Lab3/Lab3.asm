/*
 * Lab3.asm
 *
 *  Created: 2019-11-27 08:37:07
 *   Author: vidsi889, sebgr273
 */ 

	.dseg
	.org	$100
TIME:	.byte	4
POS:	.byte	1

	.cseg

	.org	$0000
	jmp		START

	.org	INT0addr
	jmp		BCD

	.org	INT1addr
	jmp		MUX

START:
	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
INIT_IO:
	ldi		r16,$7F
	out		DDRA,r16
	ldi		r16,$03
	out		DDRB,r16
	clr		r16
	out		DDRD,r16
CLEAR_SRAM:
	sts		TIME,r16
	sts		TIME+1,r16
	sts		TIME+2,r16
	sts		TIME+3,r16
INIT_INTE:
	;Configure
	;Trig
	ldi		r16,(1<<ISC01) | (0<<ISC00) | (1<<ISC11) | (0<<ISC10)
	out		MCUCR,r16
	;Activate
	ldi		r16,(1<<INT0) | (1<<INT1)
	out		GICR,r16
	;Enable Interrupts Globally
	sei
WAIT_FOR_I:	;Wait for interrupt
	;rcall	BCD
	rjmp	WAIT_FOR_I

	
;Interrupt Routines
BCD:
	push	r16
	in		r16,SREG
	push	r16
	push	r17
	clr		r17
	ldi		XH,HIGH(TIME)
	ldi		XL,LOW(TIME)
BCD_NINE:
	ld		r16,X
	inc		r16
	cpi		r16,10
	brne	BCD_END
	clr		r16
	st		X+,r16
	inc		r17
BCD_FIVE:
	ld		r16,X
	inc		r16
	cpi		r16,6
	brne	BCD_END
	clr		r16
	st		X+,r16
	inc		r17
	cpi		r17,4
	brne	BCD_NINE
BCD_END:
	st		X,r16
	pop		r17
	pop		r16
	out		SREG,r16
	pop		r16
	reti

MUX:
	push	r16
	in		r16,SREG
	push	r16
	push	r17
	ldi		ZH,HIGH(BTAB*2)
	ldi		ZL,LOW(BTAB*2)
	ldi		XH,HIGH(TIME)
	ldi		XL,LOW(TIME)
	rcall	LOAD_POS
	rcall	GET_DIGIT
	inc		r17
	cpi		r17,4
	brne	RETURN_FROM_MUX
	clr		r17
RETURN_FROM_MUX:
	sts		POS,r17
	pop		r17
	pop		r16
	out		SREG,r16
	pop		r16
	reti
;Functions for MUX
LOAD_POS:
	lds		r17,POS
	ret
GET_DIGIT:
	push	ZL
	push	XL
	add		XL,r17
	ld		r16,X
	add		ZL,r16
	lpm		r16,Z
	pop		XL
	pop		ZL
SHOW_DIGIT:
	out		PORTB,r17	;Controll which display is used
	out		PORTA,r16	;Show the right number
	ret


	.org	$200
BTAB:	;Binary table for 7-segment display
	.db		$3F,$06,$5B,$4F,$66,$6D,$7D,$07,$7F,$67
	;		0	1	2	3	4	5	6	7	8	9



