/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <stdbool.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"

extern uint8_t dir_led; // declared in led.h

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

bool _reached_exit = false;

void toggle_posLED(){
	static bool on = true; 
	on ? LED_Off(dir_led) : LED_On(dir_led);
	on = !on;
}

void toggle_All(){
	static bool on = true;
	on ? LED_Off_All() : LED_On_All();
	on = !on;
}

void TIMER0_IRQHandler (void)
{
	_reached_exit ? toggle_All() : toggle_posLED();
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
