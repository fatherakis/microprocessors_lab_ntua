MVI A, FFH     ; Α = 1111 1111 to initialize LEDs as OFF
STA 3000H
LXI B,03E8H    ; Save 03E8H on BC registers to cause 1ms * BC delay on DELB call (aka 1sec).
MVI D,00H      ; D=0 initialize count / Timer value is saved on D

START:
LDA 2000H      ; Load dip switch state on accumulator
RLC            ; Left shift for MSB check / Move MSB to CY
JNC START      ; if MSB=0 (OFF) don't count and go to start for new MSB check
RRC            ; right rotation on A to restore dip switch state
ANI 0FH        ; 0FH → 0000 1111 /isolate 4 LSB to determin conunt limit
MOV E,A        ; Save x count limit value on E

INCREASE:
CALL MSB_CHECK ; Check MSB before each change
CALL DISPLAY
MOV A,E        ; Α ← Ε
CMP D          ; if A = D meaning it reached the count limit, Z = 1
JZ DECREASE    ; If Ζ =1 then start decrease timer process
INR D          ; else increment
JMP INCREASE   ; and continue

DECREASE:
DCR D          ; Decrement D
CALL MSB_CHECK
CALL DISPLAY
MVI A,00H      ; Α ← 0
CMP D          ; A = D only when timer is zero, Ζ=1
JZ START       ; if Ζ=1 start over LSB check in case they change value during counting
JMP DECREASE   ; else continue

DISPLAY:
MOV A,D        ; Α ←D Move timer value
CMA            ; LEDs work with inverted logic (συμπληρ. ως προς 1)
STA 3000H      ; save A data in output port
CALL DELB      ; add time delay
RET            ; return

MSB_CHECK:
LDA 2000H      ; load dip switch state on accumulator
RLC            ; left rotation for MSB check / move MSB to CY
JNC MSB_CHECK  ; if MSB=0 (OFF) don't allow count and move to START for new MSB check
RET            ; return

END
