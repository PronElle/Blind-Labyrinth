#include <stdbool.h>
#include "lpc17xx.h"
#include <string.h>
#include "GUI.h"

void game_init(void);
void start_game(void);

void player_move(void);
bool consented_move(void);

cell_t next_cell (cell_t c);
bool valid_cell (cell_t c);

bool exit_reached(void);
void victory_setup(void);

void change_mode(void);
void player_changeDir(dir _dir); 
void reveal_obstacles(void);

// button functions
void restart(void);
void clear(void);

