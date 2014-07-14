/*************************************************************************/
/* Fichier:			 UART.H												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef UART_H
#define UART_H

#include "main.h"

#define UART_BAUD_RATE   9600    
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

void debug_send(uint8_t *buf, uint8_t size);
void initUART(void);

/*************************************************************************/
#endif
/*************************************************************************/