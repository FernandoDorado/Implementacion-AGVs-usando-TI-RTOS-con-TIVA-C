
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/cfg/global.h>

#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "Board.h"
#include <ti/drivers/UART.h>
#include "driverlib/ssi.h"
#include "driverlib/adc.h"

#define THREADSTACKSIZE    1024



/**************PROTOTIPO DE LAS TAREAS***************/
void ini_hardware(void);
void mov_AGV(void);
void ISR(void);
void ADC_INT(void);



/************VARIABLES GLOBALES**********************/
uint32_t reloj;
uint32_t hum;

typedef struct MsgObj {
Queue_Elem	elem;
char val;            		// Valor del mensaje
} MsgObj, *Msg;				// Usamos Mng como puntero a MngObj



/*************************main************************/
void main(void)
{
   ini_hardware();				// Inicializamos el hardware
   BIOS_start();
   while(1);
}

/*****************************************************************************
 * mov_AGV() - llamado por BIOS_Start()
 *****************************************************************************/
void mov_AGV(void)
{
    MsgObj mensaje;																    //definimos mensaje usando la estructura creada al inicio
	Msg msgp;																		//Definimos un puntero a MsgObj para usar la cola
	msgp = &mensaje;														        //Inicializamos msgp apuntando al adireccion de mensaje
	Semaphore_post(AGV_Sem);                                                        //Habilito el recurso compartido (AGV) para que los diferentes hilos accedan a el

	while(1)
	{
		Semaphore_pend(Cola_Sem, BIOS_WAIT_FOREVER);										// Esperamos que la cola de mensajes reciba un mensaje
		msgp = Queue_get(Cola_AGV);			                                                // Leemos el contenido de la cola para saber donde se dirige el AGV

		if (msgp->val == '0'){
		    /* Modo vuelta a empezar */
		    Semaphore_pend(AGV_Sem, BIOS_WAIT_FOREVER);
		    UARTCharPut(UART0_BASE,  '.');UARTCharPut(UART0_BASE,  '-');
		    UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'G');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'H');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  'm');UARTCharPut(UART0_BASE,  'e');
		    Task_sleep(1000);
		    UARTCharPut(UART0_BASE,  '.');
            Task_sleep(2000);
            UARTCharPut(UART0_BASE,  '.');
            Task_sleep(5000);
            UARTCharPut(UART0_BASE,  '.');
            Task_sleep(15000);
            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'L');UARTCharPut(UART0_BASE,  'l');UARTCharPut(UART0_BASE,  'e');UARTCharPut(UART0_BASE,  'g');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  'd');UARTCharPut(UART0_BASE,  'a');
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
            Semaphore_post(AGV_Sem);
		}
		else
		    if(msgp->val == '1'){
		    /* Recibida petición cinta 1 */
		    Semaphore_pend(AGV_Sem, BIOS_WAIT_FOREVER);
		    UARTCharPut(UART0_BASE,  '.');UARTCharPut(UART0_BASE,  '-');
		    UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'C');UARTCharPut(UART0_BASE,  'i');UARTCharPut(UART0_BASE,  'n');UARTCharPut(UART0_BASE,  't');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  '1');
            Task_sleep(1000);
            UARTCharPut(UART0_BASE,  '.');
            Task_sleep(2000);
            UARTCharPut(UART0_BASE,  '.');
            Task_sleep(5000);
            UARTCharPut(UART0_BASE,  '.');
            Task_sleep(15000);
            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'L');UARTCharPut(UART0_BASE,  'l');UARTCharPut(UART0_BASE,  'e');UARTCharPut(UART0_BASE,  'g');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  'd');UARTCharPut(UART0_BASE,  'a');
            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
            Task_sleep(10000);
            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  '-');UARTCharPut(UART0_BASE,  '>');UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'C');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  'l');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  'c');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  'd');UARTCharPut(UART0_BASE,  'o');
            Semaphore_post(AGV_Sem);
		    }
		    else
		        if(msgp->val == '2'){
		            /* Recibida peticion cinta 2 */
		            Semaphore_pend(AGV_Sem, BIOS_WAIT_FOREVER);
		            UARTCharPut(UART0_BASE,  '.');UARTCharPut(UART0_BASE,  '-');
		            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'C');UARTCharPut(UART0_BASE,  'i');UARTCharPut(UART0_BASE,  'n');UARTCharPut(UART0_BASE,  't');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  '2');
		            Task_sleep(1000);
		            UARTCharPut(UART0_BASE,  '.');
		            Task_sleep(2000);
		            UARTCharPut(UART0_BASE,  '.');
		            Task_sleep(5000);
		            UARTCharPut(UART0_BASE,  '.');
		            Task_sleep(15000);
		            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'L');UARTCharPut(UART0_BASE,  'l');UARTCharPut(UART0_BASE,  'e');UARTCharPut(UART0_BASE,  'g');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  'd');UARTCharPut(UART0_BASE,  'a');
		            GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
		            Task_sleep(10000);
		            UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  '-');UARTCharPut(UART0_BASE,  '>');UARTCharPut(UART0_BASE,  ' ');UARTCharPut(UART0_BASE,  'C');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  'l');UARTCharPut(UART0_BASE,  'o');UARTCharPut(UART0_BASE,  'c');UARTCharPut(UART0_BASE,  'a');UARTCharPut(UART0_BASE,  'd');UARTCharPut(UART0_BASE,  'o');
		            Semaphore_post(AGV_Sem);
		            }
	}

}

void leeUART(void){
    MsgObj msg;
    Msg msgp;
    msgp = &msg;

    while (1)
    {
        Semaphore_pend(UART_Sem, BIOS_WAIT_FOREVER);         //Esperamos la habilitación por ISR
        if (UARTCharsAvail(UART0_BASE)) {                    //Si hay un caracter disponible lo guardamos en la variable tipo estructura
        msg.val = UARTCharGet(UART0_BASE);                   //Enviamos por consola el valor introducido
        UARTCharPut(UART0_BASE,  msg.val);
        SSIDataPut(SSI0_BASE,msg.val);                       //Enviamos dato al periférico esclavo

        Queue_put(Cola_AGV, (Queue_Elem*)msgp);              // Pasamos usando punteros el valor a la cola de mensajes
        Semaphore_post (Cola_Sem);                           // Desbloqueamos el semaforo asociado a la cola de mensajes
        }
    }
}

//Interrupcion ADC
void ADC_INT(void){
        ADCIntClear(ADC0_BASE, 3);
        ADCProcessorTrigger(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, &hum);

        /* Proteccion humedad para AGV */
        if(hum<350){
            UARTCharPut(UART0_BASE, 'E');
            UARTCharPut(UART0_BASE, 'R');
            UARTCharPut(UART0_BASE, 'R');
            UARTCharPut(UART0_BASE, 'O');
            UARTCharPut(UART0_BASE, 'R');
        }

}

/*****************************************************************************
 * Rutina de servicio de interrupción.
 *
 *******************************************************************************/
void ISR(void)
{
    TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);			            // Limpiamos el flag
	Semaphore_post(Cola_Sem);
	Semaphore_post(UART_Sem);


}


/***************************************************************************
 * Inicialización del hardware
 **************************************************************************/
void ini_hardware(void)
{
    uint32_t ui32Period;

    //Fijar velocidad a 120MHz
    reloj=SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);



    /* Habilitar los periféricos implicados: GPIOF, N */
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);

    /* Definir tipo de pines */
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 |GPIO_PIN_4);                                   //F0 y F4: salidas
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 |GPIO_PIN_1);                                   //N0 y N1: salidas

    /* Habilitamos interrupciones */
    IntMasterEnable();


    /* Timer2 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);           // Habilitamos el periferico
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);        // Establecemos como periódico
    ui32Period = (SysCtlClockGet() /2);                     // Periodo dividido por 2 (500ms)
    TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period);         // Establecemos el timer con dicho periodo
    TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);        // Habilitamos las interrupciones por timer
    TimerEnable(TIMER2_BASE, TIMER_A);                      // Activamos


    /* UART */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, reloj, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    /* Mensaje inicial */
    UARTCharPut(UART0_BASE, 'M');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, ':');
    UARTCharPut(UART0_BASE, '\n');

    /* SSI */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    SSIConfigSetExpClk(SSI0_BASE, 48000000, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 2000000, 16);
    SSIEnable(SSI0_BASE);

    /* ADC */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END);
    ADCIntEnable(ADC0_BASE, 3);
    IntEnable(INT_ADC0SS3);
    ADCSequenceEnable(ADC0_BASE, 3);




}






