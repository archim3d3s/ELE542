/*************************************************************************/
/* Fichier:			 LED.H											 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef LED_H
#define LED_H

#include "main.h"

/*         STATE		  LED #        */
#define COMMANDE_RECUE 		1
#define OBS_SONAR_D			2
#define PING_SONAR_D		3
#define OBS_SONAR_G			4
#define PING_SONAR_G		5
#define ATTENTE				6	
#define ACTIF				7

/* 		  MACROS	   	  */
void LED_ON(INT8U state);
void LED_OFF(INT8U state);
	
/* 		 FUNCTIONS		  */
void initLED(void);
void updateLED(INT8U status);

/*************************************************************************/
#endif
/*************************************************************************/