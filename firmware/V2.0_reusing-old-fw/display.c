#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "display.h"
//#include "usart.h"

FILE display_stream;
uint8_t twi_wait_timeout(uint16_t milliseconds){
	uint16_t i, ms;
	i = 0;
  ms = 0;
	while(!(TWCR & (1<<TWINT))){
    _delay_us(5);
    if(i>=2000){
      i=0;
      ms++;
      if (ms>milliseconds){
        return 0;
      }
    }
    i++;
		// wait for interrupt
	}
	return 1;
}

uint8_t twi_start(){
	//do{
		//TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
		//_delay_us(200);
    _delay_us(10);
		TWBR = 20;
		TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
		twi_wait_timeout(5);
	//}while(TWSR!=0x08);
	if (TWSR != 0x08){
		TWCR = 0;
		return 0;
	}else{
		return 1;
	}
	//return twi_wait_timeout(5);
	//while(!(TWCR & (1<<TWINT)) || (TWSR != 0x08) ){
	//	// wait for interrupt
	//	// and right status code
	//}
}

uint8_t twi_send(uint8_t addr, uint8_t * data, uint8_t len){
  if (!twi_start()){
		TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
		return 0;
	}
  TWDR = (addr<<1) | 0;
  	// send address request:
	TWCR = (1<<TWINT) | (1<<TWEN);
	twi_wait_timeout(5);
  while(len){
    switch(TWSR){
      case 0x18:
        // SLA+W acked
        TWDR = *data;
        TWCR = (1<<TWINT) | (1<<TWEN);
        data++;
        len--;
        break;
      default:
        // cancel sending
        len = 0;
        break;
    }
    twi_wait_timeout(5);
  }
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

static int display_putchar(char c, FILE *stream){
  twi_send(42, &c, 1);
  return (int)c;
}
static int display_readchar(FILE *stream){
  return EOF;
}
void display_stream_init(){
	fdev_setup_stream(&display_stream, display_putchar, display_readchar, _FDEV_SETUP_RW);
}
