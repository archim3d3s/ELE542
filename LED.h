#ifndef LED_H
#define LED_H

#include <avr/io.h>

/*           STATE	LED #             */
#define COMMANDE_RECUE 	  1
#define OBS_SONAR_D	  2
#define PING_SONAR_D	  3
#define OBS_SONAR_G	  4
#define PING_SONAR_G	  5
#define ATTENTE		  6	
#define ACTIF		  7

/* 		  MACROS	   	  */
#define LED_ON(state) {PORTB &= ~(1<<state)}
#define LED_ON(state) {PORTB |=  (1<<state)}
/* 		 FUNCTIONS		  */
void initLED(void);

#endif
