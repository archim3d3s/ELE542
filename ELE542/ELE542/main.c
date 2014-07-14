#include "main.h"

extern volatile uint8_t flag_CMD_in;
extern volatile uint8_t flag_echo_EN;
extern volatile uint8_t flag_trans_ON;

extern volatile uint8_t commande[3];

extern volatile uint8_t flag_PWMcycle;
extern volatile uint8_t flag_50ms_TICK;

volatile uint8_t		flag_newStatus = 0;

int main(void)
{
	//INITIALISATION
    initUART();
	initTIMER();
	initLED();
	initTWI();

    sei();   /* enable interrupts */
	uint8_t robotState = 0;
	
	uint8_t rawSpeed = 0;
	uint8_t rawAngle = 0;
	float speed = 0.0;
	float angle = 0.0;
	
	uint8_t LEDstatus = (1<<ATTENTE);
	
	uint8_t PINGside = 0;
	uint8_t sonarLSB = 0;
	uint8_t sonarMSB = 0; 
	
	float RIGHT_ADCmean = 0.0;
	float LEFT_ADCmean = 0.0;
	
	float RIGHT_pwm = 0.0;
	float LEFT_pwm = 0.0;
	
	//CONFIGURATION DES SONARS
	//Gain 8 a 12
	twiWrite(SONAR_G_W,0x01,0x0A);
	twiWrite(SONAR_D_W,0x01,0x0A);
	
	//Portee 1.5 a 3 m
	twiWrite(SONAR_G_W,0x02,0x2D);
	twiWrite(SONAR_D_W,0x02,0x2D);
	
	//ROUTINE PRINCIPALE	
    while(1)
	{ 
		//Interrupteur de mise en marche et d'arret d'urgence
		if (robotState == 0 && SW6)
		{
			robotState = 1;
			
			LEDstatus |= (1<<ACTIF);
			LEDstatus &= ~(1<<ATTENTE);
		}
		
		if (robotState == 1)
		{
			if (SW7)
			{
				robotState = 0;
				
				LEDstatus |= (1<<ATTENTE);
				LEDstatus &= ~(1<<ACTIF);
			}
			
			else
			{	
				//Reception commande
				if(flag_CMD_in)
				{
					//Convertir la trame en commande pour les moteurs
					if(commande[0] == 0xF0)
					{
						//Arret obligatoire
					}
			
					else if (commande[0] == 0xF1)
					{
						//Commande normale
						rawSpeed = commande[1];
						rawAngle = commande[2];
					}
					LEDstatus |= (1<<COMMANDE_RECUE);
					flag_CMD_in = 0;
				}
		
				if(flag_50ms_TICK)
				{
					//Demarrer PING sur un sonar
					//Recuperer mesure sur l'autre sonar
					//Alterner entre sonar gauche et droit
			
					if(PINGside == 1)
					{
						//PING
						twiWrite(SONAR_G_W, 0x00, 0x51);
						
						LEDstatus |= (1<<PING_SONAR_G);
						LEDstatus &= ~(1<<PING_SONAR_D);
						//MESURE
						twiRead (SONAR_D_R, 0x02, &sonarMSB);
						twiRead (SONAR_D_R, 0x03, &sonarLSB);
						//ALTERNER
						PINGside = 0;
					}
			
					else
					{
						//PING
						twiWrite(SONAR_D_W,0x00,0x51);
						
						LEDstatus |= (1<<PING_SONAR_D);
						LEDstatus &= ~(1<<PING_SONAR_G);
						//MESURE
						twiRead (SONAR_G_R, 0x02, &sonarMSB);
						twiRead (SONAR_G_R, 0x03, &sonarLSB);				
						//ALTERNER
						PINGside = 1;
					}
				}
		
				//if(flag_obstacle)
				//{
					//Changer le comportement du robot pour eviter la collision 
				//}
		
				if(flag_PWMcycle)
				{	
					speed = ((float)rawSpeed-100.0)/100.0;
					angle = ((float)rawAngle*2.0/360.0) *2.0*Pi;
			
					//Convertir valeur de l'ADC en valeur de vitesse
					getADCmeanValues(&RIGHT_ADCmean, &LEFT_ADCmean);
			
					//Calculer nouvelles valeurs de PWM
					CalculPWM(speed, angle, LEFT_ADCmean, RIGHT_ADCmean,  &RIGHT_pwm, &LEFT_pwm);
			
					//Mettre a jour les PWM et bits de direction
				}
		
				if (flag_newStatus)
				{
					//Afficher le nouveau statut sur les LED
					updateLED(LEDstatus);
				}
			}
		}
	}            
}
