#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "timer/timer.h"
#include "game.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif


int main(void)
{
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	TP_Init();
	joystick_init();
	TouchPanel_Calibrate();
	
	screen_setup();
  
	init_timer(0, 0x4E2 ); 						    /* 200us * 25MHz = 1.25*10^3 = 0x4E2 */
	enable_timer(0);

	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
	// main loop
  while (1)	{
		__ASM("wfi");
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
