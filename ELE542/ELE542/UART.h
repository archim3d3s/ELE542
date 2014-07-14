#ifndef UART_H
#define UART_H

#include "main.h"

#define UART_BAUD_RATE   9600    
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

void debug_send(uint8_t *buf, uint8_t size);
void initUART(void);

#endif