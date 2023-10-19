#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_gpio.h"

#ifdef DEBUG
void
__error_(char *pcfilename, uint32_t ui32Line)
{
    while(1);

}
#endif

int main(void)
{

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    volatile uint32_t ui32Loop;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
//-----------------------------------------------------------
    while(1)
    {


      if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){

          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);

          for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
               {
                }
  //-----------------------------------------LUZ VERDE---------------------------------
                   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                                 for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                                                 {
                                                 }

                  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                                 for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                                 {
                                                 }

 //----------------------------------------------------LUZ VERDE PARPADEA---------------------------------
                     GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                                      for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++)
                                                      {
                                                          }

                       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                                      for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                                            {
                                                               }
 //---- -------              -----------------             ----------                 ------------       -------
                  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                   for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++)
                                      {
                                         }

                      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                      for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                           {
                                            }
 //-----------------------------------------LUZ AMARILLA----------------------------------
              GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0xff);


                  for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                       {
                         }

                      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0x0);

                           for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++ )
                                {
                                 }
 //-----------------------------------------LUZ ROJA----------------------------------
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

                       for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                       {
                      }


      }
//-------------------------------------------------------------------------------------
      if (!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)){

        //-----------------------------------------LUZ VERDE---------------------------------
          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);

          for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
               {
                }

          GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                                       for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                                                       {
                                                       }

                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                                       for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                                       {
                                                       }

       //----------------------------------------------------LUZ VERDE PARPADEA---------------------------------
                           GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                                            for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++)
                                                            {
                                                                }

                             GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                                            for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                                                  {
                                                                     }
       //---- -------              -----------------             ----------                 ------------       -------
                        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0xff);

                                         for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++)
                                            {
                                               }

                            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x0);

                                            for(ui32Loop = 0; ui32Loop < 1000000; ui32Loop++ )
                                                 {
                                                  }
       //-----------------------------------------LUZ AMARILLA----------------------------------
                    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0xff);


                        for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                             {
                               }

                            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0x0);

                                 for(ui32Loop = 0; ui32Loop < 100000; ui32Loop++ )
                                      {
                                       }
       //-----------------------------------------LUZ ROJA----------------------------------
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

                             for(ui32Loop = 0; ui32Loop < 10000000; ui32Loop++)
                             {
                            }


            }
    }
}
