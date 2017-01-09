
#include <stdint.h>
#include "systick.h"

#define NVIC_ST_CTRL_R          (*((volatile uint32_t *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile uint32_t *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile uint32_t *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 19999;  // System clock 20MHz: 50ns*(19999+1) = 1 miliseconds
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable interrupt
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_ENABLE|NVIC_ST_CTRL_INTEN;
}

