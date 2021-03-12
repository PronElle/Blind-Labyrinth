/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../RIT/RIT.h"
#include "../TouchPanel/TouchPanel.h"
#include "../GLCD/GLCD.h"
#include "../game.h"


#define PRESSURE_ON_SCREEN() display.x >= TABLE_START_X && display.x <= TABLE_START_X + COL*CELL_SIZE && \
                              display.y  >= TABLE_START_Y && display.y <= TABLE_START_Y + ROW*CELL_SIZE

#define PRESSURE_ON_CLEAR()  display.x >= CLR_START_X && display.x <= CLR_END_X && \
                              display.y >= CLR_START_Y && display.y <= CLR_END_Y

#define PRESSURE_ON_RESTART() display.x >= RSTRT_START_X && display.x <= RSTRT_END_X && \
                               display.y >= RSTRT_START_Y && display.y <= RSTRT_END_Y


volatile bool game_started = false;
volatile bool game_ended = false;

/************************************************************************************
*																																										*
*		TIMER0 	*
*																																										*
************************************************************************************/
void TIMER0_IRQHandler (void)
{
	getDisplayPoint(&display, Read_Ads7846(), &matrix ) ;
	
	if(!game_started && PRESSURE_ON_SCREEN() && !game_ended){ // i.e. if screen pressed on initial_screen
		  game_ended = false;
		  game_started = true;
			start_game();
			reveal_obstacles();
	}
	else if(game_started && PRESSURE_ON_CLEAR())	
		clear();
	
	
	if(PRESSURE_ON_RESTART() && (game_started || game_ended)) {
		 // flag check to avoid redisplaying init screen if 
    //	already displaying it and	restart pressed
		restart();
		game_started = false;
		game_ended = false;
	}
	
	reset_timer(0);
	enable_timer(0);
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)				//timer per in bottone RUN, blink del led
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}
