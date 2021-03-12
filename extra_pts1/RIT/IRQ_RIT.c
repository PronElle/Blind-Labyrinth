/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../game.h"
#include "../button_EXINT/button.h"
#include <stdbool.h>

#define IS_INT0_PRESSED() (LPC_GPIO2->FIOPIN & (1<<10)) == 0
#define IS_KEY1_PRESSED() (LPC_GPIO2->FIOPIN & (1<<11)) == 0
#define IS_KEY2_PRESSED() (LPC_GPIO2->FIOPIN & (1<<12)) == 0

volatile int key1 = 0;
volatile int key2 = 0; 
volatile int int0 = 0;

static uint8_t T_mul = RUN_T; /*  blink period in multiples of 50 ms*/ 

/**
* @brief handles INT0 pressure. Only called by RIT IRQHandler
* @param None
**/
void INT0_down(void){
	if(IS_INT0_PRESSED()){
		  int0++;
			if(int0 == 4){ // 75 ms 
				start_game();
				disable_INT0(); // never renabled
				int0 = 0; 
				return;
			}
	} else{
		int0 = 0;
		enable_INT0();
	}
}

/**
*  @brief handles KEY pressure. Only called by RIT IRQHandler
*  @param None
**/
void KEY1_down(void){

		if(IS_KEY1_PRESSED()) {	
			key1++;	
		}else{
			if(key1 >= 4) { 
				// actions performed when released
				player_rotate();
				T_mul = consented_move() ? RUN_T : BLOCK_T;
			}		
			key1 = 0;
			enable_KEY1();	
		}
	return;
}




/**
*  @brief handles KEY2 pressure. Only called by RIT IRQHandler
*  @param None
**/
void KEY2_down(void){
	static uint8_t cnt = 0;
		
	if(IS_KEY2_PRESSED()){
		  cnt ++ ; 
			if(cnt == T_mul / 2) // 0.5 sec or 100 ms elapsed
				LED_On(RUN_LED);  
			else if(cnt == T_mul){ // 1 sec or 200 ms elapsed
				cnt = 0;
				LED_Off(RUN_LED);
				
				if(consented_move()){
					player_move();
					
					// exit condition only checked if player actually moved
					if(exit_reached()) {
						victory_setup();
						disable_RIT();
						return; 
					}
					
				  T_mul = consented_move() ? RUN_T : BLOCK_T; // gets ready for NEXT move
				  update_sensor_blink();
				}
		 }
			
	 } else {	/* button released */
			LED_Off(RUN_LED);
			key2 = 0;
			cnt = 0; // to avoid blink for 10 repetitive pressures without holding
			enable_KEY2();
		}
}



void RIT_IRQHandler (void)
{	
	//handling int0
	if(int0) INT0_down();
	// handling key1
	if(key1) KEY1_down();
	// handling key2
	if(key2) KEY2_down();
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

