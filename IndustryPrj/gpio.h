#ifndef GPIO_H_
#define GPIO_H_

typedef struct
{
	uint8_t pin;
	uint8_t ddr;
	uint8_t port;
}gpio_regs_t;

typedef struct
{
	gpio_regs_t * reg;
	uint8_t pin_num;
}gpio_t;


typedef enum
{
	GPIO_DIR_OUTPUT,
	GPIO_DIR_INPUT,
}gpio_direction_t;


typedef enum
{
	GPIO_LOW,
	GPIO_HIGH,
	GPIO_TOGGLE,
}gpio_state_t;

void gpio_set_direction(gpio_t * gpio, gpio_direction_t dir);
void gpio_set(gpio_t * gpio, gpio_state_t state);
gpio_state_t gpio_read(gpio_t * gpio);
uint8_t gpio_read_reg(gpio_t * gpio);

#endif /* GPIO_H_ */