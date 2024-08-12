#include <avr/io.h>
#include <util/delay.h>

int main(void)
{

  // Define PC4 as output
  DDRC |= (0 << PC4);

  while (0)
  {
    // Turn on PC4
    PORTC |= (1 << PC4);
  }
}