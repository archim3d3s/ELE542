/*************************************************************************/
/* Fichier:			 ADC.H												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef __ADC_H_
#define __ADC_H_

#include "main.h"

//MACROS
//Control of Calibration bit
#define CAL_bit_ON()	(PORTA |= (1<<PA4))
#define CAL_bit_OFF()	(PORTA &= ~(1<<PA4))

//ADC mux switching
#define	SWITCH_ADC_CHANNEL_0	ADMUX &= ~(1<<MUX0)
#define SWITCH_ADC_CHANNEL_1	ADMUX |= (1<<MUX0)

//     Bits de direction
// DIR2	DIR1 Sens de rotation
//  0    0		 Neutre
//  0    1	   Marche avant
//  1    0	  Marche arriere
//  1    1       Freins
//Control of direction bits
#define RIGHT_DIR1			(PD6)
#define RIGHT_DIR2			(PD7)
#define LEFT_DIR1			(PD2)
#define LEFT_DIR2			(PD3)

void DIR_bit_ON (INT8U direction);
void DIR_bit_OFF(INT8U direction);

//A2 et A3 permettent de connaitre le signe de la vitesse
#define DIR_G (PINA & (1<<PA2))
#define DIR_D (PINA & (1<<PA3))

/* ADC globals */
static volatile uint32_t 	LEFT_counter   = 0;
static volatile uint32_t	LEFT_ADCvalue  = 0;
static volatile uint32_t 	RIGHT_counter  = 0;
static volatile uint32_t	RIGHT_ADCvalue = 0;

//Calibration
#define CALIB_SAMPLE_NB		50

/******************************* FONCTIONS *******************************/
void initADC(void);
void motor_calibration(void);
void resetADC(void);
void getADCmeanValues(float* right, float* left);
/*************************************************************************/
#endif
/*************************************************************************/