#include <util/delay.h>
#include "usart.h"
#include "adc.h"
#include "gpio.h"
#include <avr/wdt.h>
#include "time.h"

// struct that configures PWM and fan settings
typedef struct
{
  // Maximum count value for Fast PWM
  // Possible values: 1023(10Bit), 511(9Bit), 255(8Bit)
  uint16_t pwmResolution;

  // Factor clock is reduced before counting up the value
  // Possible values: 1, 8, 64, 256, 1024
  uint8_t pwmPrescaler;

  uint32_t fanTurnOffDelay;
  uint32_t fanTurnOnDelay;
  uint32_t fanTurnOnDuty;
} ekartConfig_t;


// define multiple config presets, those can be switched between / selected at runtime
ekartConfig_t configs[] = {
      { //122 Hz (V1) []
        .pwmResolution = 1023,
        .pwmPrescaler = 64,
        .fanTurnOffDelay = 10000,
        .fanTurnOnDelay = 2000,
        .fanTurnOnDuty = 10
    },
        { //244 Hz
        .pwmResolution = 511,
        .pwmPrescaler = 64,
        .fanTurnOffDelay = 15000,
        .fanTurnOnDelay = 2000,
        .fanTurnOnDuty = 10
    },
        { //488 Hz []
        .pwmResolution = 255,
        .pwmPrescaler = 64,
        .fanTurnOffDelay = 15000,
        .fanTurnOnDelay = 2000,
        .fanTurnOnDuty = 10
    },
      { //977 Hz (V1) []
        .pwmResolution = 1023,
        .pwmPrescaler = 8,
        .fanTurnOffDelay = 20000,
        .fanTurnOnDelay = 2000,
        .fanTurnOnDuty = 10
    },
      { //3.9 kHz []
        .pwmResolution = 255,
        .pwmPrescaler = 8,
        .fanTurnOffDelay = 20000,
        .fanTurnOnDelay = 1000,
        .fanTurnOnDuty = 0
    },
      { //7.8 kHz (default)
        .pwmResolution = 1023,
        .pwmPrescaler = 1,
        .fanTurnOffDelay = 20000,
        .fanTurnOnDelay = 1000,
        .fanTurnOnDuty = 0
    },
      { //15.6 kHz (note: too fast in low duty - never off in cycle, gets hot fast)
        .pwmResolution = 511,
        .pwmPrescaler = 1,
        .fanTurnOffDelay = 50000,
        .fanTurnOnDelay = 500,
        .fanTurnOnDuty = 0
    },
};

uint8_t configCount = sizeof(configs)/sizeof(ekartConfig_t);
// default config
uint8_t selectedConfigIndex = 5;

//====================
//==== PWM-config ====
//====================
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
#define PWM_FREQUENCY(PWM_PRESCALER, PWM_RESOLUTION) ((F_CPU) / (PWM_PRESCALER) / ((PWM_RESOLUTION) + 1))



//====================
//====== config ======
//====================
#define GAS_PEDAL_MIN 90 // actual 82
#define GAS_PEDAL_MAX 542 //actual 543

#define DUTY_PERCENT_SLOW_MODE 45 //max duty percent in slow mode (long button press)



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
GPIO_Pin switch1 = {PC2, &PORTC, &DDRC, &PINC};
GPIO_Pin switch2 = {PC3, &PORTC, &DDRC, &PINC};
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
//returns 1 if a parameter was not an allowed value
int Timer1_FastPWM_Init(uint16_t resolution, uint8_t prescaler)
{
  // Set pwm mode Fast-PWM with selected resolution
  // Mode 5-7 Datasheet page 98 Table 39
  if (resolution == 1023)
    TCCR1A = (1 << WGM10) | (1 << WGM11); // Mode 7: Fast PWM, 10-bit
  else if (resolution == 511)
    TCCR1A = (0 << WGM10) | (1 << WGM11); // Mode 6: Fast PWM, 9-bit
  else if (resolution == 255)
    TCCR1A = (1 << WGM10) | (0 << WGM11); // Mode 5: Fast PWM, 8-bit
  else
    // invalid resolution value
    return 1;

  // Set the prescaler based on the selected value
  if (prescaler == 1)
    TCCR1B = (1 << WGM12) | (1 << CS10); // No prescaling
  else if (prescaler == 8)
    TCCR1B = (1 << WGM12) | (1 << CS11); // Prescaler = 8
  else if (prescaler == 64)
    TCCR1B = (1 << WGM12) | ((1 << CS11) | (1 << CS10)); // Prescaler = 64
  else if (prescaler == 256)
    TCCR1B = (1 << WGM12) | (1 << CS12); // Prescaler = 256
  else if (prescaler == 8)
    TCCR1B = (1 << WGM12) | ((1 << CS12) | (1 << CS10)); // Prescaler = 1024
  else
    // invalid prescaler value
    return 1;

  // Set non-inverting mode
  TCCR1A |= (1 << COM1A1); // Clear OC1A on Compare Match, set OC1A at BOTTOM
  // If using OCR1B, uncomment the next line
  // TCCR1A |= (1 << COM1B1);  // Clear OC1B on Compare Match, set OC1B at BOTTOM

  // Set PB1/OC1A as output (for OC1A PWM)
  DDRB |= (1 << PB1);
  // If using OC1B, set PB2 as output
  // DDRB |= (1 << PB2);
  return 0;
}

void disable_pwm(){
  TCCR1A = 0;
  TCCR1B = 0;
}

void enable_pwm(){
  int res = Timer1_FastPWM_Init(configs[selectedConfigIndex].pwmResolution, configs[selectedConfigIndex].pwmPrescaler);
  if (res){
    printf("==== ERROR: PWM INIT FAILED! invalid parameter provided config_index=%d ====", selectedConfigIndex);
    GPIO_Set(&buzzer);
    _delay_ms(3000);
    GPIO_Clear(&buzzer);
    //trigger reset
    wdt_enable(WDTO_15MS);
  }
}


//max duty is configured PWM_RESOLUTION
//note: due to fast-pwm mode pin can never be fully off or on
// there will still be one off/on cycle at 1023/0 duty
void Set_PWM_Duty_Cycle(uint16_t duty_cycle, uint16_t max_dutyCycle)
{
  //clip to max value
  if (duty_cycle > max_dutyCycle)
    duty_cycle = max_dutyCycle;

 // Set duty cycle for OC1A
  OCR1A = duty_cycle;
  // If using OCR1B, set duty cycle for OC1B
  // OCR1B = duty_cycle;
}



// beep certain count
void beep(uint8_t count)
{
  for (int i = 0; i < count; i++)
  {
    GPIO_Set(&buzzer);
    GPIO_Set(&ledExternal);
    _delay_ms(60);
    GPIO_Clear(&buzzer);
    GPIO_Clear(&ledExternal);
    _delay_ms(100);
  }
}


typedef enum {FULL_OFF = 0, FULL_ON, PWM} motorState_t;
const char* motorState_str[] = {"FULL_OFF", "FULL_ON", "PWM"};





//======================
//======== main ========
//======================
int main(void)
{
  // init PWM
  Timer1_FastPWM_Init(configs[selectedConfigIndex].pwmResolution, configs[selectedConfigIndex].pwmPrescaler);
  disable_pwm(); // initially off

  // init adc
  ADC_Init();

  // init gpio
  init_gpios();
  GPIO_Set(&fan); //turn off fan initially

  // init Timer+ISR that tracks time in ms for time_ functions
  time_init();

  // beep
  beep(3);

  // external led always on
  GPIO_Set(&ledExternal);


  // init UART
  uart_init();
  uart_sendStr("startup finished\n");
  printf("CPU Frequency is %ld Hz\n", F_CPU);


  // variables
  uint32_t gasPedalAdc;
  uint8_t pwmEnabled = 0;
  motorState_t state = FULL_OFF;
  motorState_t statePrev = FULL_OFF;
  uint16_t duty = 0;
  uint32_t timestamp_turnedOn = 0;
  uint32_t timestamp_turnedOff = 0;
  // edge detection
  uint8_t switchPrevious = 0;
  uint32_t timestamp_buttonPressed = 0;

  uint16_t maxDutyPercent = 100;
  uint8_t lock = 0;

  //==== loop ====
  while (1)
  {

    //load currently selected config
    ekartConfig_t conf = configs[selectedConfigIndex];



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
        timestamp_turnedOff = time_get_ms();
      }
      // set pin constantly low = full off
      GPIO_Clear(&pwm);
    }
    else if ((gasPedalAdc >= GAS_PEDAL_MAX) && (maxDutyPercent == 100)) // pedal fully pressed
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
        timestamp_turnedOn = time_get_ms();
      }
      // define duty
      // scale pedal input to pwm value
      duty = (gasPedalAdc - GAS_PEDAL_MIN) * conf.pwmResolution * maxDutyPercent / 100 / (GAS_PEDAL_MAX - GAS_PEDAL_MIN);


      // update duty
      Set_PWM_Duty_Cycle(duty, conf.pwmResolution);
    }



    //=== logging ===
    // debug output via UART
    //note: for some reason wrong values are printed when printing multiple variables in on printf?!
    printf("adc=%04d", gasPedalAdc); 

    printf(" pedal=%04d/1000",
      (int32_t)(gasPedalAdc - GAS_PEDAL_MIN) * 1000 / (GAS_PEDAL_MAX - GAS_PEDAL_MIN));

    printf(" pwm=%04d/%d", duty, conf.pwmResolution);
    
    printf(" state='%s'", motorState_str[(int)state]);

    printf("freq=%dHz\n", PWM_FREQUENCY(conf.pwmPrescaler, conf.pwmResolution));



    //=== onboard LED ===
    // on when motor is running
    GPIO_SetLevel(&ledOnboard, (gasPedalAdc >= GAS_PEDAL_MIN));


    //=== external LED ===
    // always on except its blinked elsewhere
    GPIO_Set(&ledExternal);



    //=== fan ===
    // note: output is inverted
    // turn fan on when motor on and above thresholds
    uint32_t timeOn = time_msPassedSince(timestamp_turnedOn);
    if (state != FULL_OFF &&  (duty >= conf.fanTurnOnDuty) && ((time_msPassedSince(timestamp_turnedOn)) > conf.fanTurnOnDelay))
      GPIO_Clear(&fan); // turn on

    // turn fan off when motor off long enough
    else if (state == FULL_OFF && (time_msPassedSince(timestamp_turnedOff) > conf.fanTurnOffDelay))
      GPIO_Set(&fan); // turn off

    // printf("time=%d, ", time);
    // printf("turnedon=%d", timestamp_turnedOn);
    // printf("timeon=%d\n", timeOn);
    //  manual control via switch near emergency stop
    //  GPIO_SetLevel(&fan, !(GPIO_Read(&switch2)));



    //=== Switch, Relay ===
    // on when switch1 is high (currently not connected)
    GPIO_SetLevel(&relay, GPIO_Read(&switch1));


    //==== handle Button ====
    uint8_t switchNow = GPIO_Read(&switch2);
    uint32_t timePressed = time_msPassedSince(timestamp_buttonPressed); //TODO: only calculate this when switchLast is 1 otherwise 0 and reset lock?

    if (switchPrevious == 0 && switchNow == 1) // rising edge
    {
      timestamp_buttonPressed = time_get_ms();
    }

    else if (switchPrevious == 1 && switchNow == 0) // falling edge
    {
      if (timePressed < 1000) //short press
      {
    //--- cycle through configs ---
        selectedConfigIndex++;
        if (selectedConfigIndex >= configCount) // rotate back first index
          selectedConfigIndex = 0;
        // re-configure pwm immediately when active
        if (pwmEnabled){
          Set_PWM_Duty_Cycle(0, conf.pwmResolution); // set to low duty for this cycle
          enable_pwm(); // re-initialize with new settings

        }
        printf("cycled config index to %d", selectedConfigIndex);
        beep(2);
      }
    }

    else if ((switchNow == 1 && timePressed > 1000) && !lock) //long press
    {
      //--- toggle slow mode ---
      if (maxDutyPercent != 100) // enter normal mode (full speed)
      {
        maxDutyPercent = 100;
        GPIO_Set(&buzzer);
        _delay_ms(200);
        GPIO_Clear(&buzzer);
      }
      else  //enter slow mode
      {
        maxDutyPercent = DUTY_PERCENT_SLOW_MODE;
        GPIO_Set(&buzzer);
        _delay_ms(1500);
        GPIO_Clear(&buzzer);
      }
      lock = 1; // prevent multiple runs while still pressed
    }
    else if (switchNow == 0) // released
    {
      lock = 0; // reset long pressed lock
    }

    // update previous switch state
    switchPrevious = switchNow;




    //=== Voltage-Threshold ===
    //beep when battery below voltage threshold (opamp+potentiometer)
    //pass through batt threshold signal to buzzer
    //GPIO_SetLevel(&buzzer, !(GPIO_Read(&batteryThreshold)));

    //when below voltage threshold: beep every time when pedal gets released
    if (statePrev == PWM && state == FULL_OFF && !(GPIO_Read(&batteryThreshold))) // pedal just fully released and battery low
    {
      GPIO_Set(&buzzer);
      _delay_ms(800);
      GPIO_Clear(&buzzer);
    }
    // update last state
    statePrev = state;



    _delay_ms(1);
  }
}
