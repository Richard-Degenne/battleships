/*
 * game.c
 *
 * Richard Degenne - Adrien Deprez
 * 06/12/14
 *
 * Contains functions for the game mechanism itself.
 */

# include "game.h"

int main(int argc, char* argv[]) {
	grid primary, tracking;
	coord fire;
	boat fleet[FLEET_SIZE];
	int i;
	char response[MAX_REQ];

	setup_fleet(fleet);
	reset_grid(primary);
	reset_grid(tracking);
	
	for(i=0 ; i < FLEET_SIZE ; ++i) {
		print_grid(primary);
		printf(">> %s\n", fleet[i].name);
		select_boat_coord(&fleet[i], primary);
		place_boat(&fleet[i],primary);
		send_boat(&fleet[i], i);
	}
	while(1) {
		fire = select_fire_coord(tracking);
		send_fire(fire, response);
		printf("Received: \"%s\"\n", response);
		update_grid(tracking, response);
		print_grid(tracking);
	}
	return EXIT_SUCCESS;
}


/*
 * setup_fleet()
 *
 * Initializes parameters for each boat of the fleet. Positions are set to -1
 * and must be set before effectively placing the boat on the grid.
 */
void setup_fleet (boat fleet_p[]) {
	char* names[FLEET_SIZE] = {"Aircraft carrier", "Battleship", "Submarine", "Submarine", "Patrol boat", "Patrol boat"};
	int lenghts[FLEET_SIZE] = {5,4,3,3,2,2};
	int i;

	for(i=0 ; i<FLEET_SIZE ; ++i) { // For each boat
		strcpy(fleet_p[i].name, names[i]);
		fleet_p[i].lenght = lenghts[i];
		fleet_p[i].start.x = -1;
		fleet_p[i].start.y = -1;
		fleet_p[i].end.x = -1;
		fleet_p[i].end.y = -1;
	}
}


/*
 * select_boat_coord()
 * 
 * Prompts user for coordinates and applies them to a boat.
 */
void select_boat_coord(boat* boat_p, grid grid_p) {
	char buff_ch;
	int buff_int;
	int lenght = boat_p->lenght;
	coord computed[4];
	int available[4]; // 0: Unavailable, 1:Available
	int i,j;
	int x_start, y_start, x_end, y_end;
	int valid = 0; // Indicates that there is at least one available end coordinate

	do {
		for(i=0 ; i<4 ; ++i) {
			available[i] = 1;
		}

		// Select start coordinates
		boat_p->start.x = select_char_coord();
		boat_p->start.y = select_int_coord();

		// Compute end coordinates
			// To the right
		computed[0].x = boat_p->start.x + (lenght - 1);
		computed[0].y = boat_p->start.y;
			// To the top
		computed[1].x = boat_p->start.x;
		computed[1].y = boat_p->start.y - (lenght - 1);
			// To the left
		computed[2].x = boat_p->start.x - (lenght - 1);
		computed[2].y = boat_p->start.y;
			// To the bottom
		computed[3].x = boat_p->start.x;
		computed[3].y = boat_p->start.y + (lenght - 1);

		// Determine which computed coordinates are viable
		for(i=0 ; i<4 ; ++i) {
			x_start = min(boat_p->start.x, computed[i].x);
			x_end = max(boat_p->start.x, computed[i].x);
			y_start = min(boat_p->start.y, computed[i].y);
			y_end = max(boat_p->start.y, computed[i].y);

			// Check if the boat is in the grid
			if(x_end < 0 || x_end >= X_SIZE || y_start < 0 || y_end >= Y_SIZE) {
				available[i] = 0;
			}
			
			// Check for collisions
			if(x_start == x_end) { // Vertical
				for(j=y_start ; j<=y_end ; ++j) {
					if(grid_p[x_start][j] != EMPTY_SQ) {
						available[i] = 0;
					}
				}
			}
			else if(y_start == y_end) { // Horizontal
				for(j=x_start ; j<=x_end ; ++j) {
					if(grid_p[j][y_start] != EMPTY_SQ) {
						available[i] = 0;
					}
				}
			}
			else { // Not vertical nor horizontal
				perror("Invalid coordinates: Not horizontal nor vertical.");
				exit(EXIT_FAILURE);	
			}
		}
		// Check there is at least one available end coordinate
		for(i=0 ; i<4 ; ++i) {
			if(available[i]) {
				valid = 1;
			}
		}
	} while (!valid);
		
	printf("Select one of the following:\n");
	for(i=0 ; i<4 ; ++i) {
		if(available[i]) {
			printf("%d) %c%d\n", i, computed[i].x+65, computed[i].y);
		}
	}
	do {
		printf("Your choice: ");
		scanf("%d", &buff_int);
		flush();
	} while(buff_int < 0 || buff_int > 3 || !available[buff_int]);

	// Set end coordinate
	boat_p->end.x = computed[buff_int].x;
	boat_p->end.y = computed[buff_int].y;
}


/*
 * reset_grid()
 *
 * Sets every square of the grid to EMPTY_SQ.
 */
void reset_grid(grid grid_p) {
	int x,y;
	for(x=0 ; x<X_SIZE ; ++x) {
		for(y=0 ; y<Y_SIZE ; ++y) {
			grid_p[x][y] = EMPTY_SQ;
		}
	}
}


/*
 * place_boat()
 * 
 * Positions a boat on a grid. Sets every square covered by the boat with the
 * first letter of its name.
 */
void place_boat(boat* boat_p, grid grid_p) {
	int x_start = min(boat_p->start.x, boat_p->end.x);
	int x_final = max(boat_p->start.x, boat_p->end.x);
	int y_start = min(boat_p->start.y, boat_p->end.y);
	int y_final = max(boat_p->start.y, boat_p->end.y);

	int i;
	if(x_start == x_final) { // Vertical
		for(i=y_start ; i<=y_final ; ++i) {
			grid_p[x_start][i] = boat_p->name[0];
		}
	}
	else if(y_start == y_final) { // Horizontal
		for(i=x_start ; i<=x_final ; ++i) {
			grid_p[i][y_start] = boat_p->name[0];
		}
	}
	else { // Not vertical nor horizontal
		perror("Invalid coordinates: Not horizontal nor vertical.");
		exit(EXIT_FAILURE);
	}
}


/*
 * print_grid()
 *
 * Displays a grid in a pretty way.
 */
void print_grid(grid grid_p) {
	int x,y;
	printf("   | A | B | C | D | E | F | G | H | I | J |\n--------------------------------------------\n");
	for(y=0 ; y<Y_SIZE ; ++y) {
		printf(" %c ", 48+y);
		for(x=0 ; x<Y_SIZE ; ++x) {
			printf("| %c ",grid_p[x][y]);
		}
		printf("|\n--------------------------------------------\n");
	}
}


/*
 * update_grid()
 *
 * Updates the tracking grid according to a previous `FIRE` request.
 */
void update_grid(grid grid_p, char response_p[MAX_REQ]) {
	int x,y;
	char result[MAX_ARG];

	sscanf(response_p, "%s %d %d", result, &x, &y);
	if(x < 0 || x >= X_SIZE || y < 0 || y >= Y_SIZE) {
		printf("Inconsistent coordinates.\n");
		return;
	}
	if(!strcmp(result, "HIT")) { // If hit
		grid_p[x][y] = HIT_SQ;
	}
	else if(!strcmp(result, "MISS")) {
		grid_p[x][y] = MISS_SQ;
	}
	else {
		printf("Inconsistent command.\n");
		return;
	}
}

/*
 * send_boat()
 * 
 * Sets up a request and sends it to the host server.
 */
void send_boat(boat* boat_p, int id) {
	req_t request;
	
	request.type = PLACE_REQ;
	sprintf(request.args[0], "%d", id);
	sprintf(request.args[1], "%d", boat_p->start.x);
	sprintf(request.args[2], "%d", boat_p->start.y);
	sprintf(request.args[3], "%d", boat_p->end.x);
	sprintf(request.args[4], "%d", boat_p->end.y);

	send_request(&request, NULL);
}


/*
 * select_fire_coord()
 * 
 * Selects coordinates of a square the player wants to fire upon
 */
coord select_fire_coord(grid grid_p) {
	coord fire;
	do {
		fire.x = select_char_coord();
		fire.y = select_int_coord();
	} while(grid_p[fire.x][fire.y] != EMPTY_SQ);
	return fire;
}


/*
 * send_fire()
 *
 * Sets up a request and sends it to the host server.
 */
void send_fire(coord coord_p, char buff_p[MAX_REQ]) {
	req_t request;
	char buff[MAX_REQ];

	request.type = FIRE_REQ;
	sprintf(request.args[0], "%d", coord_p.x);
	sprintf(request.args[1], "%d", coord_p.y);

	send_request(&request, buff_p);
}


/*
 * select_char_coord()
 *
 * Returns 0 for A or a, 1 for B or b, ... 9 for J or j. Loops else.
 */
int select_char_coord() {
	int x;
	char x_c;
	do {
		printf("Select x: ");
		scanf("%c", &x_c);
		flush();
		x = toupper(x_c) - 65;
	} while(x < 0 || x > X_SIZE);
	return x;
}

/*
 * select_int_coord()
 *
 * Returns any int >= 0 and < Y_SIZE
 */
int select_int_coord() {
	int y;
	do {
		printf("Select y: ");
		scanf("%d", &y);
		flush();
	} while(y < 0 || y > Y_SIZE);
	return y;
}


/*
 * flush()
 *
 * Flushes stdin.
 */
void flush() {
	char c;
	do {
		c = getchar();
	} while(c != '\n' && c != EOF);
}
