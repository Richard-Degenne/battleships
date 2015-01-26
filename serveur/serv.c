/*
* serv.c
*
* Richard Degenne - Adrien Deprez
* 09/12/14
*
 * Contains functions for the host server.
 */

#define PLAYER1_ADR "192.168.1.1" 
#define PLAYER2_ADR "192.168.2.2"

#include "serv.h"

int main()
{
	boat fleet1[6];
	boat fleet2[6];
	int placed_boats = 0;
	grid grid1,grid2;

	while(1)
	{
		if (!strcmp(/*request name*/,"PLACE"))
		{
			if ( /* player IP */ = PLAYER1_ADR)
			{
				add_boat();
				placed_boats ++;
			}
			else
			{
				add_boat();
				placed_boats ++;
			}
			if(placed_boats == 12)
			{
				/*TURN player 1*/
			}
		}
		else if (!strcmp(/*request name*/,"CHAT"))
		{

		}
		else
		{

		}
	}
}

void add_boat(int boat, int s_x, int s_y, int e_x, int s_y)
{
	strcpy(fleet1[boat].name,boatname[boat]);
	fleet1[boat].start.x = s_x;
	fleet1[boat].start.y = s_y;
	fleet1[boat].end.x = e_x;
	fleet1[boat].end.y = e_y;
}
