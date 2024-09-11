#include "gpio.h"
#include "time.h"


// struct for buzzer instance
typedef struct
{
  const GPIO_Pin pin;
  const uint32_t msOn;
  const uint32_t msOff;
  const uint32_t msLong;
  uint8_t isOn;
  uint8_t countRemainingLong;
  uint8_t countRemaining;
  uint32_t timestamp_turnedOn;
  uint32_t timestamp_turnedOff;
} buzzerConfig_t;

// process queued beeps (run repeatedly in main loop)
void buzzer_handle(buzzerConfig_t *buzzer);

// queue beep count
void buzzer_beep(buzzerConfig_t *buzzer, uint8_t count);

// queue long beep count //note: long beeps are always run first in queue
void buzzer_beepLong(buzzerConfig_t *buzzer, uint8_t count);

// block until all queued beeps are finished
void buzzer_wait(buzzerConfig_t * buzzer);

// trigger beeps and block until all queued beeps are finished
void buzzer_beepWait(buzzerConfig_t *buzzer, uint8_t count);