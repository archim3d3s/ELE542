#include "LED.h"

extern volatile uint8_t flag_newStatus;

void LED_ON(uint8_t state)  {PORTB &= ~(1<<state);}
void LED_OFF(uint8_t state) {PORTB |=  (1<<state);}

void initLED(void)
{
    /* PORT B as output */
    DDRB = 0xFF;
    /* All LED off */
    PORTB = 0xFF;
}

void updateLED(uint8_t status)
{
	PORTB = ~status;
}


