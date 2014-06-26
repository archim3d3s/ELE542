#ifndef PWM_H
#define PWM_H

#include "main.h"

#define PWM_GAUCHE	OCR1B
#define PWM_DROIT	OCR1A

static volatile uint8_t flag_PWMcycle = 0;
static volatile uint8_t flag_50ms_TICK = 0;

static volatile uint8_t TICK_counter = 0;

void initTIMER();

#endif