#ifndef UART_H
#define UART_H

/* set baud rate here */
#define UART_BAUD_RATE 9600
/* automatically calcuate baud register value */
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)

/* Commands*/
#define NORMAL        0xF1
#define FORCE_STOP    0xF0

volatile uint8_t UARTbyteCounter;
volatile uint8_t commandPacket[3];


void UARTinit(void);

#endif
