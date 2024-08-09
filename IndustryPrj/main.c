#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "timer.h"
#include "gpio.h"
#include "spi.h"
#include "adc.h"
#include "uart.h"
#include "max7219.h"

#define ADC_MEAN_COUNT  (5)  // Örnekleme sayýsýný artýrdýk
#define TEMPERATURE_CH  (2)

#define ADC_MAX_VALUE   1023.0
#define MAX_VOLTAGE     4.6   // 4.6 V, ADC referans gerilimi

#define R_NTC_10K       120000.0  // Termistörün 25°C'deki direnç deðeri
#define R_MEASURED      120000.0   // 10k?'lik referans direncin ölçülen deðeri
#define BETA            3700.0   // Termistörün B sabiti
#define T0              298.15   // 25°C Kelvin cinsinden sýcaklýk

gpio_t red_led = {(gpio_regs_t *)&PINB, 0};
gpio_t max7219_cs = {(gpio_regs_t *)&PINB, 2};

uint16_t mean(uint16_t *values, uint8_t length)
{
	uint16_t min = values[0];
	uint16_t max = values[0];
	uint32_t sum = values[0];
	for (uint8_t i = 1; i < length; i++)
	{
		if (values[i] < min) min = values[i];
		if (values[i] > max) max = values[i];
		sum += values[i];
	}
	sum -= (min + max);
	return (sum / (length - 2));
}

double calculate_temperature(uint16_t adc_value)
{
	
	// ADC deðerini voltaja çevir
	double voltage = ((double)adc_value / ADC_MAX_VALUE) * MAX_VOLTAGE;
	
	// Termistörün direncini hesapla
	double R_NTC = R_MEASURED * ((MAX_VOLTAGE / voltage) - 1.0);
	
	// Kelvin cinsinden sýcaklýðý hesapla
	double Temp_K = (BETA * T0) / (BETA + T0 * log(R_NTC / R_NTC_10K));
	
	// Celsius cinsinden sýcaklýða dönüþtür
	double temperature_celsius = Temp_K - 273.15;


	return temperature_celsius;
}

void display_temperature(int16_t temperature)
{
	max7219_clear(0, 3);  // Clear the display before writing new values
	max7219_write_number(0, temperature);  // Display the temperature
	max7219_update();
}

int main(void)
{
	gpio_set_direction(&red_led, GPIO_DIR_OUTPUT);
	timer_init();
	adc_init();
	uart_init(9600);
	spi_init();
	max7219_init(&max7219_cs);
	sei();

	while (1)
	{
		static uint32_t led_time;

		if (TIMER_CHECK_COUNTER(led_time, SEC(1)))
		{
			gpio_set(&red_led, GPIO_TOGGLE);
			led_time = TIMER_COUNTER;
		}

		static uint32_t screen_time;
		if (TIMER_CHECK_COUNTER(screen_time, SEC(1)))
		{
			screen_time = TIMER_COUNTER;
			uint16_t adc_raw[ADC_MEAN_COUNT];
			uint8_t mean_count = sizeof(adc_raw) / sizeof(uint16_t);

			for (uint8_t i = 0; i < mean_count; i++)
			{
				adc_raw[i] = adc_read(TEMPERATURE_CH);
			}
			uint16_t adc_value = mean(adc_raw, mean_count);

			// Debugging
			char uart_buffer[32];
			uint8_t msg_len;
			msg_len = sprintf(uart_buffer, "Raw ADC: %u\r\n", adc_value);
			uart_write(uart_buffer, msg_len);
			
			double temperature = calculate_temperature(adc_value);

			// Convert temperature to a format suitable for a 3-digit 7-segment display
			int16_t temp_display = (int16_t)round(temperature);    // Round to nearest integer

			// Debugging for display value
			msg_len = sprintf(uart_buffer, "Temp Display: %d\r\n", temp_display);
			uart_write(uart_buffer, msg_len);

			if (temp_display < 0) temp_display = 0;  // Clamp to 0 to avoid negative temperatures

			display_temperature(temp_display);  // Display the temperature
			msg_len = sprintf(uart_buffer, "Temp: %d°C\r\n\n", temp_display);
			uart_write(uart_buffer, msg_len);
		}
	}
}
