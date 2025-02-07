# IndustryPrj

## Project Overview

This project demonstrates the implementation of a temperature measurement system using an NTC thermistor, displayed on a 3-digit seven-segment display controlled by the MAX7219 driver. The project utilizes UART communication to send temperature readings to a computer and is developed for the ATmega328P microcontroller.

## Features

- Temperature measurement using an NTC thermistor
- Display of temperature values on a 3-digit seven-segment display
- Data transfer to a computer using UART communication
- Modular code structure for ease of maintenance

## Components Used

- **Microcontroller:** ATmega328P (8-bit AVR MCU, 32KB Flash, 2KB SRAM)
- **Display Driver:** MAX7219
- **Temperature Sensor:** NTC Thermistor
- **Communication Protocol:** UART

## File Structure

```
.
├── Debug/                     # Build files
├── IndustryPrj.atsln           # Solution file
├── IndustryPrj.componentinfo.xml # Component info
├── IndustryPrj.cproj           # Project file
├── adc.c                       # ADC module source file
├── adc.h                       # ADC module header file
├── gpio.c                      # GPIO module source file
├── gpio.h                      # GPIO module header file
├── main.c                      # Main application code
├── max7219.c                   # MAX7219 driver source file
├── max7219.h                   # MAX7219 driver header file
├── spi.c                       # SPI communication source file
├── spi.h                       # SPI communication header file
├── timer.c                     # Timer configuration source file
├── timer.h                     # Timer configuration header file
├── uart.c                      # UART communication source file
├── uart.h                      # UART communication header file
```

## Setup Instructions

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```
2. Open `IndustryPrj.atsln` in your preferred IDE.
3. Build and flash the project onto the ATmega328P microcontroller.
4. Connect the UART to your PC and use a serial terminal to view temperature readings.

## How It Works

1. The ADC reads the voltage from the NTC thermistor.
2. The software calculates the temperature based on the thermistor characteristics.
3. The calculated temperature is displayed on the seven-segment display.
4. UART communication is used to transmit the readings to a connected PC.

## Dependencies

- Microchip Studio or similar IDE compatible with AVR MCUs
- UART serial terminal (e.g., PuTTY or Tera Term)

## Future Improvements

- Implement calibration for improved temperature accuracy
- Add support for multiple sensors
- Integrate real-time clock functionality

## Contact

Developed by Davut Beyazkaya

For any questions, please reach out via [your contact email, if desired].

---

