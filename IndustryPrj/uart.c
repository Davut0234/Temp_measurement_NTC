#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <stdint-gcc.h>
#include "uart.h"

static uint16_t baudtoreg(uint32_t baud, uint8_t x2)
{
	uint8_t multipler = 8;
	if(!x2) (multipler *= 2);
	
	return round(((F_CPU / (float)(multipler * baud)) -1));
}

static uint32_t regtobaud(uint16_t reg, uint8_t x2)
{
	uint8_t multipler = 8;
	if(!x2) (multipler *= 2);
	
	return F_CPU / (multipler * (reg + 1));
	
}
static void uart_put_char(char c)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}
void uart_init(uint32_t baudrate)
{
	uint16_t baud_reg, baud_reg2x;
	volatile float error, error2x;
	
	baud_reg = baudtoreg(baudrate, 0);
	baud_reg2x = baudtoreg(baudrate, 1);
	
	error = (((float)regtobaud(baud_reg, 0) / baudrate) - 1) * 100;
	error2x = (((float)regtobaud(baud_reg2x, 1) / baudrate) - 1) * 100;
	
	if(abs(error) <= abs(error2x))
	{
		UBRR0 =baud_reg;
	}
	else
	{
		UCSR0A |= (1 << U2X0);
		UBRR0 = baud_reg2x;
	}
	
	UCSR0B |= (1 << TXEN0);
	
}

void uart_write(char * str, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		uart_put_char(*str++);
		
	}
}




