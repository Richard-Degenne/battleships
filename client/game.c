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
	coord start, end;
	boat fleet[FLEET_SIZE];
	int i;

	setup_fleet(fleet);
	reset_grid(primary);
	reset_grid(tracking);
	
	for(i=0 ; i < FLEET_SIZE ; ++i) {
		print_grid(primary);
		printf(">> %s\n", fleet[i].name);
		select_boat_coord(&fleet[i]);
		place_boat(&fleet[i],primary);
	}
	print_grid(primary);
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
 * select_boat_coord()
 * 
 * Prompts user for coordinates and applies them to a boat.
 */
void select_boat_coord(boat* boat_p) {
	char buff_ch;
	int buff_int[3];
	int lenght = boat_p->lenght;
	coord computed[4];
	int available[4] = {0,0,0,0}; // 0: Unavailable, 1:Available
	int i;

	// Start x-coordinate
	do {
		printf("Select start x: ");
		scanf("%c", &buff_ch);
		flush();
		buff_int[0] = toupper(buff_ch) - 65;
	} while(buff_int[0] < 0 || buff_int[0] > X_SIZE);
	printf("X: %d\n",buff_int[0]);
	boat_p->start.x = buff_int[0];

	// Start y-coordinate
	do {
		printf("Select start y: ");
		scanf("%d", &buff_int[1]);
		flush();
	} while(buff_int[1] < 0 || buff_int[1] > Y_SIZE);

	boat_p->start.y = buff_int[1];

	// Compute end coordinates
		// To the right
	computed[0].x = buff_int[0] + (lenght - 1);
	computed[0].y = buff_int[1];
		// To the top
	computed[1].x = buff_int[0];
	computed[1].y = buff_int[1] - (lenght - 1);
		// To the left
	computed[2].x = buff_int[0] - (lenght - 1);
	computed[2].y = buff_int[1];
		// To the bottom
	computed[3].x = buff_int[0];
	computed[3].y = buff_int[1] + (lenght - 1);

	// Select one the end coordinates
	printf("Select one of the following:\n");
	for(i=0 ; i<4 ; ++i) {
		if(computed[i].x >= 0 && computed[i].x < X_SIZE &&
		computed[i].y >= 0 && computed[i].y < Y_SIZE) {
			printf("%d) %c%d\n", i, 65+computed[i].x, computed[i].y);
			available[i] = 1;
		}
	}
	do {
		printf("Your choice: ");
		scanf("%d", &buff_int[3]);
		flush();
	} while(buff_int[3] < 0 || buff_int[3] > 3 || !available[buff_int[3]]);

	boat_p->end.x = computed[buff_int[3]].x;
	boat_p->end.y = computed[buff_int[3]].y;
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
