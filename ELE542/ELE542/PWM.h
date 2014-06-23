#ifndef PWM_H
#define PWM_H

#include "main.h"

//     Bits de direction
// DIR2	DIR1 Sens de rotation
//  0    0		 Neutre
//  0    1	   Marche avant
//  1    0	  Marche arriere
//  1    1       Freins
static volatile uint8_t DIR1=0;
static volatile uint8_t DIR2=0;

static volatile uint8_t flag_PWMcycle = 0;
static volatile uint8_t flag_50ms_TICK = 0;

static volatile uint8_t TICK_counter = 0;

void initTIMER();

#endif