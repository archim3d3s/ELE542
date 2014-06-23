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

void reset_ADC(void)
{
	RIGHT_counter = 0;
	LEFT_counter = 0;
	RIGHT_ADCvalue = 0;
	LEFT_ADCvalue = 0;
}

/*
 * Function Enabled_ADC
 * Desc     enabled the adc
 * Input    none
 * Output   none
 */
void Enable_ADC(void)
{
	ADCSRA |= (1<<ADIE);
}

/*
 * Function Desable_ADC
 * Desc     desable the adc
 * Input    none
 * Output   none
 */
void Disable_ADC(void)
{
	ADCSRA &= ~(1<<ADIE);
}

/*
 * Function GetAdcValues
 * Desc     Calculate the adc left motor and the right motor
 * Input    ptr_LEFT_ADCvalue : ptr to store the adc left motor
 *          ptr_RIGHT_ADCvalue : ptr to store the adc right motor
 * Output   none
 */
void GetAdcValues(float * ptr_LEFT_ADCvalue, float * ptr_RIGHT_ADCvalue)
{
	int16_t work1,work2;

	// disabled the interrupts to make sure that adcX and adcX_counter won't be modified
	cli();

	// store the value of the average
	work1 = RIGHT_ADCvalue/RIGHT_counter;
	work2 = LEFT_ADCvalue/LEFT_counter;

	// reset value for the next sampleLEFT_counter = 0;
	RIGHT_counter = 0;
	LEFT_ADCvalue = 0;
	RIGHT_ADCvalue = 0;
	sei();

	//find witch sign LEFT_ADCvalue and RIGHT_ADCvalue should have
	if(LEFT_DIR_SIGN)
	{
        // check limits
		if(work1 <= left_cal[LOW_SPEED_NEG])
			work1 = 0;
		if(work1 >= left_cal[HIGH_SPEED_NEG])
			work1 = 1023;
		work1 = ~work1;		//invert sign of value
        // put into percentage
		*ptr_LEFT_ADCvalue = (float)work1/1023;
	}
	else
	{
        // check limits
		if(work1 <= left_cal[LOW_SPEED_POS])
			work1 = 0;
		if(work1 >= left_cal[HIGH_SPEED_POS])
			work1 = 1023;
        // put into percentage
		*ptr_LEFT_ADCvalue = (float)work1/1023;
	}

	if(RIGHT_DIR_SIGN)
	{
		// check limits
		if(work2 <= right_cal[LOW_SPEED_NEG])
			work2 = 0;
		if(work2 >= right_cal[HIGH_SPEED_NEG])
			work2 = 1023;
		work2 = ~work2;		//invert sign of value
		// put into percentage
		*ptr_RIGHT_ADCvalue = (float)work2/1023;
	}
	else
	{
	    // check limits
		if(work2 <= right_cal[LOW_SPEED_POS])
			work2 = 0;
		if(work2 >= right_cal[HIGH_SPEED_POS])
			work2 = 1023;
        // put into percentage
        *ptr_RIGHT_ADCvalue = (float)work2/1023;
	}
}

/*
 * Function Adc_calibrate
 * Desc     ADC calibration for drive control
 * Input    none
 * Output   none
 */
void Adc_calibrate(void)
{
	uint8_t loop;

	//set adc for free run without auto-triggerring and no interrupt
	ADCSRA = (1<<ADEN) | (0<<ADSC) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1)  | (1<<ADPS0) | (0<<ADATE);
	
	// make sure PWM outputs are at 0 for calibration sequence
	RIGHT_MOTOR = 0;
	LEFT_MOTOR = 0;


    // set calibration to read in foward/reverse mode
	SET_CALIBRATION;
	RIGHT_FOWARD;
	LEFT_FOWARD;

	_delay_ms(STABLISE_DELAY);		//delay for stabilisation

	reset_ADC();

    // 25 read
	for(loop = 0; loop < 25; loop++)
	{

		SWITCH_MUX_CHANNEL_1;               // motor right
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		RIGHT_ADCvalue+= ADC;                         // read
		RIGHT_counter++;

		SWITCH_MUX_CHANNEL_0;               // motor left
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		LEFT_ADCvalue+= ADC;                         // read
		LEFT_counter++;
	}
	// Store results while making the average
	right_cal[HIGH_SPEED_POS] = RIGHT_ADCvalue/RIGHT_counter;
	left_cal[HIGH_SPEED_POS] = LEFT_ADCvalue/LEFT_counter;


    // clear to read in neutral mode
	CLR_CALIBRATION;
	RIGHT_NEUTRAL;
	LEFT_NEUTRAL;

	_delay_ms(STABLISE_DELAY);		//delay for stabilisation

	// clear to make an other read
	clr_adc_cal();

    // 25 read
	for(loop = 0; loop < 25; loop++)
	{
		SWITCH_MUX_CHANNEL_1;               // motor right
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		RIGHT_ADCvalue+= ADC;                         // read value
		RIGHT_counter++;

		SWITCH_MUX_CHANNEL_0;               // motor left
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		LEFT_ADCvalue+= ADC;                         // read value
		LEFT_counter++;
	}
	// Store results while making the average
	right_cal[LOW_SPEED_POS] = RIGHT_ADCvalue/RIGHT_counter;
	left_cal[LOW_SPEED_POS] = LEFT_ADCvalue/LEFT_counter;

	RIGHT_NEUTRAL;
	LEFT_NEUTRAL;

	_delay_ms(STABLISE_DELAY);		//delay for stabilisation

	clr_adc_cal();

	// 25 read
	for(loop = 0; loop < 25; loop++)
	{
		SWITCH_MUX_CHANNEL_1;               // motor right
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		RIGHT_ADCvalue+= ADC;                         // read value
		RIGHT_counter++;

		SWITCH_MUX_CHANNEL_0;               // motor left
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		LEFT_ADCvalue+= ADC;                         // read value
		LEFT_counter++;
	}
	// Store results while making the average
	right_cal[LOW_SPEED_NEG] = RIGHT_ADCvalue/RIGHT_counter;
	left_cal[LOW_SPEED_NEG] = LEFT_ADCvalue/LEFT_counter;

    // set calibration to read in foward/reverse mode
	SET_CALIBRATION;
	RIGHT_REVERSE;
	LEFT_REVERSE;

	_delay_ms(STABLISE_DELAY);		//delay for stabilisation

	clr_adc_cal();

    // 25 read
	for(loop = 0; loop < 25; loop++)
	{

		SWITCH_MUX_CHANNEL_1;               // motor right
		ADCSRA |= (1<<ADSC);                // start conversion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		RIGHT_ADCvalue+= ADC;                         // read value
		RIGHT_counter++;

		SWITCH_MUX_CHANNEL_0;               // motor left
		_delay_ms(1);
		ADCSRA |= (1<<ADSC);                // start converion
		while( (ADCSRA & (1<<ADSC)) == 1);	//wait till convertion finishes
		LEFT_ADCvalue+= ADC;                         // read value
		LEFT_counter++;
	}
	// Store results while making the average
	right_cal[HIGH_SPEED_NEG] = RIGHT_ADCvalue/RIGHT_counter;
	left_cal[HIGH_SPEED_NEG] = LEFT_ADCvalue/LEFT_counter;


	//calibration values are now stored!
	CLR_CALIBRATION;
	RIGHT_NEUTRAL;
	LEFT_NEUTRAL;
	
	ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1)  | (1<<ADPS0) | (1<<ADATE);
	reset_ADC();
}