/*************************************************************************/
/* Fichier:			 MAIN.H												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef MAIN_H
#define MAIN_H 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <stdio.h>

#include "ADC.h"
#include "PWM.h"
#include "moteur.h"
#include "UART.h"
#include "LED.h"
#include "TWI.h"

#ifndef F_CPU
#define F_CPU            16000000UL      /* Crystal 16.000 Mhz */
#endif

/*************************************************************************/
#endif
/*************************************************************************/