/*
 * TWI.c
 *
 * Created: 6/25/2014 4:07:42 PM
 *  Author: JP
 */ 

//						Adresse
//               En lecture  En écriture
// Sonar gauche      0xE1       0xE0
// Sonar droit		 0xE3	    0xE2

//					Fonction
//No. registre  En lecture		En écriture
//     0        # de révision	  Commande
//     1		 inutilisé		  Gain max
//     2	    Mesure - MSB	   Portée
//     3		Mesure - LSB	  inutilisé

void initTWI(void)
{
	// Mode d’opération  
	//  - Traitement par interruption 
	//  - fTWI = 10 kHz
	//  - Mode maître
	//  - pas d’adresse « esclave »
	
	//SCL freq = FCPU/ (16 + 2(TWBR) * 4^TWPS) [datasheet p.173]
	//Avec prescaler = 1 (TWPS1=0 TWPS0=0), TWBR = 198 = 0xC6 
	TWBR = 0xC6;
	TWSR = ~((1<<TWPS1)|(1<<TWPS0));
	
	TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
}