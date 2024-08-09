#include <avr/io.h>
#include "gpio.h"




void gpio_set_direction(gpio_t * gpio, gpio_direction_t dir)
{
	
	if(dir == GPIO_DIR_OUTPUT)
	{
		gpio->reg->ddr |= (1 << gpio->pin_num);
	}
	else if(dir == GPIO_DIR_INPUT)
	{
		gpio->reg->ddr &= ~(1 << gpio->pin_num);
	}
}
void gpio_set(gpio_t * gpio, gpio_state_t state)
{
	if (state == GPIO_HIGH)
	{
		gpio->reg->port |= (1 << gpio->pin_num);
	}
	else if (state == GPIO_LOW)
	{
		gpio->reg->port &= ~(1 << gpio->pin_num);
	}
	else if (state == GPIO_TOGGLE)
	{
		gpio->reg->port ^= (1 << gpio->pin_num);
	}
}
gpio_state_t gpio_read(gpio_t * gpio)
{
	gpio_state_t ret;
	if(gpio->reg->pin & (1 << gpio->pin_num))
	{
		ret = GPIO_HIGH;
	}
	else
	{
		ret = GPIO_LOW;
	}
	return ret;
}

uint8_t gpio_read_reg(gpio_t * gpio)
{
	return gpio->reg->pin;
};





