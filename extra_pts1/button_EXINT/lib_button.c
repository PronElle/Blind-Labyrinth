#include "button.h"
#include "lpc17xx.h"

/**
 * @brief  Function that initializes Buttons
 */
void BUTTON_init(void) {  
  LPC_GPIO2->FIODIR      &= ~(1 << 10);    /* PORT2.10 defined as input          */

  LPC_GPIO2->FIODIR      &= ~(1 << 11);    /* PORT2.11 defined as input          */
  
  LPC_GPIO2->FIODIR      &= ~(1 << 12);    /* PORT2.12 defined as input          */

  LPC_SC->EXTMODE = 0x7;
		
	NVIC_SetPriority(EINT0_IRQn, 3);				 /* decreasing priority	from EINT2->0	 */
	NVIC_SetPriority(EINT1_IRQn, 2);				 /* decreasing priority	from EINT2->0	 */
  NVIC_SetPriority(EINT2_IRQn, 1);				 /* decreasing priority	from EINT2->0	 */
}

void enable_INT0(){
	LPC_PINCON->PINSEL4    |= (1 << 20);		 /* External interrupt 0 pin selection */
	NVIC_EnableIRQ(EINT0_IRQn);              /* enable irq in nvic                 */
}

void enable_KEY1(){
	LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */	
	NVIC_EnableIRQ(EINT1_IRQn);              /* enable irq in nvic                 */
}

void enable_KEY2(){
	 LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */	
   NVIC_EnableIRQ(EINT2_IRQn);              /* enable irq in nvic                 */
}

void disable_INT0(){
	NVIC_DisableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* External interrupt 0 pin selection */	
}

void disable_KEY1(){
	NVIC_DisableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* External interrupt 0 pin selection */	
}

void disable_KEY2(){
	NVIC_DisableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* External interrupt 0 pin selection */	
}

