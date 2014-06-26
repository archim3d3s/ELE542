#include "uart.h"

ISR(USART_TXC_vect)      
{
	if (uart_out_counter)
	{
		UDR = *uart_data_ptr;
		uart_data_ptr++;
		uart_out_counter--;
	}
	else
	{
		UDR = 0xFF;
		flag_trans_ON = 0;
		flag_echo_EN = 1;
		UCSRB &= ~(1<<TXCIE);
	}
}

ISR(USART_RXC_vect)      
{
	uint8_t data;
	data = UDR;

	/* Reset WATCHDOG*/
	wdt_reset();
		
	if(data == 0xF0 && uart_in_counter == 0)
	{
		commande[uart_in_counter] = data;
		flag_CMD_in = 1;
	} 
	else if(data == 0xF1 && uart_in_counter == 0)
	{
		commande[uart_in_counter] == data;
		uart_in_counter++;
	}
	else
	{
 		commande[uart_in_counter] = data;
 		uart_in_counter++;
	}
	/* Reset counter*/
	if(uart_in_counter == 3)
	{
 		uart_in_counter = 0;
	}

	/* Send echo if enabled */
	if( flag_echo_EN == 1)
	{
		UDR = data;
	}
}

void debug_send(uint8_t *buf, uint8_t size)
{   
    flag_echo_EN =0	;
	UCSRB |= (1<<TXCIE);
	
 	if (flag_trans_ON == 0) 
	{
        uart_data_ptr  = buf;
	    uart_out_counter = size;
        flag_trans_ON = 1;
		UDR = 0xFE;
   	}
}

void initUART(void)
{
	//- vitesse = 9600 Baud 
	//- 1 bit d’arrêt 
	//- Mode : Interruption
	//- 8 bits de données 	//- pas de parité	
	flag_echo_EN = 1;
	uart_out_counter = 0;
	uart_in_counter = 0;
    UCSRC = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);
    UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);       
   
    /* set baud rate */
    UBRRH = (uint8_t)(UART_BAUD_SELECT >> 8);         
    UBRRL = (uint8_t)(UART_BAUD_SELECT & 0x00FF);
	
	/*Enable WATCHDOG with a 1s timer*/
	wdt_enable(WDTO_1S);          
}


