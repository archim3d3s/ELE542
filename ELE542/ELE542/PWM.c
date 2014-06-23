#include "PWM.h"

void CalculPWM(float Vitesse_D, float Angle_D, float Vg, float Vd, float *Duty_G, float *Duty_D)
{

	/**
		Dans cette fonction, la valeur des duty cycle pour chaque moteur est calculé.  
		Ce calcul est effectué à l`aide de la vitesse désirée, de l`angle désriré ainsi 
		que de la vitesse (mesurée avec CalculVitesses) et l`angle actuel.
	*/
	static float Angle = 0.0;
	static float ErreurAngle = 0.0;
	static float W = 0.0;
	static float Old_W = 0.0;
	static float Vt     = 0.0;
	static float Ut     = 0.0;
	static float Ua     = 0.0;
	static int 	 Signe_Ua = 0;
	static int 	 Signe_Ut = 0;


	Vg = (Vg > 1.0) ? 1.0 : ((Vg < -1.0) ? -1.0 : Vg);  /* Regarde les limites (-1.0 à 1.0) */
	Vd = (Vd > 1.0) ? 1.0 : ((Vd < -1.0) ? -1.0 : Vd);  /* Regarde les limites (-1.0 à 1.0) */

	Old_W = W;
	W     = 0.5*(Vmax/RAYON)*(Vd-Vg);
	Vt    = 0.5*(Vd+Vg);

	Angle = Angle + (0.5)*TS*(W+Old_W);
	Angle = (Angle > 2.0*Pi) ? Angle - 2*Pi : ((Angle < 0.0) ? Angle + 2*Pi : Angle); /* Angle entre 0 et 2 pi */
	ErreurAngle = ((Angle_D >= Pi + Angle) ? Angle_D - 2*Pi : ((Angle_D <= -Pi + Angle) ? Angle_D + 2*Pi : Angle_D))-Angle;

	Ut = -H11*Vt + H12*Vitesse_D;
	Ua = H21*ErreurAngle - H22*W;

	Signe_Ua = (Ua >= 0.0)   ? 1 : -1;
	Signe_Ut = (Ut >= 0.0) ? 1 : -1;

	Ua = (Signe_Ua*Ua > 1.0) ? Signe_Ua*1.0 : ((Signe_Ua*Ua <= 0.05) ? 0.0 : Ua);
	Ut = (Signe_Ut*Ut > 1.0) ? Signe_Ut*1.0 : Ut;
	Ut = ((Signe_Ut*Ut) > (1.0 - Signe_Ua*Ua)) ? Signe_Ut*(1.0 - Signe_Ua*Ua) : Ut;


	*Duty_D = (Ut+Ua);
	*Duty_G = (Ut-Ua);

	*Duty_D = (*Duty_D > 0.99) ? 0.99 : ((*Duty_D < -0.99) ? -0.99 : *Duty_D);
	*Duty_G = (*Duty_G > 0.99) ? 0.99 : ((*Duty_G < -0.99) ? -0.99 : *Duty_G);	
}


/*
 * Function SetPwmMoteur
 * Desc     Sets new pwm value on pwm modules and controls the direction of rotation of each wheels
 * Input    float value in % (-1.0 to 1.0) of the speed of the left and right wheel 
 * Output   none
 */
void SetPwmMoteur(float dG, float dD)
{
	//check if the requested speed is positive or negative for left wheel
	if(dG > 0)
	{
		LEFT_FOWARD;
	}
	else if(dG < 0)
	{
		LEFT_REVERSE;
		dG *= -1;
	}

	//check if the requested speed is positive or negative for right wheel
	if(dD > 0)
	{
		RIGHT_FOWARD;
	}
	else if(dD < 0)
	{
		RIGHT_REVERSE;
		dD *= -1;
	}

	RIGHT_MOTOR = dD*MAX_PWM_VALUE;	//set new pwm duty cycle
	LEFT_MOTOR = dG*MAX_PWM_VALUE;	//set new pwm duty cycle
}