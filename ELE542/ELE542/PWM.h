#ifndef PWM_H
#define PWM_H

#include "ADC.h"

#define Pi      3.1415926535897932384626
#define RAYON   9.525
#define TS      0.005
#define Vmax    80.0
#define Tau     0.5

#define H11		3.90148347975678
#define H12		4.90148347975678

#define H21		1.1613504
#define H22		0.5806746734

#define Fpwm	10000

//Defines for pwm module
#define	MAX_PWM_VALUE	9999		//16Mhz /(8(prescaler)*200(freq PWM))-1
#define	RIGHT_MOTOR		OCR1A	
#define LEFT_MOTOR		OCR1B

void CalculPWM(float Vitesse_D, float Angle_D, float Vg, float Vd, float *Duty_G, float *Duty_D);

/*
 * Function SetPwmMoteur
 * Desc     Sets new pwm value on pwm modules and controls the direction of rotation of each wheels
 * Input    float value in % (-1.0 to 1.0) of the speed of the left and right wheel 
 * Output   none
 */
void SetPwmMoteur(float dG, float dD);

#endif