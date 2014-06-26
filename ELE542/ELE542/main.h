#ifndef MAIN_H
#define MAIN_H 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <stdio.h>

#include "ADC.h"
#include "PWM.h"
#include "moteur.h"
#include "UART.h"
#include "LED.h"

#ifndef F_CPU
#define F_CPU            16000000UL      /* Crystal 16.000 Mhz */
#endif

#endif