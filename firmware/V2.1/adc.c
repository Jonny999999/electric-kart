#include <avr/io.h>
#include "adc.h"

void ADC_Init(void) {
    // Set up ADC with a prescaler of 128 (assuming an 8MHz clock)
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADMUX = (1<<REFS0); // Set reference voltage to AVcc
}

uint16_t ADC_ReadChannel(uint8_t mux) {
    uint8_t i;
    uint16_t result = 0;

    // Set the ADC channel
    ADMUX = (ADMUX & 0xF0) | (mux & 0x0F); // Select ADC channel with safety mask

    // Perform a dummy conversion to stabilize the ADC
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1<<ADSC));

    // Average four measurements
    for(i = 0; i < 4; i++) {
        ADCSRA |= (1<<ADSC);
        while (ADCSRA & (1<<ADSC));
        result += ADCW;
    }

    // Return the average of the four measurements
    return result / 4;
}