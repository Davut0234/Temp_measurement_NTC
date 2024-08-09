#include <avr/io.h>
#include "spi.h"

#define SPI_DDR_PORT	DDRB
#define SPI_MOSI_PIN	(3)
#define SPI_MISO_PIN	(4)
#define SPI_SCK_PIN		(5)
#define SPI_CS_PIN		(2)

void spi_init(void)
{
	SPI_DDR_PORT |= (1 << SPI_MOSI_PIN);
	SPI_DDR_PORT |= (1 << SPI_SCK_PIN);
	SPI_DDR_PORT |= (1 << SPI_CS_PIN);
	SPI_DDR_PORT &= ~(1 << SPI_MISO_PIN);
	
	SPCR |= ((1 << SPR1) | (1 << SPR0));
	SPCR |= (1 << MSTR);
	SPCR |= (1 << SPE);
	
}

uint8_t spi_transfer_byte(const uint8_t value)
{
	SPDR = value;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}