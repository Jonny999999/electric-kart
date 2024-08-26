#include <util/delay.h>
#include "usart.h"
#include "adc.h"
#include "gpio.h"


//====================
//==== PWM-config ====
//====================
// Maximum count value for Fast PWM 
#define PWM_RESOLUTION 1023  //Possible values: 1023(10Bit), 511(9Bit), 255(8Bit)

// Factor clock is reduced before counting up the value
#define PWM_PRESCALER 64 // Possible values: 1, 8, 64, 256, 1024

//Pre-calculated possible combinations:
/*
Resolution,	Prescaler, PWM-Freq @ 8Mhz
1023	  1	      7813
1023	  8	      977
1023	  64	    122  <-- used in V1
1023	  256	    31
1023	  1024	  8
		
511	    1	      15625
511	    8	      1953
511	    64	    244
511	    256	    61
511	    1024	  15
		
255	    1	      31250
255	    8	      3906
255	    64	    488
255	    256	    122
255	    1024  	31
*/

// Calculate PWM frequency for logging
#define PWM_FREQUENCY (F_CPU / PWM_PRESCALER / (PWM_RESOLUTION + 1))
// helper functions for logging macro
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#pragma message "Resulting PWM-Frequency is: " STR(PWM_FREQUENCY) "  Hz"



//====================
//====== config ======
//====================
#define GAS_PEDAL_MIN 100 // actual 172
#define GAS_PEDAL_MAX 900

#define FAN_TURN_ON_DUTY 50
#define FAN_TURN_OFF_DELAY_MS 3000



//===========================
//===== Assign GPO Pins =====
//===========================
//outputs
GPIO_Pin relay = { PD3, &PORTD, &DDRD, &PIND };
GPIO_Pin pwm = {PB1, &PORTB, &DDRB, &PINB};
GPIO_Pin fan = {PD4, &PORTD, &DDRD, &PIND};
GPIO_Pin ledOnboard = {PC4, &PORTC, &DDRC, &PINC};
GPIO_Pin ledExternal = {PB2, &PORTB, &DDRB, &PINB};
GPIO_Pin buzzer = {PC5, &PORTC, &DDRC, &PINC};

//inputs
GPIO_Pin switch1 = {PC2, &PORTC, &DDRC, &PINC};
GPIO_Pin switch2 = {PC3, &PORTC, &DDRC, &PIND};
GPIO_Pin analog = {PC0, &PORTC, &DDRC, &PINC};
GPIO_Pin gasPedal = {PC1, &PORTC, &DDRC, &PINC};
GPIO_Pin batteryThreshold = {PD2, &PORTD, &DDRD, &PIND};

void init_gpios(){
  //outputs
  GPIO_Init(&relay, 1);
  GPIO_Init(&pwm, 1);
  GPIO_Init(&fan, 1);
  GPIO_Init(&ledOnboard, 1);
  GPIO_Init(&ledExternal, 1);
  GPIO_Init(&buzzer, 1);
  // inputs
  GPIO_Init(&switch1, 0);
  GPIO_Init(&switch2, 0);
  GPIO_Init(&analog, 0);
  GPIO_Init(&gasPedal, 0);
  GPIO_Init(&batteryThreshold, 0);
}




//=====================
//===== functions =====
//=====================
void Timer1_FastPWM_Init(void)
{
  // Set pwm mode Fast-PWM with selected resolution
  // Mode 5-7 Datasheet page 98 Table 39
  #if PWM_RESOLUTION == 1023
  TCCR1A = (1 << WGM10) | (1 << WGM11); // Mode 7: Fast PWM, 10-bit
  #elif PWM_RESOLUTION == 511
  TCCR1A = (0 << WGM10) | (1 << WGM11); // Mode 6: Fast PWM, 9-bit
  #elif PWM_RESOLUTION == 254
  TCCR1A = (1 << WGM10) | (0 << WGM11); // Mode 5: Fast PWM, 8-bit
  #else
    #error "invalid PWM_RESOLUTION value"
  #endif

// Set the prescaler based on the selected value
#if PWM_PRESCALER == 1
  TCCR1B = (1 << WGM12) | (1 << CS10); // No prescaling
#elif PWM_PRESCALER == 8
  TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler = 8
#elif PWM_PRESCALER == 64
  TCCR1B = (1 << WGM12) | ((1 << CS11) | (1 << CS10)); // Prescaler = 64
#elif PWM_PRESCALER == 256
  TCCR1B = (1 << WGM12) | (1 << CS12); // Prescaler = 256
#elif PWM_PRESCALER == 1024
  TCCR1B = (1 << WGM12) | ((1 << CS12) | (1 << CS10)); // Prescaler = 1024
#else
#error "Invalid PWM_PRESCALER value"
#endif

  // Set non-inverting mode
  TCCR1A |= (1 << COM1A1); // Clear OC1A on Compare Match, set OC1A at BOTTOM
  // If using OCR1B, uncomment the next line
  // TCCR1A |= (1 << COM1B1);  // Clear OC1B on Compare Match, set OC1B at BOTTOM

  // Set PB1/OC1A as output (for OC1A PWM)
  DDRB |= (1 << PB1);
  // If using OC1B, set PB2 as output
  // DDRB |= (1 << PB2);
}

void disable_pwm(){
  TCCR1A = 0;
  TCCR1B = 0;
}

void enable_pwm(){
  Timer1_FastPWM_Init();
}


void Set_PWM_Duty_Cycle(uint16_t duty_cycle)
{
  OCR1A = duty_cycle; // Set duty cycle for OC1A
                      // If using OCR1B, set duty cycle for OC1B
                      // OCR1B = duty_cycle;
}





int main(void)
{
  // init PWM
  Timer1_FastPWM_Init(); // init timer1 in fast PWM mode with 10bit resolution on pin PB1

  // init adc
  ADC_Init();

  // init gpio
  init_gpios();

  // beep at startup
  for (int i = 0; i < 3; i++)
  {
    GPIO_Set(&buzzer);
    _delay_ms(60);
    GPIO_Clear(&buzzer);
    _delay_ms(100);
  }
  // init UART
  uart_init();
  uart_sendStr("startup finished\n");
  printf("CPU Frequency is %d Hz\n", F_CPU);
  printf("PWM Frequency is %d Hz\n", PWM_FREQUENCY);


  // variables
  uint16_t gasPedalAdc;

  uint8_t pwmEnabled = 0;
  uint16_t duty = 0;

  //==== loop ====
  while (1)
  {

    // read gas pedal
    gasPedalAdc = ADC_ReadChannel(gasPedal.bit);

    // handle PWM pin
    if (gasPedalAdc <= GAS_PEDAL_MIN) // pedal released
    {
      // turn off pwm for complete stop
      if (pwmEnabled)
      { 
        disable_pwm();
        pwmEnabled = 0;
      }
      // set pin constantly low = full off
      GPIO_Clear(&pwm);
    }
    else if (gasPedalAdc >= GAS_PEDAL_MAX) // pedal fully pressed
    {
      // turn off pwm for full on
      if (pwmEnabled)
      {
        disable_pwm();
        pwmEnabled = 0;
      }
      // set pin constantly high = full on
      GPIO_Set(&pwm);
    }
    else // pedal anywhere between min/max
    {
      if (!pwmEnabled)
      { // re-enable pwm if previously off
        enable_pwm();
        pwmEnabled = 1;
      }
      // define duty
      // scale pedal input to pwm value
      duty = (gasPedalAdc - GAS_PEDAL_MIN) * PWM_RESOLUTION / (GAS_PEDAL_MAX - GAS_PEDAL_MIN);


      // debug output via UART
      printf("adc=%04d,  pwm=%04d/%d", gasPedalAdc, duty, PWM_RESOLUTION);
      // update duty
      Set_PWM_Duty_Cycle(duty);
    }


    // onboard LED
    // on when motor is running
    GPIO_SetLevel(&ledOnboard, (gasPedalAdc >= GAS_PEDAL_MIN));


    // Relay
    // on when switch1 is high
    GPIO_SetLevel(&relay, GPIO_Read(&switch1));

    //beep when battery below voltage threshold (opamp+potentiometer)
    //pass through batt threshold signal to buzzer
    if (GPIO_Read(&batteryThreshold))
      GPIO_Clear(&buzzer);
    else
      GPIO_Set(&buzzer);


    _delay_ms(10);
  }
}
