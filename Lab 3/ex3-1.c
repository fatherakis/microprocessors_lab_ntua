/*
 * ex3-1.c
 *
 * Created: 22/11/2021 9:37:31 AM
 * Author : PaterakisA
 */
#define F_CPU 8000000 //ATmega16 has freq of 16MHz
#include <avr/io.h>
#include <util/delay.h>

void Success (){
	PORTB = 0xFF;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	PORTB = 0x00;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
}

void Blink (){
	PORTB = 0xFF;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	PORTB = 0x00;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);	//1sec
	PORTB = 0xFF;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	PORTB = 0x00;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);	//2sec
	PORTB = 0xFF;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	PORTB = 0x00;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);	//3sec
	PORTB = 0xFF;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	PORTB = 0x00;
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);	//4sec
}

unsigned char reg[2], memory[2],num1,num2;

unsigned char scan_row_sim(int r){	//r is for row 1,2,3,4 -> PC4,5,6,7
	char a = (1 << 3);
	a = a << r;			//we initialized a with 1 and push it to the corresponding row (PC4 - PC7)
	PORTC = a;
	_delay_us(500);
	return PINC & 0x0F; //Returns the button pressed on each row
}

unsigned char flip (unsigned char r){
	return ((r & 0x0F) << 4 | (r & 0xF0) >> 4);
}

void scan_keypad_sim (){    //scans the whole keyboard and keeps the values to reg(0) and reg(1)
	unsigned char r;
	r = scan_row_sim(1);
	reg[1] = flip(r);             //reg(1) = MSB(first_row)
	r = scan_row_sim(2);
	reg[1] += r;      //reg(1) = MSB(first_row)+LSB(second_row)
	r = scan_row_sim(3);
	reg[0] = flip(r);				//reg(0) = MSB(third_row)
	r = scan_row_sim(4);
	reg[0] += r;		//reg(0) = MSB(third_row)+LSB(fourth_row)
	PORTC = 0x00;
}

int scan_keypad_rising_edge_sim(){

	scan_keypad_sim();
	unsigned char tmp[2];
	tmp[0]=reg[0];
	tmp[1]=reg[1];			//save temp values

	_delay_ms(0x15);

	scan_keypad_sim();
	reg[0] &= tmp[0];		//re-read keyboard and crossreference the temp values (avoid sparking)
	reg[1] &= tmp[1];

	tmp[0] = memory[0];		//previous keyboard state
	tmp[1] = memory[1];

	memory[0] = reg[0];		//save new state in ram
	memory[1] = reg[1];

	reg[0] &= ~tmp[0];		//fetches just pressed switches (since previous state)
	reg[1] &= ~tmp[1];

	return (reg[0]||reg[1]); //returns the switches that were just pressed and not the same as previous state
}

/* ASCII Correspondance:
PC4: 1, 2, 3, A  (PC0 -> PC3) r=1
PC5: 4, 5, 6, B  (PC0 -> PC3) r=2
PC6: 7, 8, 9, C  (PC0 -> PC3) r=3
PC7: *, 0, #, D  (PC0 -> PC3) r=4
*/

unsigned char keypad_to_ascii_sim(){
	if(reg[0] & 0x01){
		return '*';
	}
	if(reg[0] & 0x02){
		return '0';
	}
	if(reg[0] & 0x04){
		return '#';
	}
	if(reg[0] & 0x08){
		return 'D';
	}
	if(reg[0] & 0x10){
		return '7';
	}
	if(reg[0] & 0x20){
		return '8';
	}
	if(reg[0] & 0x40){
		return '9';
	}
	if(reg[0] & 0x80){
		return 'C';
	}

	if(reg[1] & 0x01){
		return '4';
	}
	if(reg[1] & 0x02){
		return '5';
	}
	if(reg[1] & 0x04){
		return '6';
	}
	if(reg[1] & 0x08){
		return 'B';
	}
	if(reg[1] & 0x10){
		return '1';
	}
	if(reg[1] & 0x20){
		return '2';
	}
	if(reg[1] & 0x40){
		return '3';
	}
	if(reg[1] & 0x80){
		return 'A';
	}

	return 0;
}

int main(void)
{
	DDRB = 0xFF; //PORTB is initialized as output
	DDRC = 0xF0; //PORTC 7-4 is output  3-0 is input
    while (1) {
		loop:
		memory[0] = 0;
		memory[1] = 0;
		PORTB = 0x00;
		while(1){
			if(scan_keypad_rising_edge_sim()){
				num1=keypad_to_ascii_sim();
				break;
			}
		}
		while(1){
			if(scan_keypad_rising_edge_sim()){
				num2=keypad_to_ascii_sim();
				//scan_keypad_rising_edge_sim(); //try removing
				break;
			}
		}
		if (num1 != '1' || num2!= '2' ) goto WRONG; // accept if pass is 12 for remote team 12 ASCII 0 - 9 = 48 - 57dec or 30-39hex
		Success();
		goto loop;
		WRONG:
		Blink();
    }
	return 0;
}
