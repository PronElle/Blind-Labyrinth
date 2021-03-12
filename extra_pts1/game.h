#include <stdbool.h>
#include "lpc17xx.h"

/*used LEDs*/
#define RUN_LED 5  							/* Running indicator LED*/
#define N_LED 	3  							/* North direction LED  */
#define E_LED 	2  							/* East direction LED   */
#define S_LED 	1  							/* South direction LED  */
#define W_LED 	0	 							/* West direction LED   */

/* dimensions */
#define ROW 		13              /* number of rows    */
#define COL 		15							/* number of columns */

#define B 			1  							/* block */
#define X 			2  							/* exit */

/*position LEDs scaling factors*/  
#define K53     0x5F5E10 				/* scaling factor for 2Hz blink frequency @ Dc 50% */
#define K21     0x2FAF08 				/* scaling factor for 4Hz blink frequency @ Dc 50% */ 
#define K0      0x17D784	 			/* scaling factor for 8Hz blink frequency @ Dc 50% */
#define K_WIN   0xBEBC20        /* scaling factor for 1Hz blink frequency @ Dc 50% */

/* running LED periods (in multiples of 50 ms)*/
#define RUN_T    0x28  /* 40*25 ms =   1 s  => f = 1 Hz*/
#define BLOCK_T  0x8   /*  8*25 ms = 200 ms => f = 5 Hz*/

/*starting line conditions*/
#define STARTING_X   	7      					
#define STARTING_Y   	7
#define DIR    				E

typedef enum {N, E, S, W} dir; /* East, South, West, North, clockwise ordered*/ 

typedef struct cell{
	int x;
	int y; 
}cell_t;

struct player{
	 cell_t position; 
	 dir direction: 4; 
};


void game_init(void);
void start_game(void);

void player_move(void);
void player_rotate(void);

uint8_t getDistance(void);
bool consented_move(void);
void update_sensor_blink(void);

cell_t next_cell (cell_t c);
bool valid_cell (cell_t c);

bool exit_reached(void);
void victory_setup(void);


