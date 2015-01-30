/**
 * \file	game.h
 * \brief	Header for the game.c file.
 *
 * \author	Richard Degenne
 * \date	12-06-14
 *
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

/**
 * \brief Number of boats in the float
 */
# define FLEET_SIZE 6

/**
 * \brief	The grid's x-size (A..J)
 * \warning	Please note that the current graphics will not update to follow the actual setting, but the game mecanism will.
 */
# define X_SIZE 10

/**
 * \brief	The grid's y-size (0..9)
 * \warning	Please note that the current graphics will not update to follow the actual setting, but the game mecanism will.
 */
# define Y_SIZE 10

/**
 * \brief	Graphical visualisation of an empty square.
 */
# define EMPTY_SQ ' '

/**
 * \brief	Graphical visualisation of a square where a shot missed.
 */
# define MISS_SQ '~'

/**
 * \brief	Graphical visualisation of a square where a shot hit.
 */
# define HIT_SQ 'X'

/**
 * \brief	Returns the maximum of two comparable values
 */
# define max(a,b) (((a)>(b)) ? (a) : (b))

/**
 * \brief	Return the minimum of two comparable values
 */
# define min(a,b) (((a)<(b)) ? (a) : (b))

// Typedefs and structs

/**
 * \brief	Grid type.
 * \details	A grid is actually a 2-d array of `char`. The status of each square is define by its current character.
 * \see	EMPTY_SQ
 * \see	MISS_SQ
 * \see	HIT_SQ
 */
typedef char grid[X_SIZE][Y_SIZE];

/**
 * \brief	A 2-d coordinate structure
 */
typedef struct coord {
	int x; //!< x-coordinate
	int y; //!< y-coordinate
} coord;

/**
 * \brief	A structure describing boats.
 * \details	The boat being the main element of the game, this structure is used in many ways.
 */
typedef struct boat {
	int id; //!< Unique number given to each boat
	/**
	 * \brief	Name of the boat.
	 * \see	setup_fleet
	 */
	char name[30];
	int lenght; //!< The boat's lenght, in squares.
	coord start; //!< Position of the first end of the boat;
	coord end; //!< Position of the other end of the boat.
} boat;

// Prototypes

/**
 * \brief	Sets the fleet up.
 * \details	Initializes parameters for each boat of the fleet. Positions are set to -1 and must be set before effectively placing the boat on the grid.
 * \param	fleet_p	The fleet to set up.
 */
void setup_fleet(boat fleet_p[]);

/**
 * \brief	Prompts user for a boat's coordinates and applies it to a boat.
 * \details	The user will first be prompted for the boat's starting x-coordinate. If their input is consistent, they will be prompted for the starting y-coordinate. If a boat is already present on the square, the user will be prompted for new coordinates. Else, this function will compute the available end coordinates (available here means that the boat doesn't go over the grid's limits and doesn't cross another boat). The user will finally be prompted to select one of the available end coordinate. Upon selection, the boat end coordinate is set accordingly.
 * \param	boat_p	The boat to prompt the user for.
 * \param	grid_p	The grid to use for collision detection
 */
void select_boat_coord(boat* boat_p, grid grid_p);

/**
 * \brief	Resets a grid.
 * \details	Sets every square of the grid to EMPTY_SQ.
 * \param	grid_p	The grid to reset
 */
void reset_grid(grid grid_p);

/**
 * \brief	Places a boat on a grid
 * \details	Sets every square covered by the boat with its id number.
 * \param	boat_p	The boat to place
 * \param	grid_p	The grid to place the boat on
 */
void place_boat(boat* boat_p, grid grid_p);

/**
 * \brief	Prints a grid.
 * \details	Prints the grid in a pretty way.
 * \param	grid_p	The grid to print
 * \warning	Please note that the heading row and column will not follow any changes on X_SIZE and Y_SIZE.
 * \see X_SIZE
 * \see Y_SIZE
 */
void print_grid(grid grid_p);

/**
 * \brief	Sends a boat's information through a socket.
 * \details	For the joining player, sets up and sends a request to the host to tell the boat's position.
 * \param	boat_p	The boat which infos are to be sent
 * \param	id	The boat's id.
 */
void send_boat(boat* boat_p, int id);

/**
 * \brief	Prompts the user for a coordinate to fire upon.
 * \details	The user will be prompted again until the selected coordinates is an empty square. This way, he/she won't fire twice upon the same square.
 * \param	grid_p	The grid to check the selected coordinate's relevance on.
 * \return	The first consistent selected coordinate
 */
coord select_fire_coord(grid grid_p);

/**
 * \brief	Checks the result of a shot.
 * \details 	Checks whether the shot does not land on an empty square on `target`, and whether if it sinks a boat. Updates `display` accordingly, prints out a log, and sends a report to the other player.
 * \param	fire	Coordinate of the shot
 * \param	target	The grid on which to fire
 * \param	display	The grid on which to update the display.
 */
void check_fire(coord fire, grid target, grid display);

/**
 * \brief	Waits for the other player to fire.
 * \details	Actually, this function waits for a `FIRE` request.
 * \return	The coordinate of the opponent's shot.
 */
coord wait_fire();

/**
 * \brief	Waits fot the host to send a report for a previous shot.
 * \details	Reports can be send in 1, 2 or 3 requests. An `OK` acknowledgement is sent after each. Udpates `target` accordingly and prints out a log.
 * \param	target	The grid to update
 * \return	1 if the game is over, 0 otherwise
 */
int receive_fire(grid target);

/**
 * \brief	Sends a fire.
 * \details	Sets up a `FIRE` request and sends it to the host player.
 * \param	coord_p	The shot's coordinates
 */
void send_fire(coord coord_p);

/**
 * \brief	Check whether a player has won.
 * \details	Look up for every boat in `fleet`. If none is found, the fleet has entirely been sunk.
 * \param	target	The grid to check
 * \param	fleet	Boats to look up
 * \returns	1 if the entire fleet has been sunk, 0 otherwise.
 */
int check_win(grid target, boat* fleet);

/**
 * \brief	Tells the other player he has won or lost.
 * \details	Sets up a `WIN` or `LOSE` and sets it to the other player.
 * \param	mode	0 for `LOSE`, any other value for `WIN`.
 */
void send_win(int mode);

// Miscalleneous

/**
 * \brief	Prompts the user for a coordinate parsing the character as a letter.
 * \details	Prompts the user for a character between A or J. Case insensitive. Loops if the input is inconsistent.
 * \return	0 for (A,a), 1 for (B,b)... 9 for (J,j)
 */
int select_char_coord();

/**
 * \brief	Prompts the user for a coordinate parsing the character as a number.
 * \details	Prompts the user for a character between '0' or '9'.  Loops if the input is inconsistent.
 * \return	The integer value matching the character given in input.
 */
int select_int_coord();

/**
 * \brief	Flushes `stdin`.
 */
void flush();

/**
 * \brief	Checks whether a boat has been sunk.
 * \details	Looks up the character corresponding to a boat in `target`.
 * \param	id	Id character of the boat to look for.
 * \param	target	Grid to search the boat
 * \return	1 if the boat is sunk, 0 otherwise.
 */
int sunk(char id, grid target);

// Thread routine

/**
 * \brief	Waits for the opponent to send their boats and place them into a grid.
 * \details	This is a thread routine so the joining player can asynchronously send their boats to the host. Received boats are placed in `opponent`, a global grid.
 * \param	arg	Dummy argument.
 * \return	NULL, dummy return value.
 */
void* receive_boat(void* arg);

# endif // _GAME_H defined
