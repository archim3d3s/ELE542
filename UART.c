/*
    Title:    AVR-GCC test program #4 for the STK200 eva board
    Author:   Volker Oth
    Date:     4/1999
    Purpose:  Uses the UART to communicate with a terminal program on the pc.
              The "tranceive complete" interrupt is used to send the string 
              "Serial Data from AVR receiced###" continuously to the pc.
              When a byte was received from the pc, the "receive complete"
              interrupt is called, which outputs the byte to PortB where the
              LEDs visualize the 8 bits it consists of.
              UART format: 9600 baud, 8bit, 1 stopbit, no parity
    needed
    Software: AVR-GCC to compile
    needed
    Hardware: ATS90S8515/8535/2313/mega(?) on STK200/300 board
    Note:     To contact me, mail to
                  volkeroth@gmx.de
              You might find more AVR related stuff at my homepage:
                  http://members.xoom.com/volkeroth
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Common.h"
#include "uart.h"
#include <util/atomic.h>

#define outp(val,port) port = val
#define inp(port) port

#ifndef F_CPU
#define F_CPU            16000000UL      /* C    rystal 16.000 Mhz */
#endif
#define UART_BAUD_RATE      9600      /* 9600 baud */


#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)



/* uart globals */
static volatile u08 *uart_data_ptr;
static volatile u08 uart_counter;
volatile int i = 0;
volatile int echo_en = 0;
volatile int trans_flag = 0;
volatile int commande[3];

SIGNAL(SIG_UART_TRANS)      
/* signal handler for uart txd ready interrupt */
{
	if (uart_counter)
	{
 		UDR = *uart_data_ptr; /*write byte to data buffer */
		uart_data_ptr++;
		uart_counter--;
	}
	else if(trans_flag==1 && uart_counter==0)
	{
		UDR=0xFF;
		trans_flag = 0;
	}
	else
	{			  
		echo_en = 1	;	
	}
}






SIGNAL(SIG_UART_RECV)      
/* signal handler for receive complete interrupt */
{
    
	
	char data =0;

	data = UDR;


/* vérification de la reception de commande et écriture*/
	if( i == 0  &&(data == 0xF1 || data == 0xF0))
	{	
 		commande[i] = data;
 		i++;
	}

	else if(i==1)
	{
 		commande[i] = data;
 		i++;
	}

	else if(i==2)
	{
 		commande[i] = data;
 		i=0;
	}

/* echo*/

	if( echo_en == 1)
	{
		UDR = data;
	}
}


/* fonction du gtctest4

void uart_send(u08 *buf, u08 size)
send buffer <buf> to uart 
{   
    if (uart_counter == 0) {
         //write first byte to data buffer 
        uart_data_ptr  = buf;
        uart_counter   = size;
        outp(*buf, UDR);
    }
}
*/

void debug_send(u08 *buf, u08 size)
/* send buffer <buf> to uart */
{   
     echo_en =0	;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
	 	 if (trans_flag == 0) 
		 {
	        uart_data_ptr  = buf;
	        uart_counter   = size;
	        UDR = 0xFE;
			trans_flag = 1;
	   	 }
	}
}


void uart_init(void)
/* initialize uart */
{
    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
    outp((1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL),UCSRC);       
    /* enable RxD/TxD and ints */
    outp((1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2),UCSRB);       
    /* set baud rate */
    outp((u08)(UART_BAUD_SELECT >> 8), UBRRH);          
    outp((u08)(UART_BAUD_SELECT & 0x00FF), UBRRL);          
}


