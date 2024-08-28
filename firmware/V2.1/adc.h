#include <util/delay.h>
// Initialize adc - run once at startup
void ADC_Init(void);

//Atmega8:
//PC5 = ADC5 = Channel 5
//PC4 = ADC4 = Channel 4
//PC3 = ADC2 = Channel 3
//PC2 = ADC2 = Channel 2
//PC1 = ADC1 = Channel 1
//PC0 = ADC0 = Channel 0

//example: value = ReadChannel(5);
uint16_t ADC_ReadChannel(uint8_t mux);
//Vcc = 1023
//GND = 0
