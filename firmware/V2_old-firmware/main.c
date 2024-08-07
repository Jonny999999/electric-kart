#include <avr/io.h>
#include <util/delay.h>

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

// MOSFET (output) MOSFET
#define MOSFET PB2
#define MOSFET_PORT PORTB
#define MOSFET_DDR DDRB
#define MOSFET_PIN PINB
#define SET_MOSFET (MOSFET_PORT |= (1<<PB2))
#define CLEAR_MOSFET (MOSFET_PORT &= ~(1<<PB2))

// RELAY (output) RELAY
#define RELAY PB7
#define RELAY_PORT PORTB
#define RELAY_DDR DDRB
#define RELAY_PIN PINB
#define SET_RELAY (RELAY_PORT |= (1<<PB7))
#define CLEAR_RELAY (RELAY_PORT &= ~(1<<PB7))

// ANALOG0 (input) - reserve
#define ANALOG0 PC0
#define ANALOG0_PORT PORTC
#define ANALOG0_DDR DDRC
#define ANALOG0_PIN PINC
#define IS_HIGH_ANALOG0 (ANALOG0_PIN & (1<<PC0))
#define SET_PULLUP_ANALOG0 (ANALOG0_PORT |= (1<<PC0))
#define CLEAR_PULLUP_ANALOG0 (ANALOG0_PORT &= ~(1<<PC0))

// ANALOG_GAS_PEDAL (input)
#define ANALOG_GAS_PEDAL PC2
#define ANALOG_GAS_PEDAL_PORT PORTC
#define ANALOG_GAS_PEDAL_DDR DDRC
#define ANALOG_GAS_PEDAL_PIN PINC
#define IS_HIGH_ANALOG_GAS_PEDAL (ANALOG_GAS_PEDAL_PIN & (1<<PC2))
#define SET_PULLUP_ANALOG_GAS_PEDAL (ANALOG_GAS_PEDAL_PORT |= (1<<PC2))
#define CLEAR_PULLUP_ANALOG_GAS_PEDAL (ANALOG_GAS_PEDAL_PORT &= ~(1<<PC2))

// Note: ANALOG batt measurement not added yet

// LED_ONBOARD (output) onbard
#define LED_ONBOARD PC3
#define LED_ONBOARD_PORT PORTC
#define LED_ONBOARD_DDR DDRC
#define LED_ONBOARD_PIN PINC
#define SET_LED_ONBOARD (LED_ONBOARD_PORT |= (1<<PC3))
#define CLEAR_LED_ONBOARD (LED_ONBOARD_PORT &= ~(1<<PC3))

// LED_EXTERNAL (output) external
#define LED_EXTERNAL PC5
#define LED_EXTERNAL_PORT PORTC
#define LED_EXTERNAL_DDR DDRC
#define LED_EXTERNAL_PIN PINC
#define SET_LED_EXTERNAL (LED_EXTERNAL_PORT |= (1<<PC5))
#define CLEAR_LED_EXTERNAL (LED_EXTERNAL_PORT &= ~(1<<PC5))

// BUZZER (output
#define BUZZER PC4
#define BUZZER_PORT PORTC
#define BUZZER_DDR DDRC
#define BUZZER_PIN PINC
#define SET_BUZZER (LED_EXTERNAL_PORT |= (1<<PC4))
#define CLEAR_BUZZER (LED_EXTERNAL_PORT &= ~(1<<PC4))

// SW1 (input)
#define SW1 PD2
#define SW1_PORT PORTD
#define SW1_DDR DDRD
#define SW1_PIN PIND
#define IS_HIGH_SW1 (SW1_PIN & (1<<PD2))
#define SET_PULLUP_SW1 (SW1_PORT |= (1<<PD2))
#define CLEAR_PULLUP_SW1 (SW1_PORT &= ~(1<<PD2))

// SW0 (input)
#define SW0 PD3
#define SW0_PORT PORTD
#define SW0_DDR DDRD
#define SW0_PIN PIND
#define IS_HIGH_SW0 (SW0_PIN & (1<<PD3))
#define SET_PULLUP_SW0 (SW0_PORT |= (1<<PD3))
#define CLEAR_PULLUP_SW0 (SW0_PORT &= ~(1<<PD3))


void io_init(){
  PWM0_DDR |= (1<<PB1);
  PWM0_PORT &= ~(1<<PB1);

  MOSFET_DDR |= (1<<PB2);
  MOSFET_PORT &= ~(1<<PB2);
  RELAY_DDR |= (1<<PB7);
  RELAY_PORT &= ~(1<<PB7);
  ANALOG0_DDR &= ~(1<<PC0);
  ANALOG_GAS_PEDAL_DDR &= ~(1<<PC2);
  LED_ONBOARD_DDR |= (1<<PC3);
  LED_ONBOARD_PORT &= ~(1<<PC3);
  LED_EXTERNAL_DDR |= (1<<PC5);
  LED_EXTERNAL_PORT &= ~(1<<PC5);
  BUZZER_DDR |= (1<<PC4);
  BUZZER_PORT &= ~(1<<PC4);
  SW1_DDR &= ~(1<<PD2);
  SW0_DDR &= ~(1<<PD3);
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

  DDRD = 0x0;
  PORTD = 0x0;
  _delay_ms(1);
  io_init();
  _delay_ms(1);
  DDRB = DDRB | (1<<1);
  PORTB = PORTB | (1<<1);
  init_pwm();




  uint8_t pwm_enabled = 1;


  uint32_t count = 0;
  uint32_t seen = 0;
  uint32_t timeout = 500;  // 10s
  uint32_t timer1 = 0;
  uint32_t timer1_timeout = 100;
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
    analog = ReadChannel(ANALOG_GAS_PEDAL);
    #define POTI_LOW 84
    #define POTI_HIGH 919
    #define PWM_MAX 0x3ff
    
    uint16_t out;
    if(analog <= POTI_LOW){ // pedal released -> turn pwm off, set to 0 duty
      if(pwm_enabled){
        CLEAR_PWM0; //turn pwm pin off constantly
        disable_pwm();
        pwm_enabled = 0;
      }
      out = 0;
      CLEAR_LED_ONBOARD;
    }else if(analog >= POTI_HIGH){ // above max input -> turn pwm off, set to max value
      if(pwm_enabled){
        SET_PWM0; //turn pwm pin on constantly
        disable_pwm();
        pwm_enabled = 0;
      }
      out = 0x3ff;
      SET_LED_ONBOARD;
    }else{  // valid range, apply duty
      if(!pwm_enabled){
        enable_pwm();
        pwm_enabled = 1;
      }
      uint32_t tmp = (uint32_t)((analog - POTI_LOW)) * PWM_MAX;

      out = tmp / (POTI_HIGH - POTI_LOW);
      CLEAR_LED_ONBOARD;
    }


    OCR1A = out;
    OCR1B = out;
    //OCR1B = 0x3ff - analog;

    // indicate pwm mode (off when duty is max or off)
    if(pwm_enabled){
      SET_LED_EXTERNAL;
    }else{
      CLEAR_LED_EXTERNAL;
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

    if(analog >= 200){
      seen = count;
    }

    if(count > seen+timeout){
      CLEAR_MOSFET;
    }else{
      SET_MOSFET;
    }

    if(count > timer1+timer1_timeout){
      timer1 = count;
    }

    _delay_ms(10);
    count++;

    //if(IS_HIGH_SW0){
    //  SET_LED1;
    //}else{
    //  CLEAR_LED1;
    //}




    if(IS_HIGH_SW0){
      SET_RELAY;
    }else{
      CLEAR_RELAY;
    }

    if(IS_HIGH_SW1){
      //SET_BUZZER;
      //SET_MOSFET;
      SET_LED_EXTERNAL;
    }else{
      //CLEAR_BUZZER;
      //CLEAR_MOSFET;
      CLEAR_LED_EXTERNAL;
    }

  }
}
