/*************************************************************************/
/* Fichier:			 LED.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#include "LED.h"

extern volatile INT8U flag_newStatus;

void LED_ON(INT8U state)  {PORTB &= ~(1<<state);}
void LED_OFF(INT8U state) {PORTB |=  (1<<state);}
	
/****************************** INIT LED *********************************/
void initLED(void)
{
    /* PORT B as output */
    DDRB = 0xFF;
    /* All LED off */
    PORTB = 0xFF;
}
/*************************************************************************/

/****************************** UPDATE LED *******************************/
void updateLED(INT8U status)
{
	PORTB = ~status;
}
/*************************************************************************/