/*
 * stopwatch.c
 *
 * Created: 12/12/2019 7:30:33 AM
 *  Author: Manar Mohamed Salah
 */ 


#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


unsigned char second1=0, second2=0, minute1=0, minute2=0, hour1=0, hour2=0;


void INT0_init(void)
{
	DDRD &= ~(1<<PA2);
	PORTD |= (1<<PA2);
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
}

void INT1_init(void)
{
	DDRD &= ~(1<<PA3);
	GICR |= (1<<INT1);
	MCUCR |= (1<<ISC11) | (1<<ISC10);
}

void INT2_init(void)
{
	DDRB &= ~(1<<PA2);
	PORTB |= (1<<PA2);
	GICR |= (1<<INT2);
	MCUCR &= ~(1<<ISC2);
}

ISR(INT0_vect)
{
	second1=0; second2=0;
	minute1=0; minute2=0;
	hour1=0; hour2=0;
}

ISR(INT1_vect)
{
	TCCR1B &= ~((1<<CS12) | (1<<CS10));
}

ISR(INT2_vect)
{
	TCCR1B |=  ((1<<CS12) | (1<<CS10));
}

void TIMER1_init_CTC(void)
{
	TCNT1=0;
	OCR1A=1000;
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
	TIMSK |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
	second1++;
}



int main(void)
{
	SREG  |= (1<<7);
	
	INT0_init();
	INT1_init();
	INT2_init();
	TIMER1_init_CTC();
	
	DDRA |= 0x3f;
	PORTA |= 0x3f;
	DDRC |= 0x0f;
	PORTC &= ~((1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3));
	
	while(1)
	
	{
		
		if(second1==10)
		{
			second1=0;
			second2++;
		}
		
		if(second2==6)
		{
			second2=0;
			minute1++;
		}
		
		if(minute1==10)
		{
			minute1=0;
			minute2++;
		}
		
		if(minute2==6)
		{
			minute2=0;
			hour1++;
		}
		
		if(hour1==10)
		{
			hour1=0;
			hour2++;
		}
		
		if(hour2==2 && hour1==4)
		{
			hour2=0;
			hour1=0;
		}
		
		
		
		PORTA=0x01;
		PORTC= (PORTC & 0xf0) | (second1 & 0x0f);
		
		_delay_ms(1);
		
		PORTA=0x02;
		PORTC= (PORTC & 0xf0) | (second2 & 0x0f);
		
		_delay_ms(1);
		
		PORTA=0x04;
		PORTC= (PORTC & 0xf0) | (minute1 & 0x0f);
		
		_delay_ms(1);
		
		PORTA=0x08;
		PORTC= (PORTC & 0xf0) | (minute2 & 0x0f);
		
		_delay_ms(1);
		
		PORTA=0x10;
		PORTC= (PORTC & 0xf0) | (hour1 & 0x0f);
		
		_delay_ms(1);
		
		PORTA=0x20;
		PORTC= (PORTC & 0xf0) | (hour2 & 0x0f);
		
		_delay_ms(1);
		
		
		
	}
}