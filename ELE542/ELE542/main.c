/*************************************************************************/
/* Fichier:			 MAIN.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#include "main.h"

#define SW6  (PINA & (1<<PA6))
#define SW7  (PINA & (1<<PA7))

/****************** IMPORTATION DES VARIABLES EXTERNES *******************/
extern volatile uint8_t flag_CMD_in;
extern volatile uint8_t flag_echo_EN;
extern volatile uint8_t flag_trans_ON;

extern volatile uint8_t commande[3];

extern volatile uint8_t flag_PWMcycle;
extern volatile uint8_t flag_50ms_TICK;

uint8_t debugStr[30] = {0};
	
/**************************** POINT D'ENTRÉE  ****************************/
int main(void)
{
/*************************** VARIABLES LOCALES ***************************/	
	uint16_t rawSpeed = 0;
	uint16_t rawAngle = 0;
	float speed = 0.0;
	float angle = 0.0;
	
	uint8_t LEDstatus = (1<<ATTENTE);
	
	uint8_t PINGside = 0;
	uint8_t sonarLSB = 0;
	uint8_t sonarMSB = 0;
	uint16_t sonarG = 0;
	uint16_t sonarD = 0;
	
	
	float RIGHT_ADCmean = 0.0;
	float LEFT_ADCmean = 0.0;
	
	float RIGHT_pwm = 0.0;
	float LEFT_pwm = 0.0;
	
	uint8_t robotState = 0;	
	
/**************************** INITIALISATION ****************************/
    initUART();
	initTIMER();
	initLED();
	initTWI();
	initADC();
	
/************************ CONFIGURATION DES SONARS  **********************/
	//Gain = 10
	twiWrite(SONAR_G_W,0x01,0x0A);
	twiWrite(SONAR_D_W,0x01,0x0A);
	
	//Portée = 200cm
	twiWrite(SONAR_G_W,0x02,0x2D);
	twiWrite(SONAR_D_W,0x02,0x2D);
	
/************************ CALIBRATION DES MOTEURS ************************/
	motor_calibration();

/************************** ROUTINE PRINCIPALE ***************************/
    while(1)
	{ 
		
/*********** INTERRUPTEURS DE MISE EN MARCHE ET ARRET D'URGENCE **********/
		if (robotState == 0 && !SW6)
		{
			robotState = 1;
			LEDstatus = 0;
			LEDstatus |= (1<<ACTIF);
			LEDstatus &= ~(1<<ATTENTE);
			
			sei();	/* enable interrupts */
		}
		
		if (robotState == 1)
		{
			if (!SW7)
			{
				robotState = 0;
				
				LEDstatus = (1<<ATTENTE);
				LEDstatus &= ~(1<<ACTIF);

				PWM_GAUCHE = 0;
				PWM_DROIT = 0;
				
				cli();   /* disable interrupts */
			}
			
/************************* FONCTIONNEMENT NORMAL *************************/
			else
			{	
/************************ RÉCEPTION DE COMMANDES *************************/
				if(flag_CMD_in)
				{
					//Convertir la trame en commande pour les moteurs
					if(commande[0] == 0xF0)
					{
						//Arret obligatoire
						PWM_DROIT = 0;
						PWM_GAUCHE = 0;
						
						DIR_bit_ON(RIGHT_DIR1); DIR_bit_ON(RIGHT_DIR2);
						DIR_bit_ON(LEFT_DIR1);  DIR_bit_ON(LEFT_DIR2);
					}
			
					else if (commande[0] == 0xF1)
					{
						//Commande normale
						rawSpeed = commande[1];
						rawAngle = commande[2];

						speed = ((float)rawSpeed-100.0)/100.0;
						angle = ((float)(rawAngle*2))*2.0*Pi/360.0;
					}
					LEDstatus |= (1<<COMMANDE_RECUE);
					flag_CMD_in = 0;
				}
				
/******************************** SONARS *********************************/		
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
					
						sonarD = (sonarMSB<<8) & (sonarLSB & 0x00FF);
						
						//LED
						if (sonarD) {LEDstatus |= (1<<OBS_SONAR_D);}
						else        {LEDstatus &= ~(1<<OBS_SONAR_D);}
						
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
						
						sonarG = (sonarMSB<<8) & (sonarLSB & 0x00FF);
						
						//LED				
						if (sonarG) {LEDstatus |= (1<<OBS_SONAR_G);}
						else		{LEDstatus &= ~(1<<OBS_SONAR_G);}
						
						//ALTERNER
						PINGside = 1;
					}

					flag_50ms_TICK = 0;
				}
							
/************************* CONTRÔLE DES MOTEURS **************************/		
				if(flag_PWMcycle)
				{				
					//Convertir valeur de l'ADC en valeur de vitesse
					getADCmeanValues(&RIGHT_ADCmean, &LEFT_ADCmean);
					resetADC();
					
					if(angle>0)
					{
					//sprintf(debugStr, "%d %d", (uint16_t)(RIGHT_pwm*100), (uint16_t)(LEFT_pwm*100));
					//debug_send(debugStr, 30);
					}
					//Calculer nouvelles valeurs de PWM
					CalculPWM( speed, angle, LEFT_ADCmean, RIGHT_ADCmean,  
							   &LEFT_pwm, &RIGHT_pwm);

					//Mettre a jour les PWM et bits de direction
					
					/* PWM GAUCHE */
					if(LEFT_pwm < 0)		/* MARCHE ARRIÈRE */
					{
						DIR_bit_OFF(LEFT_DIR1); DIR_bit_ON(LEFT_DIR2);
						PWM_GAUCHE = (uint16_t)(-1.0*LEFT_pwm*(float)PWM_TOP);
					}

					else if(LEFT_pwm > 0)	/*  MARCHE AVANT */
					{
						DIR_bit_ON(LEFT_DIR1); DIR_bit_OFF(LEFT_DIR2);
						PWM_GAUCHE = (uint16_t)(LEFT_pwm *(float)PWM_TOP); 
					}

					else					/*    NEUTRE     */
					{
						DIR_bit_OFF(LEFT_DIR1); DIR_bit_OFF(LEFT_DIR2);
						PWM_GAUCHE = 0;
					}
					
					/*PWM DROIT*/
					if(RIGHT_pwm < 0)		/* MARCHE ARRIÈRE */
					{
						DIR_bit_OFF(RIGHT_DIR1); DIR_bit_ON(RIGHT_DIR2); 
						PWM_DROIT = (uint16_t)(-1.0*RIGHT_pwm*(float)PWM_TOP); 
					}

					else if(RIGHT_pwm > 0)  /*  MARCHE AVANT */
					{
						DIR_bit_ON(RIGHT_DIR1); DIR_bit_OFF(RIGHT_DIR2);
						PWM_DROIT = (uint16_t)(RIGHT_pwm * (float)PWM_TOP);  
					}

					else					/*    NEUTRE     */
					{
						DIR_bit_OFF(RIGHT_DIR1); DIR_bit_OFF(RIGHT_DIR2);
						PWM_DROIT = 0;
					}

					flag_PWMcycle = 0;
				}
			}
		}

/************************** AFFICHAGE DU STATUT **************************/
		updateLED(LEDstatus);
		LEDstatus &= ~(1<<COMMANDE_RECUE); /* Reset la LED de CMD_IN */
	}            
}
/*************************************************************************/