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
# include "network.h"

// Defines and macros
# define FLEET_SIZE 6
# define X_SIZE 10
# define Y_SIZE 10

# define EMPTY_SQ '.'
# define MISS_SQ '—'
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
void send_boat(boat*, int);
coord select_fire_coord(grid);
void send_fire(coord);

// Misc
int select_char_coord();
int select_int_coord();
void flush();


# endif // _GAME_H defined
