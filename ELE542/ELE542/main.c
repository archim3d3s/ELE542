

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Common.h"
#include <stdio.h>

extern volatile int commande[3];

int main(void)
{
	u08 debugString[5]= {0};

    DDRB = 0xff;      /* PortB output */
    PORTB = 0x00;     /* switch LEDs on */

    uart_init();
    sei();                 /* enable interrupts */

	sprintf((char*) debugString,"%d%d%d",  (char) commande[0], (char) commande[1], (char) commande[2]);
	debug_send(debugString, 3);
	
    for (;;) /* loop forever */
	{ 
		
    }            
}
