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
#include "../game.h"
#include <stdbool.h>

#define IS_JOY_SEL()(LPC_GPIO1->FIOPIN & (1 << 25)) == 0
#define IS_JOY_UP() (LPC_GPIO1->FIOPIN & (1 << 29)) == 0
#define IS_JOY_DW() (LPC_GPIO1->FIOPIN & (1 << 26)) == 0
#define IS_JOY_LT() (LPC_GPIO1->FIOPIN & (1 << 27)) == 0
#define IS_JOY_RT() (LPC_GPIO1->FIOPIN & (1 << 28)) == 0

extern player_t pl;
extern mode _mode;

/**
* @brief handles joystick for moving/exploring
*				 update_obstacle used to avoid checking for new
*				 obstacles to be displayed if no movement/ration
*				 is performed
* @param cnt : the pointer to the counter variable
*							 associated to that direction
* @param dir:  current player direction
* @return None
**/
void handle_joy(uint8_t *cnt, dir _dir){
	static bool update_obstacle = false; 
	
	if(pl.direction != _dir){
			player_changeDir(_dir);
		  (*cnt) = 0; // reset counter if new dir
		
		  // redrawing
		  delete_cell(pl.position);
			draw_player(pl, _mode);
			update_obstacle = true;
		}
		
		if(_mode == MOVE){
			(*cnt)++;
			if(*cnt == RUN_T) { 
				(*cnt) = 0;
				if(consented_move()){
					delete_cell(pl.position); // deleting player
					player_move();     // updating position 
					draw_player(pl, _mode); // redrawing player
					
					if(exit_reached()) { // win condition check
						victory_setup();
						return;
					}
					update_obstacle = true;
				}
			}
	  }
		
		if(update_obstacle){ // action performed only if needed
			 reveal_obstacles();
			 update_obstacle = false;
		}
		
		return;
}



void RIT_IRQHandler (void)
{ 
   static uint8_t lt  = 0;
	 static uint8_t rt  = 0;
   static uint8_t up  = 0;
   static uint8_t dw  = 0;
   static uint8_t sel = 0;
	
	/*------------- sel -------------*/
  if(IS_JOY_SEL()) {
		++ sel;
		if(sel == 1) {
			change_mode();
			delete_cell(pl.position);
			draw_player(pl, _mode);
		}
	}else sel = 0;
	
	
	/*------------- up -------------*/
	if(IS_JOY_UP()) 
		handle_joy(&up, N);
	else up = 0;
	
	
	/*------------- down -------------*/
	if(IS_JOY_DW()) 
		handle_joy(&dw, S);
	else dw = 0;
	
	/*------------- left -------------*/
	if(IS_JOY_LT()) 
	 handle_joy(&lt, W);
	else lt = 0;
	
	/*------------- right -------------*/
	if(IS_JOY_RT()) 
		handle_joy(&rt, E);
	else 	rt = 0;
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

