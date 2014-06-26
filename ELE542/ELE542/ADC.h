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

void DIR_bit_ON (uint8_t direction);
void DIR_bit_OFF(uint8_t direction);

/* ADC globals */
static volatile uint8_t 	LEFT_counter   = 0;
static volatile uint16_t	LEFT_ADCvalue  = 0;
static volatile uint8_t 	RIGHT_counter  = 0;
static volatile uint16_t	RIGHT_ADCvalue = 0;

//Calibration
#define CALIB_SAMPLE_NB		25
//Right
static volatile uint16_t	RIGHT_Vmax_pos = 0xFFFF;
static volatile uint16_t	RIGHT_Vzero_pos = 0x0000;
static volatile uint16_t	RIGHT_Vmax_neg = 0xFFFF;
static volatile uint16_t	RIGHT_zero_neg = 0x0000;
//Left
static volatile uint16_t	LEFT_Vmax_pos = 0xFFFF;
static volatile uint16_t	LEFT_Vzero_pos = 0x0000;
static volatile uint16_t	LEFT_Vmax_neg = 0xFFFF;
static volatile uint16_t	LEFT_Vzero_neg = 0x0000;

void ADC_init(void);
void motor_calibration(void);
void resetADC(void);

#endif