#ifndef MAIN_H
#define MAIN_H 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <stdio.h>

#include "ADC.h"
#include "PWM.h"
#include "moteur.h"
#include "UART.h"
#include "LED.h"
#include "TWI.h"

#ifndef F_CPU
#define F_CPU            16000000UL      /* Crystal 16.000 Mhz */

#define SW6 (PORTA&(1<<PA6))
#define SW7 (PORTA&(1<<PA7))

#endif

#endif