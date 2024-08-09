#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "timer.h"

static volatile uint32_t timer_tick;

ISR(TIMER0_COMPA_vect)
{
	timer_tick++;
}



void timer_init(void)
{
	TCCR0A |= (1 << WGM01);
	OCR0A = 249U;
	TIFR0 |= (1 << OCR0A);
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= (1 << CS01) | (1 << CS00);
	

}

uint32_t timer_get_count(void)
{
	return timer_tick;
}