/*************************************************************************/
/* Fichier:			 PWM.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#include "PWM.h"
/************************** VARIABLES GLOBALES ***************************/	
volatile uint8_t flag_PWMcycle = 0;
volatile uint8_t flag_50ms_TICK = 0;

/*************************** VARIABLES LOCALES ***************************/	
static volatile uint8_t TICK_counter = 0;

/********************** Interrupt TIMER 1 overflow ***********************/
ISR(TIMER1_OVF_vect)
{	
	flag_PWMcycle = 1;
	
	TICK_counter++;
	
	if(TICK_counter == 10)
	{
		flag_50ms_TICK = 1;
		TICK_counter = 0;
	}
}
/*************************************************************************/

/****************************** INIT TIMER *******************************/
void initTIMER(void)
{
	// fPWM = 200 Hz 
	// PWMB = moteur gauche
	// PWMA = moteur droit 

	// Mode d�op�ration recommand� :
	//  - Interruption de d�bordement
	//  - Niveau haut au d�bordement
	//  - Niveau bas � la comparaison
	//  - Fast PWM
	
	// TOP = ICR1 = (f_clk)/(fPWM*prescaler) - 1 
	//    = (16000000)/(200*64) - 1 
	//    = 1249 = 0x04E1
	
	//	see [datasheet p.99]
	
	ICR1H = 0x04;
	ICR1L = 0xE1;
	
	TIMSK = 0x04;
	
	TCCR1A = (1<<COM1A1)|(0<<COM1A0)|(1<<COM1B1)|(0<<COM1B0)|
			 (0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);
			 
	TCCR1B = (1<<ICES1)|(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);	 	TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|			 (0<<CS12)|(1<<CS11)|(1<<CS10);			 	TIMSK  = (0<<TICIE1)|(0<<OCIE1A)|(0<<OCIE1B)|(1<<TOIE1);
}
/*************************************************************************/