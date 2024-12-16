/*
 * Ex-3-Interrupts.c
 *
 * Created: 01/11/2021 10:01:21 PM
 * Author : PaterakisA
 */

#include <avr/io.h>
#include <avr/interrupt.h>
int i; //for loop
char A,B,C,D,E; //variables
ISR (INT0_vect){
	if(B == 0x00){ // Option 1: Show N LED's in line starting LSB
		D = 0x00; //Count LED's ON
		C = PINB;//Input
		for(i=0; i<8; i++){
			if ((C & 0x01) == 0x01 ) D = D + 0x01;  // For all bits increment D if each pin is 1
			C = C >> 1;								//Shift right for next pin
		}
		E = 0x01;									//Set Counter
		D = D - 0x01;								//Loop leads to +1 so decrease counter by one
		while(D > 0x00){
			E = E << 1;    //printed result
			E = E + 0x01;	//Add each bit after shifting
			D = D - 0x01;	//Decrement
		}
		PORTC = E;
	}
	if(B == 0x01){//Option 2: Show N for N LED's
		D = 0x00;  //Count LED's ON
		C = PINB;
		for(i=0; i<8; i++){
			if ((C & 0x01) == 0x01 ) D = D + 0x01;  // increment for each 1 in C
			C = C >> 1;
		}
		PORTC = D;									//Show count of LED's
	}
}
int main(void){
	DDRC = 0xff; //output in portC
	DDRD = 0x00; //Configure as input (Mostly for INT0 or 1)
	GICR = 0x40; //Enable external Interrupt INT0 (0100 0000)   INT1(1000 0000)
	MCUCR = 0x03;//Set INT0 as pose edge (See chart)
	sei();//Enable Interrupts
  while (1){
		A = PINA & 0x04;   //Check pinA input
		A = A >> 2;        //Move to Bit0 position
		if (A == 0x01) B = 0x01; //If PA2 is 1 then set B for LED counting
		else B = 0x00;    //If PA2 is 0 then reset B for lighting up LEDs
  }
}
