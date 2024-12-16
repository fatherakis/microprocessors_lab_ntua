/*
 * lab4.2.c
 *
 * Created: 12/12/2021 2:18:42 PM
 * Author : PaterakisA
 */

#define F_CPU 8000000 //ATmega16 has freq of 16MHz
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char reg[2], memory[2],pin,num1,num2,success_ongoing,gas_on=0,gas=0x00;

void Success (){
  success_ongoing = 1;  //flag set on Success entrance
  pin = 0x80;           //PB7 on
	PORTB = pin|gas;      //dont override GAS status
	_delay_ms(4000);      //wait for 4 seconds
  pin = 0x00;           //PB7 off
	PORTB = pin|gas;      //dont override GAS status
	_delay_ms(500);       //wait 0.5 seconds before exit
  success_ongoing = 0;  //flag reset on exit
}

void Blink (){
	pin = 0x80;
	PORTB = pin|gas;      //PB7 on without override
	_delay_ms(500);       //wait .5 sec to turn off
	pin = 0x00;
	PORTB = pin|gas;      //PB7 off without override
	_delay_ms(500);	      //1 sec total delay until now
	pin = 0x80;
	PORTB = pin|gas;      //PB7 on without override
	_delay_ms(500);       //wait .5 sec to turn off
	pin = 0x00;
	PORTB = pin|gas;      //PB7 off without override
	_delay_ms(500);	      //2 sec total delay until now
	pin = 0x80;
	PORTB = pin|gas;      //PB7 on without override
	_delay_ms(500);       //wait .5 sec to turn off
	pin = 0x00;
	PORTB = pin|gas;      //PB7 off without override
	_delay_ms(500);	      //3 sec total delay until now
	pin = 0x80;
	PORTB = pin|gas;      //PB7 on without override
	_delay_ms(500);       //wait .5 sec to turn off
	pin = 0x00;
	PORTB = pin|gas;      //PB7 off without override
	_delay_ms(500);	      //4 sec total delay until now
}

unsigned char scan_row_sim(int r){	//r is for row 1,2,3,4 -> PC4,5,6,7
	char a = (1 << 3);
	a = a << r;//we initialized a with 1 and push it to the corresponding row (PC4 - PC7)
	PORTC = a;
	_delay_us(500);  //wait .5 ms before exiting
	return PINC & 0x0F; //Returns the button pressed on each row
}

unsigned char flip (unsigned char r){
	return ((r & 0x0F) << 4 | (r & 0xF0) >> 4);    //flip MSB and LSB part
}

void scan_keypad_sim (){//scans the whole keyboard and keeps the values to reg(0) and reg(1)
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

int Cx = 0; // Concentration of CO in ppm

unsigned char Gas_State(void){
	if (Cx < 18) return 0x01; // if   0 <= CO < 18 ppm LEDS_PORTB -> 	X0000001
	if (Cx < 36) return 0x03; // if  18 <=  CO < 36 ppm LEDS_PORTB -> 	X0000011
	if (Cx < 46) return 0x07; // if  36 <=  CO < 46 ppm LEDS_PORTB -> 	X0000111
	if (Cx < 70) return 0x0F; // if  46 <=  CO < 70 ppm LEDS_PORTB -> 	X0001111
	if (Cx < 88) return 0x1F;//  if  70 <=  CO < 88 ppm LEDS_PORTB -> 	X0011111   *CRITICAL*
	if (Cx < 183) return 0x3F;// if 88 <=  CO < 283 ppm LEDS_PORTB -> 	X0111111
	return 0x7F; 				      // if CO >= 183 ppm LEDS_PORTB -> 		  	X1111111
}

void ADC_init(void){
  // Initialize ADC
	ADMUX = 0x40;  // Initialize with Vref = 5V
	ADCSRA = (1<<ADEN | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0 );//Enable ADC & Interrupt and make Div Factor 128
}

int calc_cx (void){
  // Calculate Cx where Vin  = (ADC*5)/1024 and Cx = (1/M) * (Vin - Vgas0)
	volatile float sensitivity = 129.0, Vgas0 = 0.1;
	volatile float Vin = (ADC*5.0)/1024.0; // Vin  = (ADC/5)/1024
	volatile float M = sensitivity * 0.0001; // Cx = (1/M) * (Vin - Vgas0)
	return (int)((1/M) * (Vin - Vgas0));
}

ISR (ADC_vect){
  // ADC Interuption routine
	Cx = calc_cx(); // Calculate Cx
	gas = Gas_State(); // Tell me which leds should i Turn on
	if (Cx > 70){//Blinking will occur every 2 interrupts thus every 200ms
    if(success_ongoing == 0){ //only if NOT correct code is in
		if (gas_on == 1){ //if gas level is ON
      PORTB = pin;    //Turn it off
      gas_on = 0;    //reset flag to show its OFF
    } // Leds OFF
		else{
      gas_on = 1;   //set flag to show ON
      PORTB = gas | pin; //Turn it ON
    } // Leds ON
	}
  else PORTB = gas|pin;// If Success is active, just show level
  }
	else if (Cx <= 70){ //If level NOT critical then show level
		PORTB = gas | pin; // show gas level
	}
}


ISR (TIMER1_OVF_vect){ //Timer Interrupt Routine

	ADCSRA |= (1<<ADSC);          // Start the next conversion
	TCNT1 = 64755;	//Timer set to overflow in 100 msec  => FC F3 (65536-(n*7812.5)), n = 0.1 seconds
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10); //Restart with CLK/1024
}

int main(void){
	DDRB = 0xFF; //PORTB is initialized as output
	DDRC = 0xF0; //PORTC 7-4 is output  3-0 is input

  ADC_init();				// Initialize ADC

	TIMSK = (1 << TOIE1);									//Timer1 ,interrupt enable
	TCCR1B = (1<<CS12) | (0<<CS11) | (1<<CS10);			//frequency of Timer1 8MHz/1024
	TCNT1 = 64755;						//Timer set to overflow in 100 msec

	sei();				// enable interupts

    while (1) {
		loop:
		memory[0] = 0;    //initialize "ram"
		memory[1] = 0;
		PORTB = 0x00;    //clear output
		while(1){
			if(scan_keypad_rising_edge_sim()){ // read number 1
				num1=keypad_to_ascii_sim();     // save
				break;
			}
		}
		while(1){
			if(scan_keypad_rising_edge_sim()){ // read number 2
				num2=keypad_to_ascii_sim();     // save
				break;
			}
		}
		if (num1 != '1' || num2!= '2' ) goto WRONG; // accept if pass is 12 for remote team 12
    Success();
		goto loop; //restart
		WRONG:
		Blink();
    }
	return 0;
}
