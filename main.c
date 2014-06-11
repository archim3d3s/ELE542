

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "Common.h"
#include <stdio.h>

extern volatile int commande[3];

int main(void)
{
	char debugString[5]= {0};

    DDRB = 0xff;      /* PortB output */
    PORTB = 0x00;     /* switch LEDs on */

    uart_init();
    sei();                 /* enable interrupts */

	sprintf(debugString,"%d%d%d",  commande[0], commande[1], commande[2]);
	debug_send(debugString, 3);
    for (;;) {
		sprintf(debugString,"%d%d%d",  commande[0], commande[1], commande[2]);
		debug_send(debugString, 3);
	             /* loop forever */
    }            
}
