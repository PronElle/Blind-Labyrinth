#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "game.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif
 
int main (void) {
  	
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	enable_INT0();			
	
	init_RIT(0x2626A0);									  /* RIT Initialization 25 msec       */
  game_init();													/* initializing game robot*/
	
	enable_RIT();
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}
