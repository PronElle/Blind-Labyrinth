#include "button.h"
#include "lpc17xx.h"

#include "../RIT/RIT.h"		  			
#include "../game.h"

extern int key1, key2, int0;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{	
	disable_INT0();  
	int0 = 1;
	                
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{ 
	disable_KEY1();
	key1 = 1;
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{  
	  disable_KEY2();
		key2 = 1;
		
		LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */
}


