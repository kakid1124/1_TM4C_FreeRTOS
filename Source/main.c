//*************************************************//
//**** Tiva C - FreeRTOS Project ****//
//
// Nguyen Huynh Da Khoa - Version 1.0 - 29.09.2016 //
//
// System clock: 40MHz
//
//*************************************************//

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//**** Function Prototype ****//
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void System_Init(void);
void SysTick_Handler(void);
	
#ifdef __cplusplus
}
#endif


volatile uint32_t i = 0;

//*****************************************************************************
//
// The mutex that protects concurrent access of UART from multiple tasks.
//
//*****************************************************************************
xSemaphoreHandle g_pUARTSemaphore;

//*****************************************************************************
//
// This hook is called by FreeRTOS when an stack overflow error is detected.
//
//*****************************************************************************
void
vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while(1)
    {
    }
}


//*************************************************//
//**** Main Program ****//
//
//*************************************************//

int main()
{
	System_Init();
	
	UART_OutString("\n\nWelcome to the FreeRTOS Demo...\n");
	
    //
    // Create a mutex to guard the UART.
    //
    g_pUARTSemaphore = xSemaphoreCreateMutex();

    //
    // Create the LED task.
    //
    if(LEDTaskInit() != 0)
    {

        while(1)
        {
        }
    }

    //
    // Create the switch task.
    //
    if(SwitchTaskInit() != 0)
    {

        while(1)
        {
        }
    }

    //
    // Start the scheduler.  This should not return.
    //
    vTaskStartScheduler();

    //
    // In case the scheduler returns for some reason, print an error and loop
    // forever.
    //
		
		while(1)
		{
		}
}


//*************************************************//
//**** Systems_Init ****//
//
//*************************************************//
void System_Init(void)
{
	PLL_Init_8MHz(); // System Clock: 40MHz
//	PortF_Init();
	UART_Init_9600bps();
	
//	EnableInterrupts();
	
	FPUEnable();
	FPULazyStackingEnable();
	
}

