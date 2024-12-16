;
; Ex-1-assembly.asm
;
; Created: 31/10/2021 6:29:39 PM
; Author : PaterakisA
;


.include "m16def.inc"

reset:	ldi r24,low(RAMEND)
		out SPL,r24
		ldi r24,high(RAMEND)
		out SPH,r24    			;setup stack pointer
		ser r24
		out DDRB,r24   			;initialize PORTB as output
		clr r24

main:	in r16,PINC
		andi r16,0x01 			;this is A (Bit 0 of Pin C)

		in r17,PINC
		andi r17,0x02
		LSR r17  				;this is B (Bit 1 of Pin C)

		in r18,PINC
		andi r18,0x04
		LSR r18
		LSR r18  				;this is C (Bit 2 of Pin C)

		in r19,PINC
		LSR r19
		LSR r19
		LSR r19  				;this is D (Bit 3 of Pin C)

		mov r20,r16
		com r20
		andi r20,0x01  			;r20 is Anot

		and r20,r17 			; Anot & B

		mov r21,r17
		com r21
		andi r21,0x01  			;r21 is Bnot

		and r21,r18
		and r21,r19  			;Bnot & C & D

		or r20,r21  			; (Anot & B) | (Bnot & C & D)

		com r20   				;Negative of previous aka its F0
		andi r20,0x01
		out PORTB,r20   		;F0

		mov r21,r16    			;copy A
		and r21,r18    			;A & C

		mov r22,r17 			;copy B
		or r22,r19  			; B | D

		and r21,r22  			;F1 = (A & C & B) | D
		LSL r21      			;F1 on 2nd LSB
		add r21,r20  			;F1 , F0 (we add them since they are on different bit positions they are going to merge)
		out PORTB,r21
		rjmp main
