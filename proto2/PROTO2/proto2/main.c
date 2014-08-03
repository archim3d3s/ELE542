/*************************************************************************/
/* Fichier:			 MAIN.C												 */
/* Ecrit par:		 Jean-Philippe Cote et Maxim Beauregard				 */
/* Date de revision: 14/07/2014											 */
/* Cours:			 ELE542 - Systemes ordines en temps reel			 */
/*					 Ecole de Technologie Superieure				   	 */
/*************************************************************************/
#include "main.h"

extern void InitOSTimer(void);

#define SW6  (PINA & (1<<PA6))
#define SW7  (PINA & (1<<PA7))

#define STANDARD_STACK_SIZE 100
#define DEBUG_STACK_SIZE 150

OS_STK Cmd_Stk[STANDARD_STACK_SIZE];
OS_STK SendDbg_Stk[STANDARD_STACK_SIZE];
OS_STK Stop_Stk[STANDARD_STACK_SIZE];
OS_STK Start_Stk[STANDARD_STACK_SIZE];
OS_STK MotorCtl_Stk[STANDARD_STACK_SIZE];
OS_STK Disp_Stk[STANDARD_STACK_SIZE];

OS_EVENT	*sem_SystemON;
OS_EVENT	*sem_SystemOFF;
OS_EVENT	*sem_StatusChanged;

extern OS_EVENT	*DebugMessageQ;

extern OS_EVENT	*sem_UARTTx;
extern OS_EVENT	*sem_UARTEcho;
extern OS_EVENT	*sem_PWMtick;

int  main(void);

extern void task_ReceiveCommand(void*);
extern void task_SendDebug(void*);

static void task_SystemStop(void *p_arg);
static void task_SystemStart(void *p_arg);
static void task_MotorControl(void *p_arg);
static void task_StatusDisplay(void *p_arg);

/****************** IMPORTATION DES VARIABLES EXTERNES *******************/
void *DebugMessage[10];

volatile float speed = 0.0;
volatile float angle = 0.0;

INT8U LEDstatus = (1<<ATTENTE);

float RIGHT_ADCmean = 0.0;
float LEFT_ADCmean = 0.0;

float RIGHT_pwm = 0.0;
float LEFT_pwm = 0.0;
	
/**************************** POINT D'ENTRÉE  ****************************/
int main(void)
{
/**************************** INITIALISATION ****************************/
	cli();
	
	OSInit();
	initADC();
	initUART();
	initLED();
	initTIMER();
	
/************************ CALIBRATION DES MOTEURS ************************/
	sei();
	motor_calibration();
	cli();
/****************** CREATION DES TACHES ET DES SEMAPHORES ****************/ 	
	OSTaskCreate(task_SystemStop,	  NULL, (OS_STK *)&Stop_Stk[STANDARD_STACK_SIZE-1],		4);	
	OSTaskCreate(task_SystemStart,	  NULL, (OS_STK *)&Start_Stk[STANDARD_STACK_SIZE-1],	5);	
    OSTaskCreate(task_ReceiveCommand, NULL, (OS_STK *)&Cmd_Stk[STANDARD_STACK_SIZE-1],		6);
    OSTaskCreate(task_SendDebug,	  NULL, (OS_STK *)&SendDbg_Stk[STANDARD_STACK_SIZE-1],	7);
	OSTaskCreate(task_MotorControl,	  NULL, (OS_STK *)&MotorCtl_Stk[STANDARD_STACK_SIZE-1], 8);
	OSTaskCreate(task_StatusDisplay,  NULL, (OS_STK *)&Disp_Stk[STANDARD_STACK_SIZE-1],		9);
	
	sem_UARTTx			= OSSemCreate(1);
	sem_UARTEcho		= OSSemCreate(1);
	sem_SystemOFF		= OSSemCreate(0);
	sem_SystemON		= OSSemCreate(0);
	sem_StatusChanged	= OSSemCreate(1);
	
	DebugMessageQ = OSQCreate(&DebugMessage[0],10);
			
	OSStart();
}
/*************************************************************************/

/*********** INTERRUPTEURS DE MISE EN MARCHE ET ARRET D'URGENCE **********/
static void task_SystemStop(void * p_arg)
{
	INT8U	err;
	(void)p_arg;          // Prevent compiler warnings
	
	while(1)
	{
			if (!SW7)
			{				
				LEDstatus = (1<<ATTENTE);
				LEDstatus &= ~(1<<ACTIF);

				PWM_GAUCHE = 0;
				PWM_DROIT = 0;
			}
	}
}

static void task_SystemStart(void * p_arg)
{
	INT8U	err;
	(void)p_arg;          // Prevent compiler warnings
	
	while(1)
	{
		if (!SW6)
		{
			LEDstatus = 0;
			LEDstatus |= (1<<ACTIF);
			LEDstatus &= ~(1<<ATTENTE);
		}
	}
}



/************************* CONTRÔLE DES MOTEURS **************************/		
static void task_MotorControl(void * p_arg)
{
	INT8U	err;
    (void)p_arg;          // Prevent compiler warnings
	
	while(1)
	{
		//Convertir valeur de l'ADC en valeur de vitesse
		getADCmeanValues(&RIGHT_ADCmean, &LEFT_ADCmean);
		resetADC();
		
		//Calculer nouvelles valeurs de PWM
		CalculPWM( speed, angle, LEFT_ADCmean, RIGHT_ADCmean,
		&LEFT_pwm, &RIGHT_pwm);

		//Mettre a jour les PWM et bits de direction
		
		/* PWM GAUCHE */
		if(LEFT_pwm < 0)		/* MARCHE ARRIÈRE */
		{
			DIR_bit_OFF(LEFT_DIR1); DIR_bit_ON(LEFT_DIR2);
			PWM_GAUCHE = (INT16U)(-1.0*LEFT_pwm*(float)PWM_TOP);
		}

		else if(LEFT_pwm > 0)	/*  MARCHE AVANT */
		{
			DIR_bit_ON(LEFT_DIR1); DIR_bit_OFF(LEFT_DIR2);
			PWM_GAUCHE = (INT16U)(LEFT_pwm *(float)PWM_TOP);
		}

		else					/*    NEUTRE     */
		{
			DIR_bit_OFF(LEFT_DIR1); DIR_bit_OFF(LEFT_DIR2);
			PWM_GAUCHE = 0;
		}
		
		/*PWM DROIT*/
		if(RIGHT_pwm < 0)		/* MARCHE ARRIÈRE */
		{
			DIR_bit_OFF(RIGHT_DIR1); DIR_bit_ON(RIGHT_DIR2);
			PWM_DROIT = (INT16U)(-1.0*RIGHT_pwm*(float)PWM_TOP);
		}

		else if(RIGHT_pwm > 0)  /*  MARCHE AVANT */
		{
			DIR_bit_ON(RIGHT_DIR1); DIR_bit_OFF(RIGHT_DIR2);
			PWM_DROIT = (INT16U)(RIGHT_pwm * (float)PWM_TOP);
		}

		else					/*    NEUTRE     */
		{
			DIR_bit_OFF(RIGHT_DIR1); DIR_bit_OFF(RIGHT_DIR2);
			PWM_DROIT = 0;
		}

	}
}


/************************** AFFICHAGE DU STATUT **************************/
static void task_StatusDisplay(void * p_arg)
{
	INT8U	err;
    (void)p_arg;          // Prevent compiler warnings
	
	while(1)
	{
		OSSemPend(sem_StatusChanged,0,&err);
		
		updateLED(LEDstatus);
		
		LEDstatus &= ~(1<<COMMANDE_RECUE); /* Reset la LED de CMD_IN */
	}
}
