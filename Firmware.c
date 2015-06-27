/*
Project: Measuring heart rate through fingertip
ATMEGA168 at 8.0 MHz internal clock
*/

//#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned short j, D0, D1, D2, D3, D4, D5;
unsigned short pulserate, pulsecount;
unsigned int i;

//--------------Function to Return mask for common anode 7-seg. display

unsigned short mask(unsigned short num)
{

	switch (num)
	{

		case 0 : return 0x40;
		case 1 : return 0x79;
		case 2 : return 0x24;
		case 3 : return 0x30;
		case 4 : return 0x19;
		case 5 : return 0x12;
		case 6 : return 0x02;
		case 7 : return 0x78;
		case 8 : return 0x00;
		case 9 : return 0x10;
		
	} //case end


}


void delay_debounce(void)
{
	_delay_ms(300);
}


void delay_refresh(void)
{
	_delay_ms(5);
}


void countpulse(void)
{

	PORTD |= (1<<2); //Turn on Tx
	delay_debounce();
	delay_debounce();
	TCNT0=0;
	_delay_ms(15000);; // Delay 15 Sec
	PORTD &= ~(1<<2); //Turn off Tx
	pulsecount = TCNT0;
	pulserate = pulsecount*4;


}


void display(void)
{

	D0 = pulserate%10;
	D1 = mask(D0);
	D2 = (pulserate/10)%10;
	D3 = mask(D2);
	D4 = pulserate/100;
	D5 = mask(D4);
	for (i = 0; i<=180*j; i++)
	{


		PORTC &= ~(1<<5);
		PORTC |= (1<<3);
		PORTC |= (1<<1);
		PORTB = D1;
		delay_refresh();
		PORTC |= (1<<5);
		PORTC &= ~(1<<3);
		PORTC |= (1<<1);
		PORTB = D3;
		delay_refresh();
		PORTC |= (1<<5);
		PORTC |= (1<<3);
		PORTC &= ~(1<<1);
		PORTB = D5;
		delay_refresh();
	}


	PORTC |= (1<<1);
}


int main(void)
{

	//CLKPR = 0x81; // Prescaler (2:1)
	PRR = 0x01; // Disable ADC
	TCCR0B = 0x01; // Clock for Counter

	DDRB = 0xFF; // All output (PB7 unused)
	PORTB = 0x7F; // Output is '0' for PB7
	DDRC = 0x3F; // PC6 i/p, rest output
	PORTC = 0x3F; //All o/p high by default
	DDRD = 0xEE; // PD4 & PD0 i/p, rest output
	PORTD = 0x10; // Internal pull-up enabled for PD4

	pulserate = 0;
	j =1;

	display();

	do
	{

		if((PIND & (1<<0)) == 0)
		{

			delay_debounce();
			countpulse();


			j= 3;
			display();

		}
	} while(1); // Infinite loop

}
