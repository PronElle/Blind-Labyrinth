#include "GUI.h"

/**
* @brief  covers table screen area with given color
* @param  color 
* @return Non
**/
void clean_screen(int color){
	LCD_drawRectangle(TABLE_START_X, TABLE_START_Y, 
	                  ROW*CELL_SIZE + 1, COL*CELL_SIZE + 1, color
	);
}


/**
* @brief draws title, buttons and displays initial screen
* @param  None
* @return None
**/
void screen_setup(){
	LCD_Clear(BG_COLOR);
	GUI_Text(50, 16, (uint8_t *) " Blind Labyrinth ", Black, TITLE_BG_COLOR);
	draw_buttons();
	draw_initScreen();
}

/**
* @brief cleans screen and displays init message
* @param  None
* @return None
**/
void draw_initScreen(){	
	clean_screen(INIT_SCREEN_COLOR);
	GUI_Text(60, 120, (uint8_t *)"Touch to Start", Black, INIT_SCREEN_COLOR);
	GUI_Text(76, 145, (uint8_t *)"a New Game", Black, INIT_SCREEN_COLOR);
}

/**
* @brief displays win message on screen when exit reached
* @param  None
* @return None
**/
void draw_winScreen(){
	uint16_t i;
	// upper border
	GUI_Text(15, 95,  (uint8_t *)"*************************", Red, TABLE_COLOR);
	
	// rt border
	for(i = 0; i < 8; ++i)
		GUI_Text(208, 105 + 10*i,   (uint8_t *)"*", Red, TABLE_COLOR);
	
	// lower border
	for(i = 0; i < 24; ++i) GUI_Text(208 - 8*i, 175,(uint8_t *)"*", Red, TABLE_COLOR);
	
	// lt border
	for(i = 0; i < 8; ++i)
		GUI_Text(15, 175 - 10*i,   (uint8_t *)"*", Red, TABLE_COLOR);
	
	// text
	GUI_Text(95, 125, (uint8_t *)"EXIT", Red, TABLE_COLOR);
	GUI_Text(80, 140, (uint8_t *)"REACHED!", Red, TABLE_COLOR);
}



/**
* @brief employs LCD built-in function to draw maze's grid
* @param  None
* @return None
**/
void draw_grid(){
uint16_t i;
	
for (i = 0; i <= COL; ++i) 
		LCD_DrawLine(TABLE_START_X + i*CELL_SIZE, TABLE_START_Y, TABLE_START_X + i*CELL_SIZE,
                	TABLE_START_Y + ROW*CELL_SIZE, GRID_COLOR);

for (i = 0; i <= ROW; ++i) 
		LCD_DrawLine(TABLE_START_X, TABLE_START_Y + i*CELL_SIZE, TABLE_START_X + CELL_SIZE*COL,
              	TABLE_START_Y + i*CELL_SIZE, GRID_COLOR);
}



/**
* @brief employs GLCD functions to draw player
* @param  pl : player struct
* @param _mod: MOVE or EXPLORE. player's color depends on it 
* @return None
*
**/
void draw_player(player_t pl, mode _mod){
	int i, j;
	int start = 0;
	int end = CELL_SIZE - 1; 
	
	int x0 = pl.position.y * CELL_SIZE + TABLE_START_X + 1;
	int y0 = pl.position.x * CELL_SIZE + TABLE_START_Y + 1;
	int color = _mod == MOVE ? MOVE_COLOR : EXPL_COLOR;
	
	switch(pl.direction){
		/* ---------- going right ----------*/
		case E:	
			for(i = 0; i < CELL_SIZE - 1; ++i){
				for(j = start; j < end; ++j)
					LCD_SetPoint(x0 + i, y0 + j, color);
					
				if(i % 2 == 0){
					start++;
					end--;
				}
			}
			break;
			
	/* ---------- going down ----------*/	
		case S:
			for(i = 0; i < CELL_SIZE - 1; ++i){
				for(j = start; j < end; ++j)
					LCD_SetPoint(x0 + j, y0 + i, color);
					
				if(i % 2 == 0){
					start++;
					end--;
				}
			}
			break;
			
		/* ---------- going left ----------*/	
		case W:
			start = CELL_SIZE - 2;
		  end = 0;
			
		  for(i = CELL_SIZE - 2; i >= 0 ; -- i){
				for(j = start; j >= end; -- j)
					LCD_SetPoint(x0 + i, y0 + j, color);
					
				if(i % 2 == 0){
					start --;
					end ++;
				}
			}
			break;
		   
		  
	 /* ----------- going up ------------*/	
		case N:
			start = CELL_SIZE - 2;
		  end = 0;
			
		  for(i = CELL_SIZE - 2; i >= 0 ; -- i){
				for(j = start; j >= end; -- j)
					LCD_SetPoint(x0 + j, y0 + i, color);
					
				if(i % 2 == 0){
					start --;
					end ++;
				}
			}
			break;	
		default: break;
		
 }
}


/**
* @brief employs GLCD functions to delete player
*		     useful to avoid redrawing everything when
*        player moves
* @param  pl: player struct
* @return None
**/
void delete_cell(cell_t cell){
		LCD_drawRectangle(TABLE_START_X + cell.y*CELL_SIZE + 1,
                    	TABLE_START_Y + cell.x*CELL_SIZE + 1,
                    	CELL_SIZE - 1, CELL_SIZE -1, White
	  );
}


/**
* @brief employs GLCD functions to draw obstacle
*				 at given position
* @param  cell : struct cell containing obstacle
* @return None
*
**/
void draw_obstacle(cell_t cell){
	LCD_drawRectangle(TABLE_START_X + cell.y*CELL_SIZE + 1,
                  	TABLE_START_Y + cell.x*CELL_SIZE + 1, 
	                  CELL_SIZE - 1, CELL_SIZE - 1, BLOCK_COLOR
	);
}


/**
* @brief marks exit when reached
* @param exit's cell
* @return None
**/
void draw_exit(cell_t cell){
	delete_cell(cell);
	LCD_drawRectangle(TABLE_START_X + cell.y*CELL_SIZE + 1,
                  	TABLE_START_Y + cell.x*CELL_SIZE + 1, 
	                  CELL_SIZE - 1, CELL_SIZE - 1, EXIT_COLOR
	);
}



/**
* @brief employs GLCD functions to draw restart
*				 and clear buttons 
* @param  None
* @return None
*
**/
void draw_buttons(){
	LCD_drawRectangle(RSTRT_START_X, RSTRT_START_Y, BTN_LEN, BTN_WID, BUTTON_COLOR);
	GUI_Text(20, 280, (uint8_t *)"Restart", Black, BUTTON_COLOR);
	
	LCD_drawRectangle(CLR_START_X, CLR_START_Y, BTN_LEN, BTN_WID, BUTTON_COLOR);
	GUI_Text(168, 280, (uint8_t *)"Clear", Black, BUTTON_COLOR);
}



