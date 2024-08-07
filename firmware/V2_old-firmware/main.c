#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "usart.h"
#include "display.h"

// define the PWM clock divider. The PWM frequency will be
// F_PWM = F_CPU / PWM_CLOCKDIV
// F_CPU has to be defined in the makefile, and make fuse
// has to be executed for changes of F_CPU to get effective
// Possible values for PWM_CLOCKDIV are
// 1024, 8192, 65536, 262144

//#define PWM_CLKDIV 1024     // presc 1
//#define PWM_CLKDIV 8192     // presc 8
#define PWM_CLKDIV 65536    // presc 64
//#define PWM_CLKDIV 262144   // presc 256


// PWM0 (output)
#define PWM0 PB1
#define PWM0_PORT PORTB
#define PWM0_DDR DDRB
#define PWM0_PIN PINB
#define SET_PWM0 (PWM0_PORT |= (1<<PB1))
#define CLEAR_PWM0 (PWM0_PORT &= ~(1<<PB1))

// PWM1 (output)
#define PWM1 PB2
#define PWM1_PORT PORTB
#define PWM1_DDR DDRB
#define PWM1_PIN PINB
#define SET_PWM1 (PWM1_PORT |= (1<<PB2))
#define CLEAR_PWM1 (PWM1_PORT &= ~(1<<PB2))

// OUT0 (output)
#define OUT0 PB6
#define OUT0_PORT PORTB
#define OUT0_DDR DDRB
#define OUT0_PIN PINB
#define SET_OUT0 (OUT0_PORT |= (1<<PB6))
#define CLEAR_OUT0 (OUT0_PORT &= ~(1<<PB6))

// OUT2 (output)
#define OUT2 PB7
#define OUT2_PORT PORTB
#define OUT2_DDR DDRB
#define OUT2_PIN PINB
#define SET_OUT2 (OUT2_PORT |= (1<<PB7))
#define CLEAR_OUT2 (OUT2_PORT &= ~(1<<PB7))

// ANALOG0 (input)
#define ANALOG0 PC0
#define ANALOG0_PORT PORTC
#define ANALOG0_DDR DDRC
#define ANALOG0_PIN PINC
#define IS_HIGH_ANALOG0 (ANALOG0_PIN & (1<<PC0))
#define SET_PULLUP_ANALOG0 (ANALOG0_PORT |= (1<<PC0))
#define CLEAR_PULLUP_ANALOG0 (ANALOG0_PORT &= ~(1<<PC0))

// ANALOG1 (input)
#define ANALOG1 PC1
#define ANALOG1_PORT PORTC
#define ANALOG1_DDR DDRC
#define ANALOG1_PIN PINC
#define IS_HIGH_ANALOG1 (ANALOG1_PIN & (1<<PC1))
#define SET_PULLUP_ANALOG1 (ANALOG1_PORT |= (1<<PC1))
#define CLEAR_PULLUP_ANALOG1 (ANALOG1_PORT &= ~(1<<PC1))

// LED1 (output)
#define LED1 PC2
#define LED1_PORT PORTC
#define LED1_DDR DDRC
#define LED1_PIN PINC
#define SET_LED1 (LED1_PORT |= (1<<PC2))
#define CLEAR_LED1 (LED1_PORT &= ~(1<<PC2))

// LED0 (output)
#define LED0 PC3
#define LED0_PORT PORTC
#define LED0_DDR DDRC
#define LED0_PIN PINC
#define SET_LED0 (LED0_PORT |= (1<<PC3))
#define CLEAR_LED0 (LED0_PORT &= ~(1<<PC3))

// SPEED1 (input)
#define SPEED1 PD2
#define SPEED1_PORT PORTD
#define SPEED1_DDR DDRD
#define SPEED1_PIN PIND
#define IS_HIGH_SPEED1 (SPEED1_PIN & (1<<PD2))
#define SET_PULLUP_SPEED1 (SPEED1_PORT |= (1<<PD2))
#define CLEAR_PULLUP_SPEED1 (SPEED1_PORT &= ~(1<<PD2))

// SPEED2 (input)
#define SPEED2 PD3
#define SPEED2_PORT PORTD
#define SPEED2_DDR DDRD
#define SPEED2_PIN PIND
#define IS_HIGH_SPEED2 (SPEED2_PIN & (1<<PD3))
#define SET_PULLUP_SPEED2 (SPEED2_PORT |= (1<<PD3))
#define CLEAR_PULLUP_SPEED2 (SPEED2_PORT &= ~(1<<PD3))

// SW1 (input)
#define SW1 PD6
#define SW1_PORT PORTD
#define SW1_DDR DDRD
#define SW1_PIN PIND
#define IS_HIGH_SW1 (SW1_PIN & (1<<PD6))
#define SET_PULLUP_SW1 (SW1_PORT |= (1<<PD6))
#define CLEAR_PULLUP_SW1 (SW1_PORT &= ~(1<<PD6))

// SW0 (input)
#define SW0 PD7
#define SW0_PORT PORTD
#define SW0_DDR DDRD
#define SW0_PIN PIND
#define IS_HIGH_SW0 (SW0_PIN & (1<<PD7))
#define SET_PULLUP_SW0 (SW0_PORT |= (1<<PD7))
#define CLEAR_PULLUP_SW0 (SW0_PORT &= ~(1<<PD7))


void io_init(){
  PWM0_DDR |= (1<<PB1);
  PWM0_PORT &= ~(1<<PB1);
  PWM1_DDR |= (1<<PB2);
  PWM1_PORT &= ~(1<<PB2);

  OUT0_DDR |= (1<<PB6);
  OUT0_PORT &= ~(1<<PB6);
  OUT2_DDR |= (1<<PB7);
  OUT2_PORT &= ~(1<<PB7);
  ANALOG0_DDR &= ~(1<<PC0);
  ANALOG1_DDR &= ~(1<<PC1);
  LED1_DDR |= (1<<PC2);
  LED1_PORT &= ~(1<<PC2);
  LED0_DDR |= (1<<PC3);
  LED0_PORT &= ~(1<<PC3);
  SPEED1_DDR &= ~(1<<PD2);
  SPEED2_DDR &= ~(1<<PD3);
  SW1_DDR &= ~(1<<PD6);
  SW0_DDR &= ~(1<<PD7);
}


uint16_t ReadChannel(uint8_t mux){
	uint8_t i;
	uint16_t result;

	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);    //frequPrescaler 8

	ADMUX = mux; //Kanal
	ADMUX |= (1<<REFS0);  //Vcc RefU
	ADMUX &= ~(1<<REFS1);  //Vcc RefU
	//ADMUX |= (1 << REFS0); //

	ADCSRA |= (1<<ADSC);   // eine ADC-Wandlung Dummy-Readout

	while ( ADCSRA & (1<<ADSC) ) {
		;     //warten bis fertig 
	}
	result = ADCW;
	// echte messung = mittelwert aus 4 messungen
	result = 0; 
	for( i=0; i<4; i++ ){
		ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
		while ( ADCSRA & (1<<ADSC) ) {
			;   // auf Abschluss der Konvertierung warten
		}
		result += ADCW;
		//result += (ADCL | (ADCH << 8));		    // Wandlungsergebnisse aufaddieren
	}
	ADCSRA &= ~(1<<ADEN);             // ADC deaktivieren 
	result /= 4;
	return result;
}

void enable_pwm(void){
  // fast pwm with 10 bit resolution non inverted
  // on OC1A and OC1B
  TCCR1A = (1<<WGM10) | (1<<WGM11) | (1<<COM1A1) | (1<<COM1B1);
#if PWM_CLKDIV == 1024
  TCCR1B = (1<<CS10) | (1<<WGM12);
#elif PWM_CLKDIV == 8192
  TCCR1B = (1<<CS11) | (1<<WGM12);
#elif PWM_CLKDIV == 65536
  TCCR1B = (1<<CS11) | (1<<CS10) | (1<<WGM12);
#elif PWM_CLKDIV == 262144
  TCCR1B = (1<<CS12) | (1<<WGM12);
#else
  #error invalid PWM_CLKDIV value
#endif
}

void disable_pwm(void){
  TCCR1A = 0;
  TCCR1B = 0;
}
void init_pwm(void){
  enable_pwm();
  
  // set default speed to 0
  OCR1A = 0;
  OCR1B = 0;
}

int main (void)
{
	//static FILE usart_stdout = FDEV_SETUP_STREAM( mputc, 0, _FDEV_SETUP_WRITE);
	//stdout = &usart_stdout;
	stdout = &display_stream;
  uart_init();


  DDRD = 0x0;
  PORTD = 0x0;
  _delay_ms(1);
  io_init();
  _delay_ms(1);
  DDRB = DDRB | (1<<1);
  PORTB = PORTB | (1<<1);
  init_pwm();

  printf("hello world.\n");

  
  //uint8_t an=0;



  uint8_t pwm_enabled = 1;

  SET_LED0;
  _delay_ms(1000);
  CLEAR_LED0;

  while(1){
    //    _delay_ms (500);
    //    PORTD = PORTD&(~(1<<5)); // D5 ausschalten
    //    PORTD = PORTD|(1<<6);  // D6 einschalten
    //    _delay_ms (500);
    //    PORTD = PORTD&(~(1<<6)); // D6 ausschalten
    //    PORTD = PORTD|(1<<5);  // D5 einschalten
    //

    //
    uint16_t analog;
    analog = ReadChannel(ANALOG1);
    #define POTI_LOW 84
    #define POTI_HIGH 919
    #define PWM_MAX 0x3ff
    
    uint16_t out;
    if(analog <= POTI_LOW){
      if(pwm_enabled){
        CLEAR_PWM0;
        CLEAR_PWM1;
        disable_pwm();
        pwm_enabled = 0;
      }
      out = 0;
      CLEAR_LED1;
    }else if(analog >= POTI_HIGH){
      if(pwm_enabled){
        SET_PWM0;
        SET_PWM1;
        disable_pwm();
        pwm_enabled = 0;
      }
      out = 0x3ff;
      SET_LED1;
    }else{
      if(!pwm_enabled){
        enable_pwm();
        pwm_enabled = 1;
      }
      uint32_t tmp = (uint32_t)((analog - POTI_LOW)) * PWM_MAX;

      out = tmp / (POTI_HIGH - POTI_LOW);
      CLEAR_LED1;
    }

    //printf("in: %d, out: %d\n\r", analog, out);

    OCR1A = out;
    OCR1B = out;
    //OCR1B = 0x3ff - analog;

    if(pwm_enabled){
      SET_LED0;
    }else{
      CLEAR_LED0;
    }

    //if (analog >= 1023/4) {
    //  SET_LED0;
    //}else{
    //  CLEAR_LED0;
    //}

    //if (analog >=900) {
    //  SET_LED1;
    //}else{
    //  CLEAR_LED1;
    //}

    //if(IS_HIGH_SW0){
    //  SET_LED1;
    //}else{
    //  CLEAR_LED1;
    //}

  }
}
