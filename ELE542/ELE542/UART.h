#ifndef UART_H
#define UART_H

#include "main.h"
#include <util/atomic.h>

#define UART_BAUD_RATE   9600    
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

/* uart globals */
static volatile uint8_t *uart_data_ptr;
static volatile uint8_t uart_out_counter;
static volatile uint8_t uart_in_counter;

void debug_send(uint8_t *buf, uint8_t size);
void initUART(void);

static volatile uint8_t commande[3];

static volatile uint8_t flag_CMD_in = 0;
static volatile uint8_t flag_echo_EN = 1;
static volatile uint8_t flag_trans_ON = 0;

#endif