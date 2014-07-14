/*************************************************************************/
/* Fichier:			 PWM.H												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#ifndef PWM_H
#define PWM_H

#include "main.h"

#define PWM_GAUCHE	OCR1B
#define PWM_DROIT	OCR1A

#define PWM_TOP 0x04E1

void initTIMER();

/*************************************************************************/
#endif
/*************************************************************************/