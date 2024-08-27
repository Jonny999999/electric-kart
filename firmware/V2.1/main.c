#include <util/delay.h>
#include "usart.h"
#include "adc.h"
#include "gpio.h"


//====================
//==== PWM-config ====
//====================
// Maximum count value for Fast PWM 
#define PWM_RESOLUTION 1023 //Possible values: 1023(10Bit), 511(9Bit), 255(8Bit)

// Factor clock is reduced before counting up the value
#define PWM_PRESCALER 1 // Possible values: 1, 8, 64, 256, 1024

//Pre-calculated possible combinations:
/*
|  Resolution  |  Prescaler  |  PWM-Freq @ 8MHz  |  Comment  |
1023	  1	      7813      (2) >> OK - minor beeping, temperature just fine with fan on
1023	  8	      977       (5) >> still beeping, a little less annoying than 2kHz (4)
1023	  64	    122       (8) <== used in V1 ==> loud humming and metal vibrations
1023	  256	    31
1023	  1024	  8
		
511	    1	      15625     (1) >> OK - no sound at all, but IGBT gets VERY-HOT in seconds
511	    8	      1953      (4) >> annoying beeping, similar to 3.9kHz (3)
511	    64	    244       (7) >> OK - quieter than V1.0 122Hz (8)
511	    256	    61
511	    1024	  15
		
255	    1	      31250
255	    8	      3906      (3) >> very annoying/loud beeping
255	    64	    488       (6) >> annoying low beep
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
#define GAS_PEDAL_MIN 90 // actual 82
#define GAS_PEDAL_MAX 542 //actual 543

#define FAN_TURN_ON_DUTY 50
#define FAN_TURN_OFF_DELAY_MS 3000



//===============================
//===== Configure GPIO Pins =====
//===============================
//outputs
GPIO_Pin relay = { PD3, &PORTD, &DDRD, &PIND };
GPIO_Pin pwm = {PB1, &PORTB, &DDRB, &PINB};
GPIO_Pin fan = {PD4, &PORTD, &DDRD, &PIND};
GPIO_Pin ledOnboard = {PC4, &PORTC, &DDRC, &PINC};
GPIO_Pin ledExternal = {PB2, &PORTB, &DDRB, &PINB};
GPIO_Pin buzzer = {PC5, &PORTC, &DDRC, &PINC};

//inputs
GPIO_Pin switch1 = {PC3, &PORTC, &DDRC, &PINC};
GPIO_Pin switch2 = {PC2, &PORTC, &DDRC, &PINC};
GPIO_Pin analog = {PC0, &PORTC, &DDRC, &PINC};
GPIO_Pin gasPedal = {PC1, &PORTC, &DDRC, &PINC};
GPIO_Pin batteryThreshold = {PD2, &PORTD, &DDRD, &PIND};

//Initialize Pins (define direction)
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
  #elif PWM_RESOLUTION == 255
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


//max duty is configured PWM_RESOLUTION
//note: due to fast-pwm mode pin can never be fully off or on
// there will still be one off/on cycle at 1023/0 duty
void Set_PWM_Duty_Cycle(uint16_t duty_cycle)
{
  //clip to max value
  if (duty_cycle > PWM_RESOLUTION)
    duty_cycle = PWM_RESOLUTION;

 // Set duty cycle for OC1A
  OCR1A = duty_cycle;
  // If using OCR1B, set duty cycle for OC1B
  // OCR1B = duty_cycle;
}


typedef enum {FULL_OFF = 0, FULL_ON, PWM} motorState_t;
const char* motorState_str[] = {"FULL_OFF", "FULL_ON", "PWM"};


//======================
//======== main ========
//======================
int main(void)
{
  // init PWM
  Timer1_FastPWM_Init(); // init timer1 in fast PWM mode with 10bit resolution on pin PB1
  disable_pwm(); // initially off

  // init adc
  ADC_Init();

  // init gpio
  init_gpios();
  GPIO_Set(&fan); //turn off fan initially

  // beep at startup
  for (int i = 0; i < 3; i++)
  {
    GPIO_Set(&buzzer);
    GPIO_Set(&ledExternal);
    _delay_ms(60);
    GPIO_Clear(&buzzer);
    GPIO_Clear(&ledExternal);
    _delay_ms(100);
  }
  // external led always on
  GPIO_Set(&ledExternal);


  // init UART
  uart_init();
  uart_sendStr("startup finished\n");
  printf("CPU Frequency is %ld Hz\n", F_CPU);
  printf("PWM Frequency is %d Hz\n", PWM_FREQUENCY);


  // variables
  uint32_t gasPedalAdc;
  uint8_t pwmEnabled = 0;
  motorState_t state = FULL_OFF;
  uint16_t duty = 0;
  uint32_t time = 0;
  uint32_t timestamp_turnedOn = 0;
  uint32_t timestamp_turnedOff = 0;

  //==== loop ====
  while (1)
  {

    // read gas pedal
    gasPedalAdc = ADC_ReadChannel(gasPedal.bit);

    //=== handle PWM pin ===
    if (gasPedalAdc <= GAS_PEDAL_MIN) // pedal released
    {
      state = FULL_OFF;
      // turn off pwm for complete stop
      if (pwmEnabled)
      { 
        disable_pwm();
        pwmEnabled = 0;
        timestamp_turnedOff = time;
      }
      // set pin constantly low = full off
      GPIO_Clear(&pwm);
    }
    else if (gasPedalAdc >= GAS_PEDAL_MAX) // pedal fully pressed
    {
      state = FULL_ON;
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
      state = PWM;
      if (!pwmEnabled)
      { // re-enable pwm if previously off
        enable_pwm();
        pwmEnabled = 1;
        timestamp_turnedOn = time;
      }
      // define duty
      // scale pedal input to pwm value
      duty = (gasPedalAdc - GAS_PEDAL_MIN) * PWM_RESOLUTION / (GAS_PEDAL_MAX - GAS_PEDAL_MIN);


      // update duty
      Set_PWM_Duty_Cycle(duty);
    }



    //=== logging ===
    // debug output via UART
    //note: for some reason wrong values are printed when printing multiple variables in on printf?!
    printf("adc=%04d", gasPedalAdc); 

    printf(" pedal=%04d/1000",
      (int32_t)(gasPedalAdc - GAS_PEDAL_MIN) * 1000 / (GAS_PEDAL_MAX - GAS_PEDAL_MIN));

    printf(" pwm=%04d/%d", duty, PWM_RESOLUTION);
    
    printf(" state='%s'\n", motorState_str[(int)state]);


    //=== onboard LED ===
    // on when motor is running
    GPIO_SetLevel(&ledOnboard, (gasPedalAdc >= GAS_PEDAL_MIN));



    //=== fan ===
    // note: output is inverted
    // turn fan on when above threshold
   // if (duty > FAN_TURN_ON_DUTY)
   //   GPIO_Clear(&fan);
   // // fan still on, motor off long enough
   // else if (time - timestamp_turnedOff) > FAN_TURN_OFF_DELAY_MS )
   //   GPIO_Set(&fan);

    // manual control via switch near emergency stop
    GPIO_SetLevel(&fan, !(GPIO_Read(&switch2)));


    //=== Relay ===
    // on when switch1 is high (currently not connected)
    GPIO_SetLevel(&relay, GPIO_Read(&switch1));



    //=== Voltage-Threshold ===
    //beep when battery below voltage threshold (opamp+potentiometer)
    //pass through batt threshold signal to buzzer
    GPIO_SetLevel(&buzzer, !(GPIO_Read(&batteryThreshold)));


    _delay_ms(10);
    time+=10; //TODO: overflows in 49 days
  }
}
