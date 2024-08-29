#include "buzzer.h"

// process queued beeps
void buzzer_handle(buzzerConfig_t *buzzer)
{
  // currently on - turn off condition:
  if (buzzer->isOn)
  {
    if (((buzzer->countRemainingLong > 0) && (time_msPassedSince(buzzer->timestamp_turnedOn) > buzzer->msLong)) // timeout long beep
        || ((buzzer->countRemainingLong == 0) && (time_msPassedSince(buzzer->timestamp_turnedOn) > buzzer->msOn)))
    { // timeout normal beep
      GPIO_Clear(&buzzer->pin);
      buzzer->timestamp_turnedOff = time_get_ms();
      buzzer->isOn = 0;
      // decrement count
      if (buzzer->countRemainingLong > 0)
        buzzer->countRemainingLong--;
      else
        buzzer->countRemaining--;
    }
  }

  // currently off - turn on condition
  else if (((buzzer->countRemaining > 0) || (buzzer->countRemainingLong > 0)) && (time_msPassedSince(buzzer->timestamp_turnedOff) > buzzer->msOff))
  {
    GPIO_Set(&buzzer->pin);
    buzzer->timestamp_turnedOn = time_get_ms();
    buzzer->isOn = 1;
  }
}


// queue beep count
void buzzer_beep(buzzerConfig_t *buzzer, uint8_t count)
{
  buzzer->countRemaining += count; // add new count
  buzzer_handle(buzzer);           // update immediately
}


// queue long beep count //note: long beeps are always run first in queue
void buzzer_beepLong(buzzerConfig_t *buzzer, uint8_t count)
{
  buzzer->countRemainingLong += count; // add new count
  buzzer_handle(buzzer);               // update immediately
}


// block until all queued beeps are finished
void buzzer_wait(buzzerConfig_t * buzzer){
  while (buzzer->countRemaining > 0 || buzzer->countRemainingLong > 0)
    buzzer_handle(buzzer);
}


// trigger beeps and block until all queued beeps are finished
void buzzer_beepWait(buzzerConfig_t *buzzer, uint8_t count)
{
  buzzer_beep(buzzer, count);
  buzzer_wait(buzzer);
}