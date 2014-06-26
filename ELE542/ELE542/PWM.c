#include "PWM.h"

ISR(TIMER1_OVF_vect)
{	
	flag_PWMcycle = 1;
	
	TICK_counter++;
	
	if(TICK_counter == 10)
	{
		flag_50ms_TICK = 1;
	}
}

void initTIMER(void)
{
	// fPWM = 200 Hz 
	// PWMB = moteur gauche
	// PWMA = moteur droit 

	// Mode d’opération recommandé :
	//  - Interruption de débordement
	//  - Niveau haut au débordement
	//  - Niveau bas à la comparaison
	//  - Fast PWM
	
	//TOP = ICR1 = (f_clk)/(fPWM*prescaler) - 1 = (16000000)/(200*64) - 1 = 1249 = 0x04E1 [datasheet p.99]
	
	ICR1H = 0x04;
	ICR1L = 0xE1;
	
	TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);	TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(1<<CS10);	TIMSK  = (0<<TICIE1)|(0<<OCIE1A)|(0<<OCIE1B)|(1<<TOIE1);
}

