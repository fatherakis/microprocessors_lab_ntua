#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LCD_Dir DDRD   /* Define LCD data port direction */
#define LCD_Port PORTD /* Define LCD data port */
#define RS PD2		   /* Define Register Select pin */
#define EN PD3		   /* Define Enable signal pin */

unsigned char reg[2], memory[2], button;
// OC0 is connected to pin PB3

/* KEYBOARD FUNCTIONS */
unsigned char scan_row_sim(int r)
{ // r is for row 1,2,3,4 -> PC4,5,6,7
	char a = (1 << 3);
	a = a << r; // we initialized a with 1 and push it to the corresponding row (PC4 - PC7)
	PORTC = a;
	_delay_us(500);		// wait .5 ms before exiting
	return PINC & 0x0F; // Returns the button pressed on each row
}

unsigned char flip(unsigned char r)
{
	return ((r & 0x0F) << 4 | (r & 0xF0) >> 4); // flip MSB and LSB part
}

void scan_keypad_sim()
{ // scans the whole keyboard and keeps the values to reg(0) and reg(1)
	unsigned char r;
	r = scan_row_sim(1);
	reg[1] = flip(r); // reg(1) = MSB(first_row)
	r = scan_row_sim(2);
	reg[1] += r; // reg(1) = MSB(first_row)+LSB(second_row)
	r = scan_row_sim(3);
	reg[0] = flip(r); // reg(0) = MSB(third_row)
	r = scan_row_sim(4);
	reg[0] += r; // reg(0) = MSB(third_row)+LSB(fourth_row)
	PORTC = 0x00;
}

int scan_keypad_rising_edge_sim()
{

	scan_keypad_sim();
	unsigned char tmp[2];
	tmp[0] = reg[0];
	tmp[1] = reg[1]; // save temp values

	_delay_ms(0x15);

	scan_keypad_sim();
	reg[0] &= tmp[0]; // re-read keyboard and crossreference the temp values (avoid sparking)
	reg[1] &= tmp[1];

	tmp[0] = memory[0]; // previous keyboard state
	tmp[1] = memory[1];

	memory[0] = reg[0]; // save new state in ram
	memory[1] = reg[1];

	reg[0] &= ~tmp[0]; // fetches just pressed switches (since previous state)
	reg[1] &= ~tmp[1];

	return (reg[0] || reg[1]); // returns the switches that were just pressed and not the same as previous state
}

/* ASCII Correspondance:
PC4: 1, 2, 3, A  (PC0 -> PC3) r=1
PC5: 4, 5, 6, B  (PC0 -> PC3) r=2
PC6: 7, 8, 9, C  (PC0 -> PC3) r=3
PC7: *, 0, #, D  (PC0 -> PC3) r=4
*/

unsigned char keypad_to_ascii_sim()
{
	if (reg[0] & 0x01)
	{
		return '*';
	}
	if (reg[0] & 0x02)
	{
		return '0';
	}
	if (reg[0] & 0x04)
	{
		return '#';
	}
	if (reg[0] & 0x08)
	{
		return 'D';
	}
	if (reg[0] & 0x10)
	{
		return '7';
	}
	if (reg[0] & 0x20)
	{
		return '8';
	}
	if (reg[0] & 0x40)
	{
		return '9';
	}
	if (reg[0] & 0x80)
	{
		return 'C';
	}

	if (reg[1] & 0x01)
	{
		return '4';
	}
	if (reg[1] & 0x02)
	{
		return '5';
	}
	if (reg[1] & 0x04)
	{
		return '6';
	}
	if (reg[1] & 0x08)
	{
		return 'B';
	}
	if (reg[1] & 0x10)
	{
		return '1';
	}
	if (reg[1] & 0x20)
	{
		return '2';
	}
	if (reg[1] & 0x40)
	{
		return '3';
	}
	if (reg[1] & 0x80)
	{
		return 'A';
	}

	return 0;
}

/* LCD functions */
void LCD_Command(unsigned char cmnd)
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
	LCD_Port &= ~(1 << RS); /* RS=0, command reg. */
	LCD_Port |= (1 << EN);	/* Enable pulse */
	_delay_us(1);
	LCD_Port &= ~(1 << EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1 << EN);
	_delay_us(1);
	LCD_Port &= ~(1 << EN);
	_delay_ms(2);
}

unsigned char read_to_ascii(int a)
{
	if (a == 0)
	{
		return '0';
	}
	if (a == 1)
	{
		return '1';
	}
	if (a == 2)
	{
		return '2';
	}
	if (a == 3)
	{
		return '3';
	}
	if (a == 4)
	{
		return '4';
	}
	if (a == 5)
	{
		return '5';
	}
	if (a == 6)
	{
		return '6';
	}

	if (a == 7)
	{
		return '7';
	}
	if (a == 8)
	{
		return '8';
	}
	if (a == 9)
	{
		return '9';
	}
	return 0;
}

void LCD_Char(unsigned char data)
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);
	LCD_Port |= (1 << RS); /* RS=1, data reg. */
	LCD_Port |= (1 << EN);
	_delay_us(1);
	LCD_Port &= ~(1 << EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);
	LCD_Port |= (1 << EN);
	_delay_us(1);
	LCD_Port &= ~(1 << EN);
	_delay_ms(2);
}

void LCD_Init(void)
{					   /* LCD Initialize function */
	LCD_Dir = 0xFF;	   /* Make LCD port direction as o/p */
	_delay_ms(20);	   /* LCD Power ON delay always >15ms */
	LCD_Command(0x02); /* send for 4 bit initialization of LCD  */
	LCD_Command(0x28); /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0x0c); /* Display on cursor off*/
	LCD_Command(0x06); /* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01); /* Clear display screen*/
	_delay_ms(2);
}

void LCD_Clear()
{
	LCD_Command(0x01); /* Clear display */
	_delay_ms(2);
	LCD_Command(0x80); /* Cursor at home position */
}

/* LETS USE TIMER OF PREVIOUS EXCERSISE TO TRIGGER ADC CONVERSION */
void ADC_init(void)
{
	// Initialize ADC
	ADMUX = 0x40;															 // Initialize with Vref = 5V
	ADCSRA = (1 << ADEN | 1 << ADIE | 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // Enable ADC & Interrupt and make Div Factor 128
}

ISR(ADC_vect)
{
	volatile float Vin = (ADC * 5.0) / 1024.0;
	int calc = Vin * 100;
	int num1 = Vin / 1;			   // Y.xx
	int calc_points = calc % 100;  // x.YY
	int point1 = calc_points / 10; // x.Yx
	int point2 = calc_points % 10; // x.xY

	/* IRL DISPLAY */
	LCD_Command(0x01); // Clear Display (easyAVR Board needs to reset cursor too )
	// LCD_Clear();   //uncomment if flash on board
	LCD_Char('V');
	LCD_Char('o');
	LCD_Char('1');
	LCD_Char('\n'); /* Use the next line if flash on board */
	// LCD_Command(0xc0); //Force Cursor in New LINE
	LCD_Char(read_to_ascii(num1));
	LCD_Char('.');
	LCD_Char(read_to_ascii(point1));
	LCD_Char(read_to_ascii(point2));
	// LCD_Command(0x80);

	// ADC Interuption routine
}

ISR(TIMER1_OVF_vect)
{													  // Timer Interrupt Routine
	ADCSRA |= (1 << ADSC);							  // Start the next conversion
	TCNT1 = 63974;									  // Timer set to overflow in 200 msec  => FC F3 (65536-(n*7812.5)), n = 0.2 seconds
	TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); // Restart with CLK/1024
}

/* NEW EXERSISE CODE INITIALIZING TCNTR0 AS PWM */
void PWM_init()
{
	// set TMR0 in fast PWM mode with non-inverted output, prescale=8
	TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01);
}

int main()
{
	unsigned char duty;
	DDRB = 0xFF;   // PORTB is initialized as output (PWM usage)
	DDRC = 0xF0;   // PORTC 7-4 is output  3-0 is input  (Keyboard Usage)
	memory[0] = 0; // initialize "ram"
	memory[1] = 0;
	LCD_Init();
	PWM_init();
	duty = 127;

	ADC_init(); // Initialize ADC

	TIMSK = (1 << TOIE1);							  // Timer1 ,interrupt enable
	TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); // frequency of Timer1 8MHz/1024
	TCNT1 = 63974;									  // Timer set to overflow in 200 msec

	sei(); // enable interupts

	while (1)
	{
		// Forever, after reading button, change Duty Cycle accordingly NOTE: default state is half way so we know its working.
		// Also note while this routine is excecuted, every .2 sec ADC interrupts to Show voltage on LCD
		while (1)
		{
			if (scan_keypad_rising_edge_sim())
			{									// read button
				button = keypad_to_ascii_sim(); // On success: save and edit duty cycle
				break;
			}
		}
		if (button == '1')
		{ // If key pressed is 1, increase duty cycle
			if (duty < 255)
			{ // if duty cycle is at max then just stay there
				duty++;
			}
			OCR0 = duty;
			_delay_ms(8);
		}
		else if (button == '2')
		{ // If key pressed is 2, decrease duty cycle
			if (duty > 0)
			{ // if duty cycle is at bottom then stay there
				duty--;
			}
			OCR0 = duty;
			_delay_ms(8);
		}
	}
}
