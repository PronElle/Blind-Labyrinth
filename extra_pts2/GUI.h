#include "lpc17xx.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"
#include "game_globals.h"

#include <math.h>
 
/* --- GUI Colors ----- */
#define MOVE_COLOR 				Black
#define EXPL_COLOR 				Blue
#define GRID_COLOR 				Black
#define BLOCK_COLOR 			Red
#define BG_COLOR 					Black
#define TABLE_COLOR	  		White
#define INIT_SCREEN_COLOR Blue2
#define BUTTON_COLOR 		  Yellow
#define TITLE_BG_COLOR 		Yellow
#define EXIT_COLOR				Green

/*---- GUI Constants ----- */
// grid cell
#define CELL_SIZE 		15

// table
#define TABLE_START_X	 7  
#define TABLE_START_Y	48 

/* buttons dims */
#define BTN_LEN 			38  
#define BTN_WID       75

// clear button 
#define CLR_START_X		150 
#define CLR_START_Y		270
#define CLR_END_X			CLR_START_X + BTN_WID  
#define CLR_END_Y			CLR_START_Y + BTN_LEN 

// restart button 
#define RSTRT_START_X		10 
#define RSTRT_START_Y		270
#define RSTRT_END_X			RSTRT_START_X + BTN_WID
#define RSTRT_END_Y			RSTRT_START_Y + BTN_LEN

void screen_setup(void);
void clean_screen(int color);

void draw_initScreen(void);
void draw_winScreen(void);
void draw_grid(void);
void draw_buttons(void);

void draw_player(player_t pl, mode _mod);
void draw_obstacle(cell_t cell);
void draw_exit(cell_t cell);

void delete_cell(cell_t cell);


