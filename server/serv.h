/*
 * serv.h
 *
 * Richard Degenne - Adrien Deprez
 * 09/12/14
 *
 * Header for the serv.c file.
 */

// Macrofunctions
#define max(a,b) (((a)>(b)) ? (a) : (b))
#define min(a,b) (((a)<(b)) ? (a) : (b))
#define X_SIZE 10
#define	Y_SIZE 10

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Definitions
typedef int grid[X_SIZE][Y_SIZE];
typedef struct coord {
	int x;
	int y;
} coord;
typedef struct boat {
	char name[30];
	coord start, end;
}boat;

//Prototypes
void add_boat(int,int,int,int,int); // Add a boat for a player at the target location
