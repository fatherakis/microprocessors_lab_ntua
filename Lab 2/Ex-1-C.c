/*
 * Ex-1.c
 *
 * Created: 31/10/2021 6:27:35 PM
 * Author : PaterakisA
 */
#include <avr/io.h>
char A, B, C, D, F0, F1;
int main(void)
{
	DDRB=0xFF; // Set PORTB as output
	DDRC=0x00; // Set PORTC as input
	while(1)
	{
		A = PINC & 0x01; // take 1st bit of pinC
		B = PINC & 0x02; // isolate 2st bit of pinC
		B = B >> 1;      // shift to bit 0 position
		C = PINC & 0x04; // take 3st bit of pinC
		C = C >> 2;      // shift to bit 0 position
		D = PINC & 0x08; // take 4st bit of pinC
		D = D >> 3;      // shift to bit 0 position
		F0 = ~( (~A & B) |( ~B & C & D) ) & 0x01; // F0 isolate bit 0 from result
		F1 = (A & C) & ( B | D) ; // F1
		F1 = F1 << 1; // shift F1 to bit 1 position
		PORTB = (F1+F0); //output F1+F0 since they are in different positions they are merging
	}
	return 0;
}
