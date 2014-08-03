/*************************************************************************/
/* Fichier:			 UART.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/

#include "uart.h"

/************************** VARIABLES GLOBALES ***************************/	

extern volatile float speed;
extern volatile float angle;

OS_EVENT	*UARTRxBox;
OS_EVENT	*DebugMessageQ;

OS_EVENT	*sem_UARTTx;
OS_EVENT	*sem_UARTEcho;

INT8U tpRx = 0;

/********************** INTERRUPT UART TX Complete ***********************/	
ISR(USART_TXC_vect)      
{
	OS_INT_ENTER();

	OSSemPost(sem_UARTTx);

	OS_INT_EXIT();
}
/*************************************************************************/

/********************** INTERRUPT UART RX Complete ***********************/	
ISR(USART_RXC_vect)      
{
	OS_INT_ENTER();

	/* Reset WATCHDOG*/
	wdt_reset();
		
	tpRx = UDR;
	OSMboxPost(UARTRxBox, &tpRx);
	
	OS_INT_EXIT();
}
/*************************************************************************/

/******************************* INIT UART *******************************/
void initUART(void)
{
	//- vitesse = 9600 Baud 
	//- 1 bit d’arrêt 
	//- Mode : Interruption
	//- 8 bits de données 	//- pas de parité	
    UCSRC = (1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|
			(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);
			
    UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);       
   
    /* set baud rate */
    UBRRH = (INT8U)(UART_BAUD_SELECT >> 8);         
    UBRRL = (INT8U)(UART_BAUD_SELECT & 0x00FF);
	
	/*Enable WATCHDOG with a 1s timer*/
	wdt_enable(WDTO_1S);          
}
/*************************************************************************/

static void task_SendDebug(void *p_arg)
{
	INT8U	err;
	char	*p;
	
	(void)p_arg;          // Prevent compiler warnings

	while (1)
	{
		p = (char *) OSQPend(DebugMessageQ, 0, &err);
		OSSemPend(sem_UARTEcho, 0, &err);
		OSSemPend(sem_UARTTx, 0, &err);
		UDR = 0xFE;
		
		while (*p != 0)
		{
			OSSemPend(sem_UARTTx, 0, &err);
			UDR = *p++;
		}
		
		OSSemPend(sem_UARTTx, 0, &err);
		UDR = 0xFF;
		OSSemPost(sem_UARTEcho);
	}
}

/************************ RÉCEPTION DE COMMANDES *************************/
static void task_ReceiveCommand(void * p_arg)
{
	INT8U	err;
	short	tp;
	INT8U	Etat = 0;

	(void)p_arg;          // Prevent compiler warnings

	cli();
	InitOSTimer();
	UARTRxBox = OSMboxCreate(NULL);
	sei();

	while (1)
	{
		tp = *((short *) OSMboxPend(UARTRxBox, 0, &err));
		
		if (OSSemAccept(sem_UARTEcho))
		{
			OSSemPend(sem_UARTTx, 0, &err);
			UDR = tp;
			OSSemPost(sem_UARTEcho);
		}
		switch(Etat)
		{
			case 0 :	if (tp == 0xF0)		 { speed = 0.0; }
			else if (tp == 0xF1) { Etat = 1; }
			break;
			
			case 1 :	speed = (((float) tp) - 100.0)/100.0;
			Etat = 2;
			break;
			
			case 2 :	angle = ((float) tp)*Pi/90.0;
			break;
		}
	}
}

