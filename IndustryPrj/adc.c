#include <avr/io.h>
#include "adc.h"

// ADC mod�l� i�in konfig�rasyon fonksiyonu
void adc_init(void)
{
	// ADMUX Register'�: 0x00 ayarlar�
	// REFS0 = 1 -> AVCC referans voltaj�, ADC kanal 0
	// MUX4:0 = 0000 -> ADC kanal 0 se�ili, 10-bit ��z�n�rl�k
	ADMUX = (1 << REFS0);  // AVCC'yi referans olarak kullan, ADC kanal 0 se�ili

	// ADCSRA Register'�: 0x07 ayarlar�
	// ADPS2:0 = 111 -> ADC Prescaler: 128 (�rnekleme h�z� i�in uygun)
	// ADEN = 1 -> ADC'yi etkinle�tir
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler 128, ADC enable
}

// ADC'den okuma fonksiyonu
uint16_t adc_read(uint8_t ch)
{
	// Kanal se�im i�lemi: Se�ilen kanal� belirlemek i�in ADMUX'� g�ncelle
	ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);  // Kanal� se�

	// ADC d�n���m�n� ba�lat
	ADCSRA |= (1 << ADSC);  // ADC d�n���m�n� ba�lat

	// D�n���m tamamlanana kadar bekle
	while (ADCSRA & (1 << ADSC));  // ADSC bitini kontrol et

	// ADC sonu� register'�ndan de�eri oku
	uint16_t result = ADC;  // ADC register'�ndan sonucu oku (ADCL ve ADCH okumas� birle�ti)

	return result;
}
