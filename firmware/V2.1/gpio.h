#include <avr/io.h>


#define GPIO_PIN(bit, port, ddr, pin)  { bit, &port, &ddr, &pin }

typedef struct {
    uint8_t bit;
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    volatile uint8_t *pin;
} GPIO_Pin;

//create instance:
//GPIO_Pin relay = { PD3, &PORTD, &DDRD, &PIND };
//using macro:
//GPIO_Pin relay = GPIO_PIN(PD3, PORTD, DDRD, PIND);


void GPIO_Init(GPIO_Pin *gpio, uint8_t direction) {
    if (direction) {
        *(gpio->ddr) |= (1 << gpio->bit);  // Set as output
    } else {
        *(gpio->ddr) &= ~(1 << gpio->bit);  // Set as input
    }
}




void GPIO_Set(GPIO_Pin *gpio) {
    *(gpio->port) |= (1 << gpio->bit);
}

void GPIO_Clear(GPIO_Pin *gpio) {
    *(gpio->port) &= ~(1 << gpio->bit);
}

void GPIO_Toggle(GPIO_Pin *gpio) {
    *(gpio->port) ^= (1 << gpio->bit);
}

void GPIO_SetLevel(GPIO_Pin *gpio, uint8_t level) {
    if (level != 0)
        GPIO_Set(gpio);
    else
        GPIO_Clear(gpio);
}

uint8_t GPIO_Read(GPIO_Pin *gpio) {
    return (*(gpio->pin) & (1 << gpio->bit)) != 0;
}