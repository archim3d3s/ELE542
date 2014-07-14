/*
 * TWI.h
 *
 * Created: 6/25/2014 4:07:56 PM
 *  Author: JP
 */ 

#ifndef TWI_H_
#define TWI_H_

#include "main.h"

#define CIRCULAR_BUFFER_SIZE 32

//						Adresse
//               En lecture  En écriture
// Sonar gauche      0xE1       0xE0
// Sonar droit		 0xE3	    0xE2
#define SONAR_G_R	0xE1
#define SONAR_G_W	0xE0
#define SONAR_D_R	0xE3
#define SONAR_D_W	0xE2

/** Prototype */
void initTWI(void);
void putDataOutBuf(uint8_t data);
uint8_t getDataOutBuf(void);
void putDataInBuf(uint8_t* ptr);
uint8_t* getDataInBuf(void);

void twiWrite(uint8_t address, uint8_t registre, uint8_t data);
void twiRead(uint8_t address, uint8_t registre, uint8_t *data);

/** Les variables */
uint8_t CircularBufferOut[CIRCULAR_BUFFER_SIZE];
uint8_t* CircularBufferIn[CIRCULAR_BUFFER_SIZE];

uint8_t CircularBufferOutEnd;
uint8_t CircularBufferOutIndex;
uint8_t CircularBufferInEnd;
uint8_t CircularBufferInIndex;

#endif /* TWI_H_ */