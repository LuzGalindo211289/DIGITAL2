

/**
 * main.c
 */
#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"


#define THRESHOLD 2458

volatile uint32_t adcValues[4];
volatile uint8_t flag[4] = {0};
unsigned int i=0;

// Cadena de valores a enviar tiva 1
     char *uartString1 = "\xDD\x00\x01\x00\xFF";
     char *uartString2 = "\xDD\x00\x02\x00\xFF";
     char *uartString3 = "\xDD\x00\x03\x00\xFF";
     char *uartString4 = "\xDD\x00\x04\x00\xFF";

 /* Sustituir declaración de strings por estos para el TIVA 2 con la prestada. El que simula el parking 2, las plazas 5, 6 ,7 y 8.
  *  char *uartString1 = "\xDD\x10\x01\x00\xFF";
     char *uartString2 = "\xDD\x10\x02\x00\xFF";
     char *uartString3 = "\xDD\x10\x03\x00\xFF";
     char *uartString4 = "\xDD\x10\x04\x00\xFF";
  */

void initDigitalOutputs(void) {
    // Habilitar los módulos GPIOA y GPIOE
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Configurar pines de salida para las salidas digitales
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Inicializar las salidas en estado bajo
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void initUART(void) {
    // Habilitar el módulo UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    // Esperar a que el módulo UART1 esté listo
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART1)) {
    }

    // Habilitar el puerto serie para los pines PB0 y PB1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configurar los pines PB0 y PB1 para la función alternativa del UART1
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configurar el UART1 para una velocidad de baudios de 115200
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    // Habilitar el módulo UART1
    UARTEnable(UART1_BASE);
}

void initTimer(void) {
    // Habilitar el módulo TIMER0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Esperar a que el módulo TIMER0 esté listo
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {
    }

    // Configurar el temporizador para que funcione en modo temporizador de un solo disparo
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Configurar el temporizador para que interrumpa cada 500 ms (suponiendo una frecuencia de reloj de 16 MHz)
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 2);

    // Habilitar la interrupción del temporizador
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Habilitar el temporizador
    TimerEnable(TIMER0_BASE, TIMER_A);
}
void Timer0IntHandler(void) {
    // Limpiar la bandera de interrupción del temporizador
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enviar la cadena por el UART
    UARTCharPut(UART1_BASE, *uartString1);
    UARTCharPut(UART1_BASE, *uartString2);
    UARTCharPut(UART1_BASE, *uartString3);
    UARTCharPut(UART1_BASE, *uartString4);

}

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Esperar a que el módulo ADC0 esté listo ;
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {
    }

    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 2, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 3, ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

        // Habilitar interrupción de fin de secuencia para el secuenciador 3
    ADCIntEnable(ADC0_BASE, 3);

    initDigitalOutputs();

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


    // Inicializar el UART
    initUART();              // initialize UART

    initTimer();

    // Habilitar las interrupciones
    IntMasterEnable();



    while (1) {
            // Iniciar la conversión
            ADCProcessorTrigger(ADC0_BASE, 3);

            // Esperar hasta que la conversión haya terminado
            while (!ADCIntStatus(ADC0_BASE, 3, false)) {
            }

            // Leer los resultados de la conversión
            ADCSequenceDataGet(ADC0_BASE, 3, adcValues);

            // Comparar con el valor umbral y activar la flag si el valor leído es menor
            if (adcValues[0] < THRESHOLD) {
            flag[0] = 1;
            *uartString1 = "\xDD\x00\x01\x01\xFF";
            } else {
            flag[0] = 0;
            *uartString1 = "\xDD\x00\x01\x00\xFF";
            }

            if (adcValues[1] < THRESHOLD) {
            flag[1] = 1;
            *uartString1 = "\xDD\x00\x02\x01\xFF";
            } else {
            flag[1] = 0;
            *uartString1 = "\xDD\x00\x02\x00\xFF";
            }

            if (adcValues[2] < THRESHOLD) {
            flag[2] = 1;
            *uartString1 = "\xDD\x00\x03\x01\xFF";
            } else {
            flag[2] = 0;
            *uartString1 = "\xDD\x00\x03\x00\xFF";
            }

            if (adcValues[3] < THRESHOLD) {
            flag[3] = 1;
            *uartString1 = "\xDD\x00\x04\x01\xFF";
            } else {
            flag[3] = 0;
            *uartString1 = "\xDD\x00\x04\x00\xFF";
            }

            // Controlar las salidas digitales basadas en las flags
            if (flag[1]) {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);
            } else {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0);
            }

            if (flag[2]) {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0);
            } else {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_PIN_1);
            }

            if (flag[3]) {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0);
            } else {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_PIN_2);
            }

            if (flag[0]) {
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);
           GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0);
           } else {
           GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);
           GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_PIN_3);
           }
           // Borrar la bandera de interrupción
           ADCIntClear(ADC0_BASE, 3);

           // Tu lógica aquí usando los valores de las entradas analógicas (adcValues[0] a adcValues[3])
       }
}
