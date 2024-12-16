;
; Ex-2-interrupts.asm
;
; Created: 01/11/2021 12:08:02 AM
; Author : PaterakisA
;


; Replace with your application code

.org 0x0
rjmp reset
.org 0x4 ;INT0 address is 0x02 INT1 address is 0x04
rjmp ISR1
clr r28 ;interrupt counter
clr r30 ;counter save point
reset:	ldi r24,(1 <<ISC11)|(1<<ISC10)  ;See chart: We setup interrupt for pose edge clock edge
		out MCUCR,r24					;Set choosen preferances for interrupt on MCUCR register
		ldi r24,(1<<INT1)				;Choose interrupt type (INT0 = PINB2 INT1 = PINB3)
		out GICR,r24					;Set interrupt type
		sei								;Enable interrupts
		ser r26
		out DDRC,r26					;Prepare C for output
		clr r26
		mov r26,r30						;load counter state

loop:	out PORTC,r26       ;output count
		ldi r24,low(100)      ;
		ldi r25,high(100)     ; Load r25,r24 registers for 100ms delay
		;rcall wait_msec      ; which we disable for simulation
		inc r26               ; count up
		mov r30,r26						;save counter state
		rjmp loop

ISR1:	push r26        ;save register contents
		in r26,PINA       ;read PINA value
		cpi r26,0xC0      ;check for PINA7 and PINA6
		brne dontcount		;if Z=1 (PINA7 & PINA6 = 1 (0xc0)) count interrupt else exit
		inc r28           ;count interrupt
		out PORTB,r28     ;output on PORTB
		ldi r24,low(998)  ;
		ldi r25,high(998) ; Load r25,r24 registers for 998ms delay
		;rcall wait_msec  ; which we disable for simulation
dontcount:	pop r26   ;recover register contents
		reti
