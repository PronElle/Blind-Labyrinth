#include "lpc17xx.h"
#include "game.h"
#include "led/led.h"
#include "timer/timer.h"
#include "button_EXINT/button.h"

struct player pl; 
extern uint8_t dir_led; // declared in led.h
extern bool _reached_exit; // declared in timer.h
	
const uint8_t table[ROW][COL] = {
	{X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X},
	{0, 0, B, B, B, B, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, B, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, B, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, B, 0, 0, 0, 0, 0, 0, 0},
	{B, B, B, B, B, 0, 0, B, 0, B, 0, 0, 0, B, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, B, 0},
	{B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, B, 0},
	{B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, B, 0},
	{B, 0, B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{B, 0, B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{B, 0, B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{X, 0, B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X},
};



/**
* @brief initializes game
**/
void game_init(void){
 pl.direction = DIR;
 pl.position.x = STARTING_X;
 pl.position.y = STARTING_Y;
 dir_led = E_LED;  // set direction LED
}


/**
* @brief enables run and rotate buttons
*				 and blinks direction led  
**/
void start_game(void){
  LED_On(dir_led);
	update_sensor_blink();
	
	enable_KEY1();
  enable_KEY2();
}



/**
* @brief  provides next cell according to direction
* @param  c: current cell
* @return ret: next cell
**/
cell_t next_cell (cell_t c){	
  cell_t ret = c;
	
	switch(pl.direction){
		case N: ret.x --; break; 
	  case E: ret.y ++; break;
		case S: ret.x ++; break;
		case W: ret.y --; break;
		default: break;
	}
	
	return ret;
}


/**
* @brief return wheter provided cell is
*				 legit for robot to step on it
* @param c: cell
* @retval true if legit, false otherwise
**/
bool valid_cell(cell_t c){
 return c.x >= 0 &&  c.x < ROW && 
	      c.y >= 0 &&  c.y < COL && 
        table[c.x][c.y] != B;  
}



/**
* @brief evaluates distance from nearest obstancle 
*        in current running direction
* @param None
* @return distance if no exit on path, "infinite" otherwise
**/
uint8_t getDistance(void){
	uint8_t dist = 0;
	bool _exit = false;

	cell_t next = next_cell(pl.position);

	while(valid_cell(next) && !_exit){    
		dist ++;
		if (table[next.x][next.y] == X) _exit = true;
		
		next = next_cell(next);		
	}

	return _exit ? 0xff : dist; // if exit on path, return "infinite" distance (no blink) 
}

/**
* @brief checks whether robot can move in current direction or not
* @param None
* @return true if move consented, false otherwise
**/
bool consented_move(void){
	 return valid_cell(next_cell(pl.position));
}



/**
* @brief rotates player clockwise
* @param None
**/
void player_rotate(void){
	LED_Off(dir_led); // turning off previous pos led

  // more readable than using "++" operator 	
	switch(pl.direction){
		case N: pl.direction = E; dir_led = E_LED; break;
		case E: pl.direction = S; dir_led = S_LED; break;
		case S: pl.direction = W; dir_led = W_LED; break;
		case W: pl.direction = N; dir_led = N_LED; break;
		default: break;
	}	
	
	LED_On(dir_led); // turning on new pos led
	update_sensor_blink();
}


/**
* @brief moves player according to its orientation
**/
void player_move(void){	
	pl.position = next_cell(pl.position);
}


/**
* @brief updates position LED blink 
*			   according to current distance from
* 		   obstacles.
*        curr_k used to avoid repeating unneeded 
*				 actions, e.g. rearming timer if distance goes
*        from 5 to 4 
**/
void update_sensor_blink(void){
	static int curr_k = -1; 
	
	switch(getDistance()){
					   case 5: 
					   case 4:
					   case 3:
										if(curr_k != K53){
											rearm_timer(0, K53);
											curr_k = K53;
										}
										break;
					   case 2: 
						 case 1:
									 if(curr_k != K21){
											rearm_timer(0, K21);
											curr_k = K21;
										}
										break;
					   case 0: 
							       if(curr_k != K0){
											 rearm_timer(0, K0);
											 curr_k = K0;
											}
										 break;
					   default: // > 5 
									 if(curr_k != -1) {
										 curr_k = -1;		
										 disable_timer(0);
									 }								  
	 }
}


/**
* @brief checks whether exit has been reached
* @param None
* @return true if exist reached, false otherwise
**/
bool exit_reached(void){
	return table[pl.position.x][pl.position.y] == X;
}



/**
* @brief disables button and timer interrupts
*	       arms timer 0 to interrupt every 1 s
*        toggling all LEDs
* @param None
**/
void victory_setup(void){
	disable_KEY1();
	disable_KEY2();
	
	// setup timer for all LEDs 1Hz blink
	_reached_exit = true; // useful for timer0
	rearm_timer(0, K_WIN);
	
	LED_On_All(); // turning all LEDs ON
}

