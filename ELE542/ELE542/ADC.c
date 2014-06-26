#include "ADC.h"
#include <util/delay.h>

ISR (ADC_vect)
{
	//Cette routine lit la valeur sur le canal ADC actif et fait la moyenne
	//à l'aide du compteur de lecture	
	if(ADMUX)
	{
		RIGHT_counter++;
		RIGHT_ADCvalue += ADC;
	}
	else
	{
		LEFT_counter++;
		LEFT_ADCvalue += ADC;
	}
	//Alterner le canal actif entre le canal 0 et le canal 1
	ADMUX ^= 1;
}

void DIR_bit_ON (uint8_t direction)	{PORTD |= (1<<direction);}
void DIR_bit_OFF(uint8_t direction)	{PORTD &= ~(1<<direction);}

void ADC_init(void)
{
    /*
	  - Fréquence de l’horloge de l’ADC = 125 kHz
	  - Mode d’opération = Freerun
	  - Référence de voltage = AREF
	  - Mode de traitement des acquisitions = Interruption
    */	
	
    // AREF-interne et Vref desactivée, Résultat ajusté à droite. [Datasheet p.212]
	ADMUX = (0<<REFS1)|(0<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);

    // Active l'ADC, démarre la conversion, active les interruptions et prescaler de 128 pour f=125kHz
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADATE);

	// Mode d'opération free-run [voir datasheet p.216]
	SFIOR |= (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
	
	//Configure port A with pin A4 as an output, others are inputs
	DDRA |= (1<<PA4);
	
	//Configure PORT D with pins 2,3,6,7 as outputs for motor directions control
	DDRD |= (1<<PD2)|(1<<PD3)|(1<<PD6)|(1<<PD7); 
}

void motor_calibration(void)
{
	//CAL DIR2 DIR1 PWM Mesure
	// 1   0    1    0   Vmax+
	// 0   0    1    0   Vzero+
	// 0   1    0    0   Vzero-
	// 1   1    0    0   Vmax-
	
	//Les deux PWM doivent être maintenus à zéro pendant la procédure de calibration.
	PWM_GAUCHE = 0;
	PWM_DROIT  = 0;
	
	//Mesure de Vmax + pour chaque moteur
	CAL_bit_ON();
	DIR_bit_OFF(RIGHT_DIR2); DIR_bit_OFF(LEFT_DIR2); 
	DIR_bit_ON (RIGHT_DIR1); DIR_bit_ON (LEFT_DIR1);
	
	resetADC();
	
	while(RIGHT_counter < CALIB_SAMPLE_NB && LEFT_counter < CALIB_SAMPLE_NB);
	
	RIGHT_Vmax_pos = RIGHT_ADCvalue / RIGHT_counter;
	LEFT_Vmax_pos  = LEFT_ADCvalue / LEFT_counter;
	
	//Mesure de Vzero + pour chaque moteur
	CAL_bit_OFF();	

	resetADC();
	
	while(RIGHT_counter < CALIB_SAMPLE_NB && LEFT_counter < CALIB_SAMPLE_NB);
		
	RIGHT_Vzero_pos = RIGHT_ADCvalue / RIGHT_counter;
	LEFT_Vzero_pos  = LEFT_ADCvalue / LEFT_counter;	
	
	//Mesure de Vzero - pour chaque moteur
	
	DIR_bit_OFF(RIGHT_DIR1); DIR_bit_OFF(LEFT_DIR1);
	DIR_bit_ON (RIGHT_DIR2); DIR_bit_ON (LEFT_DIR2);
	
	resetADC();
	
	while(RIGHT_counter < CALIB_SAMPLE_NB && LEFT_counter < CALIB_SAMPLE_NB);
	
	RIGHT_Vzero_neg = RIGHT_ADCvalue / RIGHT_counter;	
	LEFT_Vzero_neg  = LEFT_ADCvalue / LEFT_counter;	
	
	//Mesure de Vmax - pour chaque moteur
	
	CAL_bit_ON();
	
	resetADC();
	
	while(RIGHT_counter < CALIB_SAMPLE_NB && LEFT_counter < CALIB_SAMPLE_NB);
	
	RIGHT_Vmax_neg = RIGHT_ADCvalue / RIGHT_counter;
	LEFT_Vmax_neg  = LEFT_ADCvalue / LEFT_counter;
	
	resetADC();
	CAL_bit_OFF();
}

void resetADC(void)
{
	cli();
	LEFT_counter   = 0;
	LEFT_ADCvalue  = 0;
	RIGHT_counter  = 0;
	RIGHT_ADCvalue = 0;
	sei();
}