#include "LED.h"

void initLED(void)
{
    /* PORT B as output */
    DDRB = 0xFF;
    /* All LED off */
    PORTB = 0xFF;
}
