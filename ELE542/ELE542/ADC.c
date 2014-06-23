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
}

void ADC_calibration(void)
{
	
}