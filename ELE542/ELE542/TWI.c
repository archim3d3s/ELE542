/*************************************************************************/
/* Fichier:			 TWI.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#include "TWI.h"

//					Fonction
//No. registre  En lecture		En �criture
//     0        # de r�vision	  Commande
//     1		 inutilis�		  Gain max
//     2	    Mesure - MSB	   Port�e
//     3		Mesure - LSB	  inutilis�

uint8_t TWIdataCounter = 0;

/******************************************************************************
 *
 *****************************************************************************/
ISR(TWI_vect) {
	
	uint8_t* ptr;
	uint8_t status  = TWSR & 0xF8;
		
	switch (status) {
		case	0x08: /* Start Condition */
		case	0x10: /* Restart Condition */
			
			// Load data register with TWI slave address
			TWDR = getDataOutBuf();
			// TWI Interrupt enabled and clear flag to send next byte
			TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
			
			break;

		case	0x18: /* Address Write Ack */
		case	0x28: /* Data Write Ack */
		case	0x30: /* Date Write NoAck */

			
			if (TWIdataCounter>0) /*Si le buffer n'est pas vide*/
			{
				/* Nouveau packet*/
				if(CircularBufferOut[CircularBufferOutIndex+1] >= 0xE0)
				{
					//Generate restart condition on the bus to start next transmission
					TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
				}
				/*Suite du packet en cours de transmission*/
				else
				{
					//Load TWDR with data byte to send
					TWDR = getDataOutBuf();
					// TWI Interrupt enabled and clear flag to send next byte
					TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
				}
			}
			/*Fin */
			else
			{
				//Generate STOP condition
				TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
			}
			
			
			break;

		case	0x50: /* Data Read Ack */
		case	0x58: /* Data Read NoAck */

			ptr = getDataInBuf(); /*R�cup�rer la lecture sur le bus*/
			*ptr = TWDR;

		case	0x40: /* Address Read Ack */

			if(TWIdataCounter > 0) /*Si le buffer n'est pas vide*/
			{
				if(CircularBufferOut[CircularBufferOutIndex+1] >= 0xE0)
				{
					//Generate restart condition on the bus to start next transmission
					TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
				}
				else
				{
					//Load TWDR with data byte to send
					TWDR = getDataOutBuf();
					// TWI Interrupt enabled and clear flag to send next byte
					TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
				}
			}
			else
			{
				//Generate STOP condition
				TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
			}
	
			break;

		case	0x48: /* Address Read NoAck */
		case	0x20: /* Address Write NoAck */


			while((CircularBufferOut[CircularBufferOutIndex+1] <= 0xE0) && (TWIdataCounter > 0))
			{
				getDataOutBuf(); /*Sauter au prochain packet dans le buffer*/
			}
			
			if(TWIdataCounter >0)
			{
				//Generate restart condition on the bus to start next transmission
				TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
			}
			
			else
			{
				//Generate STOP condition
				TWCR = (1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);				
			}
			break;

		default : 
			break;
	}
}

void initTWI(void)
{
	// Mode d�op�ration  
	//  - Traitement par interruption 
	//  - fTWI = 10 kHz
	//  - Mode ma�tre
	//  - pas d�adresse � esclave �
	
	//SCL freq = FCPU/ (16 + 2(TWBR) * 4^TWPS) [datasheet p.173]
	//Avec prescaler = 1 (TWPS1=0 TWPS0=0), TWBR = 198 = 0xC6 
	TWBR = 0xC6;
	TWSR = ~((1<<TWPS1)|(1<<TWPS0));
	TWDR = 0xFF;
	TWCR = (0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(0<<TWIE);
}

/******************************************************************************
 * Ins�rer dans le buffer out
 *****************************************************************************/
void putDataOutBuf(uint8_t data){

	CircularBufferOutEnd++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	TWIdataCounter++;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


/******************************************************************************
 * Retirer du buffer out
 *****************************************************************************/
uint8_t getDataOutBuf(void){

	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	TWIdataCounter--;
	return (uint8_t)CircularBufferOut[CircularBufferOutIndex];

}


/******************************************************************************
 * Ins�rer dans le buffer in
 *****************************************************************************/
void putDataInBuf(uint8_t* ptr){

	CircularBufferInEnd++;
	CircularBufferInEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferIn[CircularBufferInEnd] = ptr;

}


/******************************************************************************
 * Retirer du buffer in
 *****************************************************************************/
uint8_t* getDataInBuf(void){

	CircularBufferInIndex++;
	CircularBufferInIndex %= CIRCULAR_BUFFER_SIZE;
	return CircularBufferIn[CircularBufferInIndex];	
}


/******************************************************************************
 * �crire sur le bus twi
 *****************************************************************************/
void twiWrite(uint8_t address, uint8_t registre, uint8_t data)
{	
	cli();

	/* Ajouter les 3 donnees a envoyer sur le bus au buffer circulaire */
	putDataOutBuf(address);
	putDataOutBuf(registre);
	putDataOutBuf(data);
	
	sei();
	
	if(~(TWCR & (1<<TWIE)))
	{
		TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
	}
}

/******************************************************************************
 * lire sur le bus
 *****************************************************************************/
void twiRead(uint8_t address, uint8_t registre, uint8_t *ptr)
{
	cli();
	putDataOutBuf(address-1);
	putDataOutBuf(registre);
	putDataOutBuf(address);
	putDataInBuf(ptr);
	
	sei();
	
	if(~(TWCR & (1<<TWIE)))
	{
		TWCR = (1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC)|(1<<TWEN)|(1<<TWIE);
	}
}
/*************************************************************************/