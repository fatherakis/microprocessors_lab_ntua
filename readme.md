## Microprocessors Lab - NTUA Class 2021-2022

This course was laboratory exclusive. In total there are 5 Lab exercises with their complete codes and questions.

Please note all codes contain explanatory comments and are fully completed and runnable.


### <div align="center"> Lab 1 - 8085 Assembly </div>


* ask1.asm: A seconds timer, showing time in binary form on output LEDs. 
    * Include enable bit from dip switch MSB and Timer setting from 4 switches (4 bit values).
    * Once the timer is finished, continue by counting up to the set value.


* ask2.asm: Read 2 hexadecimal numbers (X,Y) and show the 3-digit hex result of 16*x + y on display.

* ask3.asm: A wagon simulation represented from an LED row featuring right and left movement, start/stop bit control, direction control and automatic direction movement when reaching a corner (LSB or MSB LEDs).


### <div align="center"> Lab 2 - AVR Programming - Assembly and C </div>


* Ex-1-assembly.asm , .c : Calculate 2 given logic functions and output results on LEDs in assembly and in C.


* Ex-2-interrupts .asm: Interrupt routines and application on running counter program.


* Ex-3-interupt .c: Interrupt routines in C.
    * Note of sparking issue where if delays are too short, interrupt button may cause multiple unintended interrupts.


### <div align="center"> Lab 3 - AVR Programming </div>

* ex3-1.c: Electronic lock program. Continuous program with keypad support and ascii conversion waiting on a correct 2digit passcode, lighting LEDs accordingly.

* ex3-2.asm: Same program written in assembly with the addition of LCD Display support. Display corresponding messages on LCD Display along with the LEDs on correct or wrong passwords.


### <div align="center"> Lab 4 - AVR Programming</div>

This lab has one exercise split in 2 cases.

* lab4-1.asm: Program written in assembly were ADC data are read within the ADC conversion interrupt.

* lab4-2.c: Program written in C where we can choose to use polling or interrupts to read data from ADC with LCD support and definition being optional.

This program is an evolution of Lab 3 where we need to detect the CO presence in a room, trigger LEDs and show messages in the LCD Display. A CO sensor is connected to the ADC (Analog to Digital Converter) of the board which we can access through interrupts or polling. The threshold is set to 70ppm while also if the correct code is pressed (from Lab 3) then the Alarm turns off for 4 seconds.

### <div align="center"> Lab 5 - AVR Programming - C </div>

* lab5.c

A PWM waveform generation of 4kHz, utilizing the previous programs. Added support for duty cycle maniputlation from buttons and ADC voltage reading. Also added support for LCD Display, showing the ADC voltage readings with 2 decimals precision. These are all achieved with interrupts and multiple helper and support functions.


