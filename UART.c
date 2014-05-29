#include <UART.h>

void UARTinit(void)
{
    /* enable RxD/TxD and ints */
    outp((1<<RXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2),UCSRB);       
    /* configure asynchronous operation, no parity, 1 stop bit, 8 data bits, Tx on rising edge */
    outp((1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL),UCSRC);       
    /* set baud rate */
    outp((u08)(UART_BAUD_SELECT >> 8), UBRRH);          
    outp((u08)(UART_BAUD_SELECT & 0x00FF), UBRRL);          
    uart_txd_in_ptr  = uart_txd_out_ptr = uart_txd_buffer;
    uart_rxd_in_ptr  = uart_rxd_out_ptr = uart_rxd_buffer;
    uart_txd_buf_cnt = 0;
    uart_rxd_buf_cnt = 0;
}
