#include <stdint.h>
#include <stdlib.h>
#include "gpio.h"
#include "spi.h"
#include "max7219.h"

#define MAX7219_DIG_COUNT (3)  // Limit to 3 digits

typedef enum
{
	MAX7219_REG_ADDR_NOP,
	MAX7219_REG_ADDR_DIG0,
	MAX7219_REG_ADDR_DIG1,
	MAX7219_REG_ADDR_DIG2,
	MAX7219_REG_ADDR_DIG3,
	MAX7219_REG_ADDR_DIG4,
	MAX7219_REG_ADDR_DIG5,
	MAX7219_REG_ADDR_DIG6,
	MAX7219_REG_ADDR_DIG7,
	MAX7219_REG_ADDR_DECODE_MODE,
	MAX7219_REG_ADDR_INTENSITY,
	MAX7219_REG_ADDR_SCAN_LIM,
	MAX7219_REG_ADDR_SHUTDOWN,
	MAX7219_REG_ADDR_TEST = 0X0F,
} max7219_address_t;

static uint8_t numbers[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
static uint8_t buffer[MAX7219_DIG_COUNT];
static uint8_t written_positions[MAX7219_DIG_COUNT] = {0};
static gpio_t *cs = NULL;

static void max7219_write(const max7219_address_t address, const uint8_t value)
{
	gpio_set(cs, GPIO_LOW);
	spi_transfer_byte(address);
	spi_transfer_byte(value);
	gpio_set(cs, GPIO_HIGH);
}

void max7219_write_number(uint8_t pos, uint32_t number)
{
	uint32_t temp = number;
	uint8_t digit_count = 0;
	uint32_t divider = 1;

	while ((temp != 0) && (pos + digit_count) < MAX7219_DIG_COUNT)
	{
		temp /= 10;
		buffer[digit_count + pos] = numbers[(number / divider) % 10];
		written_positions[digit_count + pos] = 1; // Mark the position as written
		digit_count++;
		divider *= 10;
	}

	// Ensure remaining digits are cleared if the number has less than 3 digits
	for (uint8_t i = digit_count; i < MAX7219_DIG_COUNT; i++) {
		buffer[pos + i] = numbers[0]; // Set to zero
		written_positions[pos + i] = 1; // Mark the position as written
	}
}

void max7219_init(gpio_t *cs_pin)
{
	cs = cs_pin;
	gpio_set_direction(cs, GPIO_DIR_OUTPUT);
	gpio_set(cs, GPIO_HIGH);

	max7219_write(MAX7219_REG_ADDR_DECODE_MODE, 0x00);
	max7219_write(MAX7219_REG_ADDR_INTENSITY, 0x0F);
	max7219_write(MAX7219_REG_ADDR_SCAN_LIM, 0x02); // Scan limit to first 3 digits
	max7219_write(MAX7219_REG_ADDR_SHUTDOWN, 0x01);
	max7219_write(MAX7219_REG_ADDR_TEST, 0x00);
}

void max7219_update(void)
{
	uint8_t i;
	for (i = 0; i < MAX7219_DIG_COUNT; i++)
	{
		max7219_write((max7219_address_t)(i + 1), buffer[i]);
	}
}

void max7219_clear(uint8_t pos, uint8_t count)
{
	uint8_t i;
	for (i = pos; i < (pos + count); i++)
	{
		buffer[i] &= 0x80;
		written_positions[i] = 0; // Mark the position as cleared
	}
}

void max7219_write_dot(uint8_t pos, uint8_t write)
{
	if (write)
	{
		buffer[pos] |= (1 << 7);
	}
	else
	{
		buffer[pos] &= ~(1 << 7);
	}
}

void max7219_display_zero()
{
	uint8_t i;
	for (i = 0; i < MAX7219_DIG_COUNT; i++)
	{
		if (written_positions[i] == 0)
		{
			buffer[i] = numbers[0]; // Set to zero only if not written by max7219_write_number
		}
	}
}
