#include "lpc17xx.h"
#include "game.h"
#include "RIT/RIT.h"
#include "timer/timer.h"

player_t pl; 
mode _mode;
extern bool game_started, game_ended; // defined in timer.h

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

uint8_t visible_table[ROW][COL] = { 0 }; // table of visible obstacles

/**
* @brief initializes game
**/
void game_init(void){
 pl.direction = DIR;
 pl.position.x = STARTING_X;
 pl.position.y = STARTING_Y;
 _mode = MOVE;
}


/**
* @brief enables run and rotate buttons
*				 and blinks direction led  
**/
void start_game(void){
	clean_screen(TABLE_COLOR);
	draw_grid();
	game_init(); 
	draw_player(pl, _mode);
	
	// RIT only used during gameplay
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec */
	enable_RIT();
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
	      c.y >= 0 &&  c.y < COL ;
}


/**
* @brief checks whether robot can move in current direction or not
* @param None
* @return true if move consented, false otherwise
**/
bool consented_move(void){
	 cell_t c = next_cell(pl.position);
	 return valid_cell(c) && IS_NOT_OBSTACLE(c.x, c.y);
}

/**
* @brief moves player according to its orientation
**/
void player_move(void){	
	pl.position = next_cell(pl.position);
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
* @brief disables RIT (joystick no longer polled)
*				 and draws winning message	
* @param None
**/
void victory_setup(void){
	disable_RIT();
	reset_RIT();
	draw_exit(pl.position);
  draw_winScreen();
	game_started = false;
	game_ended = true;
}


/**
* @brief  changes game mode
* @param  None
* @return None
**/
void change_mode(void){
	_mode = _mode == MOVE ? EXPLORE : MOVE;
}


/**
* @brief changes player direction
* @param _dir: new direction
* @return None
*
**/
void player_changeDir(dir _dir){
	pl.direction = _dir;
}

/**
* @brief  draws obstacles (if any) included in the next
*         5 slots according to player's direction
* @param  None
* @return None
**/
void reveal_obstacles(){
	int i = 0;
	cell_t next;
	
	for( i = 0, next = next_cell(pl.position); i < 5 && valid_cell(next); ++i, next = next_cell(next))
		if(table[next.x][next.y] == B) { 
			if(!visible_table[next.x][next.y]){ // if not visible, reveal it, otherwise exit
				draw_obstacle(next); 
				visible_table[next.x][next.y] = 1; // mark as now visible
			}
			break;
		}
}


/**
* @brief  function associated to "CLEAR" button 
*					pressure: obscures visible obstacles
* @param  None
* @return None
**/
void clear(void){
	int i,j;

	for (i = 0; i < ROW; ++i)
		for(j = 0; j < COL; ++j)
			if(visible_table[i][j]) {
				cell_t c;  c.x = i; c.y = j;
				delete_cell(c);
				visible_table[i][j] = 0;
			}
}

/**
* @brief  function associated to "RESTART" button 
*					pressure: setups everytingh to start over 
* @param  None
* @return None
**/
void restart(void){
	// resetting array of visible obstacles
	memset(visible_table, 0, sizeof(visible_table));
	draw_initScreen();
}


