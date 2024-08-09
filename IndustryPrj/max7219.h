#ifndef MAX7219_H_
#define MAX7219_H_




void max7219_init(gpio_t * cs_pin);
void max7219_write_number(uint8_t pos, uint32_t number);
void max7219_update(void);
void max7219_clear(uint8_t pos, uint8_t count);
void max7219_write_dot(uint8_t pos, uint8_t write);
void max7219_display_zero();

#endif /* MAX7219_H_ */
