#ifndef __ADC_H_
#define __ADC_H_

#include "main.h"

//Defines for controlling right engine h-bridge
#define RIGHT_ENGINE_MASK		(1<<MOTEUR_DROIT_DIR1)|(1<<MOTEUR_DROIT_DIR2)
#define RIGHT_FOWARD			PORTD = (PORTD & ~(RIGHT_ENGINE_MASK))|(1<<MOTEUR_DROIT_DIR1)|(0<<MOTEUR_DROIT_DIR2)
#define	RIGHT_REVERSE			PORTD = (PORTD & ~(RIGHT_ENGINE_MASK))|(0<<MOTEUR_DROIT_DIR1)|(1<<MOTEUR_DROIT_DIR2)
#define RIGHT_NEUTRAL			PORTD = (PORTD & ~(RIGHT_ENGINE_MASK))|(0<<MOTEUR_DROIT_DIR1)|(0<<MOTEUR_DROIT_DIR2)
#define RIGHT_BRAKE				PORTD = (PORTD & ~(RIGHT_ENGINE_MASK))|(1<<MOTEUR_DROIT_DIR1)|(1<<MOTEUR_DROIT_DIR2)
//Defines for controlling right engine h-bridge
#define LEFT_ENGINE_MASK		(1<<MOTEUR_GAUCHE_DIR1)|(1<<MOTEUR_GAUCHE_DIR2)
#define LEFT_FOWARD				PORTD = (PORTD & ~(LEFT_ENGINE_MASK))|(1<<MOTEUR_GAUCHE_DIR1)|(0<<MOTEUR_GAUCHE_DIR2)
#define LEFT_REVERSE			PORTD = (PORTD & ~(LEFT_ENGINE_MASK))|(0<<MOTEUR_GAUCHE_DIR1)|(1<<MOTEUR_GAUCHE_DIR2)
#define LEFT_NEUTRAL			PORTD = (PORTD & ~(LEFT_ENGINE_MASK))|(0<<MOTEUR_GAUCHE_DIR1)|(0<<MOTEUR_GAUCHE_DIR2)
#define LEFT_BRAKE				PORTD = (PORTD & ~(LEFT_ENGINE_MASK))|(1<<MOTEUR_GAUCHE_DIR1)|(1<<MOTEUR_GAUCHE_DIR2)
//Defines for controlling calibration
#define MOTEUR_CALIB			4
#define CALIBRATION_MASK		(1<<MOTEUR_CALIB)
#define SET_CALIBRATION			PORTA = (PORTA & ~(CALIBRATION_MASK))|(1<<MOTEUR_CALIB)
#define CLR_CALIBRATION			PORTA = (PORTA & ~(CALIBRATION_MASK))|(0<<MOTEUR_CALIB)

//Defines for ADC mux switching
#define	SWITCH_MUX_CHANNEL_0	ADMUX &= ~(1<<MUX0)
#define SWITCH_MUX_CHANNEL_1	ADMUX |= (1<<MUX0)

//Define for adc sign reading
#define RIGHT_DIR_SIGN		PINA&0b00001000		//bit 3
#define LEFT_DIR_SIGN		PINA&0b00000100		//bit 2

/* ADC globals */
static volatile uint8_t 	LEFT_counter = 0;	//left engine adc sample counter variable
static volatile uint16_t	LEFT_ADCvalue = 0;
static volatile uint8_t 	RIGHT_counter = 0;	//right engine adc sample counter variable
static volatile uint16_t	RIGHT_ADCvalue = 0;

//Calibration arrays
static volatile uint16_t	right_cal[4] = {0,0,1023,1023};
static volatile uint16_t	left_cal[4]= {0,0,1023,1023};

void ADC_init(void);
void ADC_calibration(void);

#endif