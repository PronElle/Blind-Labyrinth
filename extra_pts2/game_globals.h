/* dimensions */
#define ROW 		13              /* number of rows    */
#define COL 		15							/* number of columns */

#define B 			1  							/* block */
#define X 			2  							/* exit */

/* step period in multiples of 50 ms*/
#define RUN_T    0x14  /* 20*50 ms = 1 s  */


/*starting line conditions*/
#define STARTING_X   	7      					
#define STARTING_Y   	7
#define DIR    				E

#define IS_NOT_OBSTACLE(x,y) table[x][y] != B 

typedef enum {N, E, S, W} dir; /* East, South, West, North, clockwise ordered*/ 
typedef enum {EXPLORE, MOVE} mode;

typedef struct cell{
	int x;
	int y; 
}cell_t;

typedef struct player{
	 cell_t position; 
	 dir direction: 4; 
} player_t;

