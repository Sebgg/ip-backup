;
; Lab4.asm
;
; Created: 2019-12-07 16:22:10
; Author : sebgr273, vidsi889
;


; --- lab4spel.asm

	.equ	VMEM_SZ     = 5		; #rows on display
	.equ	AD_CHAN_X   = 0		; ADC0=PA0, PORTA bit 0 X-led
	.equ	AD_CHAN_Y   = 1		; ADC1=PA1, PORTA bit 1 Y-led
	.equ	GAME_SPEED  = 120	; inter-run delay (millisecs)
	.equ	PRESCALE    = 7		; AD-prescaler value
	.equ	BEEP_PITCH  = 50	; Victory beep pitch
	.equ	BEEP_LENGTH = 100	; Victory beep length
	
	; ---------------------------------------
	; --- Memory layout in SRAM
	.dseg
	.org	SRAM_START
POSX:	.byte	1	; Own position
POSY:	.byte 	1
TPOSX:	.byte	1	; Target position
TPOSY:	.byte	1
LINE:	.byte	1	; Current line	
VMEM:	.byte	VMEM_SZ ; Video MEMory
SEED:	.byte	1	; Seed for Random

	; ---------------------------------------
	; --- Macros for inc/dec-rementing
	; --- a byte in SRAM
	.macro	INCSRAM	; inc byte in SRAM
		lds	r16,@0
		inc	r16
		sts	@0,r16
	.endmacro

	.macro	DECSRAM	; dec byte in SRAM
		lds	r16,@0
		dec	r16
		sts	@0,r16
	.endmacro

	; ---------------------------------------
	; --- Code
	.cseg
	.org 	$0
	rjmp	START
	.org	INT0addr
	rjmp	MUX


START:
	; s?tt stackpekaren, DONE
	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	rcall	HW_INIT	;DONE
	rcall	WARM
RUN:
	rcall	JOYSTICK
	rcall	ERASE_VMEM
	rcall	UPDATE

;*** 	V?nta en stund s? inte spelet g?r f?r fort 	***
	ldi		r18,GAME_SPEED
	rcall	GAME_DELAY
;*** 	Avg?r om tr?ff				 	***
CHECK_HIT:
	lds		r16,POSX
	lds		r17,POSY
	lds		r18,TPOSX
	lds		r19,TPOSY
	cp		r16,r18		;check if same x
	brne	NO_HIT
	cp		r17,r19		;check if same y
	brne	NO_HIT	
	;ldi		r16,BEEP_LENGTH
	ldi		r18,BEEP_PITCH
	rcall	BEEP
	rcall	WARM
NO_HIT:
	rjmp	RUN

	; ---------------------------------------
	; --- Multiplex display
MUX:	

/*** 	skriv rutin som handhar multiplexningen och ***
*** 	utskriften till diodmatrisen. ?ka SEED.		***/
	push	r17
	push	r16
	in		r16,SREG
	push	r16	
	clr		r17
	out		PORTB,r17 ;ghosting exists funkade inte med NOPs
	;Increase seed 
	INCSRAM	SEED 
	rcall	GET_ROW
	pop		r16
	out		SREG,r16
	pop		r16
	pop		r17
	reti
GET_ROW:
	push	XL
	push	XH
	lds		r16,LINE
	ldi		XH,HIGH(VMEM)
	ldi		XL,LOW(VMEM)
	add		XL,r16
	adc		XH,r17
	ld		r17,X
	pop		XH
	pop		XL
SHOW_ROW:
	swap	r16			;0000 0xxx => 0xxx 0000
	out		PORTA,r16	;decide which row is used, POSY
	swap	r16			;0xxx 0000 => 0000 0xxx
	nop
	nop
	nop
	nop
	out		PORTB,r17	;decide which column is used, POSX (LED-bulb)
	inc		r16
	cpi		r16,5
	brne	END_SHOW
	clr		r16
END_SHOW:
	sts		LINE,r16
	ret

	; ---------------------------------------
	; --- JOYSTICK Sense stick and update POSX, POSY
	; --- Uses r16
JOYSTICK:	

;*** 	skriv kod som ?kar eller minskar POSX beroende 	***
;*** 	p? insignalen fr?n A/D-omvandlaren i X-led...	***
	ldi		r16,AD_CHAN_X
	rcall	INIT_AD
	rcall	MOVE_X
;*** 	...och samma f?r Y-led 				***
	ldi		r16,AD_CHAN_Y
	rcall	INIT_AD
	rcall	MOVE_Y
	rcall	JOY_LIM
	ret
INIT_AD:
	ldi		r17, (1<<ADLAR) | (1<<REFS0)
	add		r16, r17
	out		ADMUX,r16
	ldi		r16,(1<<ADEN)
	out		ADCSRA,r16
	rcall	GET_AD
	ret
	
; Check movement for breaking limits

GET_AD:
	sbi		ADCSRA,ADSC
WAIT:
	sbic	ADCSRA,ADSC
	rjmp	WAIT
	in		r16,ADCH
CONVERT_RESULT:
	andi	r16,$C0
	ret

MOVE_X:
	cpi		r16,$C0
	breq	MOVE_RIGHT
	cpi		r16,$00
	breq	MOVE_LEFT
	rjmp	MOVE_RET
MOVE_RIGHT:
	DECSRAM	POSX
	rjmp	MOVE_RET
MOVE_LEFT:
	INCSRAM POSX
	rjmp	MOVE_RET

MOVE_Y:
	cpi		r16,$C0
	breq	MOVE_UP
	cpi		r16,$00
	breq	MOVE_DOWN
	rjmp	MOVE_RET

MOVE_DOWN:
	DECSRAM	POSY
	rjmp	MOVE_RET
MOVE_UP:
	INCSRAM POSY
MOVE_RET:
	ret

JOY_LIM:
	rcall	LIMITS		; don't fall off world!
	ret

	; ---------------------------------------
	; --- LIMITS Limit POSX,POSY coordinates	
	; --- Uses r16,r17
LIMITS:
	lds		r16,POSX	; variable
	ldi		r17,7		; upper limit+1
	rcall	POS_LIM		; actual work
	sts		POSX,r16
	lds		r16,POSY	; variable
	ldi		r17,5		; upper limit+1
	rcall	POS_LIM		; actual work
	sts		POSY,r16
	ret

POS_LIM:
	ori		r16,0		; negative?
	brmi	POS_LESS	; POSX neg => add 1
	cp		r16,r17		; past edge
	brne	POS_OK
	subi	r16,2
POS_LESS:
	inc	r16	
POS_OK:
	ret

	; ---------------------------------------
	; --- UPDATE VMEM
	; --- with POSX/Y, TPOSX/Y
	; --- Uses r16, r17
UPDATE:	
	clr		ZH 
	ldi		ZL,LOW(POSX)
	call 	SETPOS
	clr		ZH
	ldi		ZL,LOW(TPOSX)
	call	SETPOS
	ret

	; --- SETPOS Set bit pattern of r16 into *Z
	; --- Uses r16, r17
	; --- 1st call Z points to POSX at entry and POSY at exit
	; --- 2nd call Z points to TPOSX at entry and TPOSY at exit
SETPOS:
	ld		r17,Z+  	; r17=POSX
	call	SETBIT		; r16=bitpattern for VMEM+POSY
	ld		r17,Z		; r17=POSY Z to POSY
	ldi		ZL,LOW(VMEM)
	add		ZL,r17		; *(VMEM+T/POSY) ZL=VMEM+0..4
	ld		r17,Z		; current line in VMEM
	or		r17,r16		; OR on place
	st		Z,r17		; put back into VMEM
	ret
	
	; --- SETBIT Set bit r17 on r16
	; --- Uses r16, r17
SETBIT:
	ldi		r16,$01		; bit to shift
SETBIT_LOOP:
	dec 	r17			
	brmi 	SETBIT_END	; til done
	lsl 	r16		; shift
	jmp 	SETBIT_LOOP
SETBIT_END:
	ret

	; ---------------------------------------
	; --- Hardware init
	; --- Uses r16
HW_INIT:

;check pointers for mux

/**** 	Konfigurera h?rdvara och MUX-avbrott enligt ***
*** 	ditt elektriska schema. Konfigurera 		***
*** 	flanktriggat avbrott p? INT0 (PD2).			****/
	;Initialize ports
	ldi		r16,$FF
	out		DDRB,r16 ;1111 1111
	ldi		r16,$70
	out		DDRA,r16 ;0111 0000
	clr		r16
	out		DDRD,r16 ;0000 0000

	;Initialize interrupt
	;Trig
	ldi		r16,(1<<ISC01) | (0<<ISC00) | (1<<ISC11) | (0<<ISC10)
	out		MCUCR,r16

	;Activate
	ldi		r16,(1<<INT0)
	out		GICR,r16
	sei			; display on
	ret

	; ---------------------------------------
	; --- WARM start. Set up a new game
WARM:

;*** 	S?tt startposition (POSX,POSY)=(0,2)		*** DONE
	rcall	INIT_PLAYER
	push	r0		
	push	r0		
	rcall	RANDOM		; RANDOM returns x,y on stack
	pop		r0
	sts		TPOSX,r0
	pop		r0
	sts		TPOSY,r0

;*** 	S?tt startposition (TPOSX,TPOSY)			***

	rcall	ERASE_VMEM
	ret

INIT_PLAYER:
	push	r16
	clr		r16
	sts		POSX,r16
	ldi		r16,2
	sts		POSY,r16
	pop		r16
	ret


	; ---------------------------------------
	; --- RANDOM generate TPOSX, TPOSY
	; --- in variables passed on stack.
	; --- Usage as:
	; ---	push r0 
	; ---	push r0 
	; ---	call RANDOM
	; ---	pop TPOSX 
	; ---	pop TPOSY
	; --- Uses r16
RANDOM:
	in		r16,SPH
	mov		ZH,r16
	in		r16,SPL
	mov		ZL,r16
;*** 	Anv?nd SEED f?r att ber?kna TPOSX		***
	lds		r16,SEED
	rcall	CALC_RAND
			; store TPOSX	2..6
	subi	r16,-2
	std		Z+3,r16
;*** 	Anv?nd SEED f?r att ber?kna TPOSY		***
	lds		r16,SEED
	rcall	CALC_RAND
			; store TPOSY   0..4
	std		Z+4,r16
	ret

	;Uses r16
CALC_RAND:
	andi	r16,7 ;xxxx xxxx ^ 0000 0111 = 0000 0xxx
	cpi		r16,5
	brlo	RET_RAND
	subi	r16,4
RET_RAND:
	ret

	; ---------------------------------------
	; --- Erase Videomemory bytes
	; --- Clears VMEM..VMEM+4
	
ERASE_VMEM:
;*** 	Radera videominnet						***
	;save X if pointing to somewhere
	push	XL 
	push	XH
	;save r16 and r17
	push	r16
	push	r17
	;load VMEM pointer to X
	ldi		XH,HIGH(VMEM)
	ldi		XL,LOW(VMEM)
	ldi		r17,5
	clr		r16
ERASE_LOOP:
	st		X+,r16
	dec		r17
	brne	ERASE_LOOP
END_ERASE_VMEM:
	pop		r17
	pop		r16
	pop		XH
	pop		XL
	ret

	; ---------------------------------------
	; --- BEEP(r16) r16 half cycles of BEEP-PITCH
BEEP:	
	cli
;*** skriv kod f?r ett ljud som ska markera tr?ff 	***
	rcall	CYCLE_BEEP
	dec		r18
	brne	BEEP
	sei
	ret

CYCLE_BEEP:
	sbi		PORTB,7
	rcall	DELAY
	cbi		PORTB,7
	rcall	DELAY
	ret

GAME_DELAY:
	rcall	DELAY
	dec		r18
	brne	GAME_DELAY
	ret

DELAY:
	ldi		r16,10 ; Decimal bas
delayYttreLoop:
	ldi		r17,$1f
delayInreLoop:
	dec		r17
	brne	delayInreLoop
	dec		r16
	brne	delayYttreLoop
	ret

