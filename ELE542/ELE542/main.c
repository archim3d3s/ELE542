#include "main.h"

int main(void)
{
	//INITIALISATION
    initUART();
	initTIMER();
	initLED();
	//initTWI();

    sei();   /* enable interrupts */
	
	//ROUTINE PRINCIPALE	
    while(1)
	{ 
		//Reception commande
		if(flag_CMD_in)
		{
			//Convertir la trame en commande pour les moteurs
		}
		
		if(flag_50ms_TICK)
		{
			//Demarrer PING sur un sonar
			//Recuperer mesure sur l'autre sonar
			//Alterner entre sonar gauche et droit
		}
		
		//if(flag_obstacle)
		//{
			//Changer le comportement du robot pour eviter la collision 
		//}
		
		if(flag_PWMcycle)
		{
			//Convertir valeur de l'ADC en valeur de vitesse
			//Calculer nouvelles valeurs de PWM
			//Mettre a jour les PWM et bits de direction
		}
		
		if (flag_newStatus)
		{
			//Afficher le nouveau statut sur les LED
		}
    }            
}
