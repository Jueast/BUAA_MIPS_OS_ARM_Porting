#include <printf.h>

#include <rpi-aux.h>
#include <rpi-armtimer.h>
#include <rpi-gpio.h>
#include <rpi-interrupts.h>
#include <rpi-systimer.h>

void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags)
{
	// Enable the timer interrupt IRQ
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;
	
	/* Setup the system timer interrupt */
   	/* Timer frequency = Clk/256 * 0x400 */
    	RPI_GetArmTimer()->Load = 0x400;

    	/* Setup the ARM Timer */
    	RPI_GetArmTimer()->Control =
           	 RPI_ARMTIMER_CTRL_23BIT |
           	 RPI_ARMTIMER_CTRL_ENABLE |
            	 RPI_ARMTIMER_CTRL_INT_ENABLE |
            	 RPI_ARMTIMER_CTRL_PRESCALE_256;

	/* Initialise the UART */
    	RPI_AuxMiniUartInit( 115200, 8 );
	printf("main.c:\tmain is start ...\n");
	arm_init();	
	while(1) {
	   /* trap here */
	}
	
}	
