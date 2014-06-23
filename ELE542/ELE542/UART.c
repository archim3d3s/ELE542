#include "uart.h"

volatile int i = 0;
volatile int echo_en = 0;
volatile int trans_flag = 0;
volatile int commande[3];

ISR(USART_TXC_vect)      
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

ISR(USART_RXC_vect)      
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

void debug_send(uint8_t *buf, uint8_t size)
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

void initUART(void)
{
    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
    UCSRC = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);   
    /* enable RxD/TxD and ints */
    UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);       
    /* set baud rate */
    UBRRH = (uint8_t)(UART_BAUD_SELECT >> 8);         
    UBRRL = (uint8_t)(UART_BAUD_SELECT & 0x00FF);          
}
