#include <avr/io.h>
#include "adc.h"

// ADC modülü için konfigürasyon fonksiyonu
void adc_init(void)
{
	// ADMUX Register'ý: 0x00 ayarlarý
	// REFS0 = 1 -> AVCC referans voltajý, ADC kanal 0
	// MUX4:0 = 0000 -> ADC kanal 0 seçili, 10-bit çözünürlük
	ADMUX = (1 << REFS0);  // AVCC'yi referans olarak kullan, ADC kanal 0 seçili

	// ADCSRA Register'ý: 0x07 ayarlarý
	// ADPS2:0 = 111 -> ADC Prescaler: 128 (Örnekleme hýzý için uygun)
	// ADEN = 1 -> ADC'yi etkinleþtir
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Prescaler 128, ADC enable
}

// ADC'den okuma fonksiyonu
uint16_t adc_read(uint8_t ch)
{
	// Kanal seçim iþlemi: Seçilen kanalý belirlemek için ADMUX'ü güncelle
	ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);  // Kanalý seç

	// ADC dönüþümünü baþlat
	ADCSRA |= (1 << ADSC);  // ADC dönüþümünü baþlat

	// Dönüþüm tamamlanana kadar bekle
	while (ADCSRA & (1 << ADSC));  // ADSC bitini kontrol et

	// ADC sonuç register'ýndan deðeri oku
	uint16_t result = ADC;  // ADC register'ýndan sonucu oku (ADCL ve ADCH okumasý birleþti)

	return result;
}
