IN 10H       ; deactivate memory protection
MVI A,10H    ; Α ← 10Η to zero out 7-segment display
STA 0900H    ; save to display (apply zeroing)
STA 0901H
STA 0902H
STA 0903H
STA 0904H
STA 0905H

START:
MVI B,10H    ; Β ← 10Η
CALL KIND    ; Α ← Χ / input first number from keyboard
RLC          ; each RLC rotation multiplies the number while moving it
RLC          ; to the next power of 2. So 4 rotations
RLC          ; increase the value by *16. The initial number is multiplied by
RLC          ; 16 / A ←16·Χ
MOV D,A      ; D ← 16·X
CALL KIND    ; A←y / input second number from keyboard
ADD D        ; A ← A+D = 16X+Y
MOV H,A      ; H ← 16X+Y

FIGURE_NUMBERS:
MVI B,00H    ; B ← 0 / Hundreds on B
MVI C,00H    ; C ← 0 / Tens on C
MVI L,00H    ; L ← 0 / Units on L
MOV A,H      ; Α ← Η = 16X+Y

HUNDREDS:
CPI 64H      ; if A < 100 then there are no hundreds CY = 1
JC TENS      ; if CY = 1 count tens
SUI 64H      ; continuous subtraction of 100 since Α ≥ 100, to count hundreds
INR B        ; with each subtraction, increment hundreds counter
JMP HUNDREDS ; repeat until A < 100

TENS:
CPI 0AH      ; if A < 10 there are no tens and CY = 1
JC ONES      ; if CY = 1 go to ones count
SUI 0AH      ; continuous subtraction of 10 since Α ≥ 10, to count tens
INR C        ; with each subtraction, increment tens counter
JMP TENS     ; repeat until A < 10

ONES:
CPI 01H      ; if A < 1 there are no ones and CY = 1
JC DISPLAY   ; if CY = 1 display results in hex
SUI 01H      ; continuous subtraction of 10 since 1<Α<10 to count ones
INR L        ; with each subtraction, increment ones counter
JMP ONES     ; repeat until A < 1


DISPLAY:
MOV A,B      ; Α←Β for hundreds
STA 0902H    ; display on left-most 7-segment digit
MOV A,C      ; Α←C for tens
STA 0901H    ; display on middle 7-segment digit
MOV A,L      ; Α←Β for ones
STA 0900H    ; display on right-most 7-segment digit
LXI D,0900H
CALL STDM    ; Take the 6 character message from DE address and save in RAM.
             ; The 1st character is shown in righ-most display digit.
CALL DCD     ; Take the character codes of digits saved in RAM
             ; and show them on display.
JMP START    ; Continuous function

END
