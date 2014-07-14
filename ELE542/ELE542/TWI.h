/*************************************************************************/
/* Fichier:			 TWI.H												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef TWI_H_
#define TWI_H_

#include "main.h"

#define CIRCULAR_BUFFER_SIZE 32

//						Adresse
//               En lecture  En �criture
// Sonar gauche      0xE1       0xE0
// Sonar droit		 0xE3	    0xE2
#define SONAR_G_R	0xE1
#define SONAR_G_W	0xE0
#define SONAR_D_R	0xE3
#define SONAR_D_W	0xE2

/******************************* FONCTIONS *******************************/
void initTWI(void);
void putDataOutBuf(uint8_t data);
uint8_t getDataOutBuf(void);
void putDataInBuf(uint8_t* ptr);
uint8_t* getDataInBuf(void);

void twiWrite(uint8_t address, uint8_t registre, uint8_t data);
void twiRead(uint8_t address, uint8_t registre, uint8_t *data);

/******************************* VARIABLES *******************************/
uint8_t CircularBufferOut[CIRCULAR_BUFFER_SIZE];
uint8_t* CircularBufferIn[CIRCULAR_BUFFER_SIZE];

uint8_t CircularBufferOutEnd;
uint8_t CircularBufferOutIndex;
uint8_t CircularBufferInEnd;
uint8_t CircularBufferInIndex;

/*************************************************************************/
#endif /* TWI_H_ */
/*************************************************************************/