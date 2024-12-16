MVI A,FFH             ; Α = 1111 1111 to initialize LEDs as OFF
STA 3000H
LXI B,01F4H           ; save 01F4H on BC to cause BC * 1ms = 0.5sec delay through DELB call
MVI D,08H             ; D = 0000 1000 as initial wagon position / D has the position of the wagon

START:
CALL MSB_CHECK        ; check MSB before each change
LDA 2000H             ; Load dip switch state on accumutlator
ANI 01H               ; isolate LSB
MOV E,A               ; E ← A=LSB / E has the state of lsb to detect changes in wagon movement
                      ; independent of the intial state
CPI 01H               ; If Α (=LSB) = 1 → ΟΝ then CY = 1
JNZ LEFT_TO_RIGHT     ; if CY=a0=0 → OFF then move towards left, else towards right.

; Starting Routines

RIGHT_TO_LEFT:
MVI H,00H             ; Η←0 / H is used as a direction detection flag
                      ; if wagon moves left then H = 0, else H = 1 for right movement
CALL MSB_CHECK        ; check MSB if movement is allowed
CALL DISPLAY          ; show position on LEDs
MOV A,D               ; A ← D , load wagon position
CPI 80H               ; if Α = 80H (1000 0000), corner case, Z = 1
JZ CHANGE_DIR_CORNER  ; if Ζ=1 then change direction due to edge
RLC                   ; else rotate left to continue movement normaly
MOV D,A               ; D ←A / save new position
JMP LSB_CHECK         ; check LSB for possible wagon direction change
RET

LEFT_TO_RIGHT:
MVI H,01H              ; Η←1 wagon moves right
CALL MSB_CHECK         ; MSB check to continue or stop
CALL DISPLAY           ; Position display
MOV A,D                ; A ← D , load wagon position on A
CPI 01H                ; if Α = 80H (1000 0000), corner case, Z = 1
JZ CHANGE_DIR_CORNER   ; if Ζ=1 then change direction due to edge
RRC                    ; else rotate right to continue normally
MOV D,A                ; D ←A / save wagon position
JMP LSB_CHECK          ; check LSB for possible wagon direction change
RET

; MSB and LSB switch check routines

MSB_CHECK:
LDA 2000H               ; Load dip switch state on accumulator
RLC                     ; left rotation for MSB check/ move MSB to CY
JNC MSB_CHECK           ; if MSB=0 (OFF), unable to move, wait until MSB is 1
RET                     ; return

LSB_CHECK:
LDA 2000H                ; load dip switch state on accumulator
ANI 01H                  ; isolate LSB
CMP E                    ; E containts previous LSB value. If E = 1 there is no switch state change
                         ; and no direction change is needed.
MOV E,A                  ; Ε ←A / save new LSB value on Ε
JZ NO_CHANGE             ; If Ζ=1, don't change directions
JNZ CHANGE_DIR           ; If Ζ=0 Change direction
RET

; Direction control and verification routines

NO_CHANGE:
MOV A,H                   ; Α←Η / H is used as a direction detection flag
CPI 00H                   ; if Α=Η=0, wagon moves from right to left, Ζ=1
JZ RIGHT_TO_LEFT          ; if Ζ=1 then continue left movement
CPI 01H                   ; if Α=Η=1, wagon moves from left to right, Ζ=0
JZ LEFT_TO_RIGHT          ; if Ζ=1 then continue right movement
RET

CHANGE_DIR_CORNER:
CALL DELB                 ; insert 0.5sec delay due to edge position
MOV A,H                   ; Α ← Η
CPI 00H                   ; if Α=Η=0, wagon is moving right to left, Ζ=1
JZ LEFT_TO_RIGHT          ; if Ζ=1 then reverse movement and go towards right
CPI 01H                   ; if Α=Η=1, wagon is moving from left to right, Ζ=1
JZ RIGHT_TO_LEFT          ; if Ζ=1 then reverse movement and continue towards left
RET

CHANGE_DIR:
MOV A,H                   ; Α←Η / Η is used as a direction detection flag
CPI 00H                   ; if Α=Η=0,wagon is moving towards left, Ζ=1
JZ LEFT_TO_RIGHT          ; if Ζ=1 then reverse the movement towards right
CPI 01H                   ; if Α=Η=1,the wagon is moving from Left to Right, Ζ=1
JZ RIGHT_TO_LEFT          ; if Ζ=1 then revert movment and continue towards left
RET

; Display data in output port

DISPLAY:
MOV A,D                    ; Α ←D move D state on A
CMA                        ; LEDs work in inverted logic (συμπληρ. ως προς 1)
STA 3000H                  ; save A data on output port
CALL DELB                  ; insert 0.5sec delay
RET                        ; return
END
