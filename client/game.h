/*
 * game.h
 *
 * Richard Degenne - Adrien Deprez
 * 06/12/14
 *
 * Header for the game.c file.
 */

# ifndef _GAME_H
# define _GAME_H 1


// Includes
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <pthread.h>
# include "network.h"

// Defines and macros
# define FLEET_SIZE 1
# define X_SIZE 10
# define Y_SIZE 10

# define EMPTY_SQ '.'
# define MISS_SQ 'O'
# define HIT_SQ 'X'

# define max(a,b) (((a)>(b)) ? (a) : (b))
# define min(a,b) (((a)<(b)) ? (a) : (b))

// Typedefs and structs
typedef char grid[X_SIZE][Y_SIZE];
typedef struct coord {
	int x;
	int y;
} coord;
typedef struct boat {
	int id;
	char name[30];
	int lenght;
	coord start, end;
} boat;

// Prototypes
void setup_fleet(boat[]);
void select_boat_coord(boat*, grid);
void reset_grid(grid);
void place_boat(boat*, grid);
void print_grid(grid);
void update_grid(grid, char*);
void send_boat(boat*, int);
coord select_fire_coord(grid);
void check_fire(coord, grid, grid);
coord wait_fire();
int receive_fire(grid);
void send_fire(coord);
int check_win(grid, boat*);
void send_win(int);

// Misc
int select_char_coord();
int select_int_coord();
void flush();
int sunk(char, grid);

// Thread routine
void* receive_boat(void*);

# endif // _GAME_H defined
