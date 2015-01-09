/*
 * game.c
 *
 * Richard Degenne - Adrien Deprez
 * 06/12/14
 *
 * Contains functions for the game mechanism itself.
 */

# include "game.h"
# include "overall_net.h"

int sfd;
grid opponent;

int main(int argc, char* argv[]) {
	int sfd_s, sfd_l; // s: server, l: listening
	opponent_t connected;
	int games_count;
	char name[MAX_NAME];
	int mode = argc - 1, win = 0;
	grid primary, tracking;
	coord fire;
	boat fleet[FLEET_SIZE];

	int i;
	pthread_t th;
	
	if(mode) {
		strcpy(name,"Joining");
	}
	else {
		strcpy(name,"Hosting");
	}
	game_t games[MAX_GAMES];

	
	if(mode) { // JOINING PLAYER
		sign_in(name, &sfd_s, &sfd_l, MODE_JOIN); 
		printf("Press enter to scan games...\n");
		getchar();

		games_count = get_games(sfd_s, games);
		printf("%d games received.\n", games_count);
		print_games(games, games_count);

		int selected;
		printf("Select a game: ");
		scanf("%d", &selected);
		flush();

		connect_player(games[selected], &(connected.sfd), sfd_s);
		send_name(connected.sfd, name);
		wait_start(sfd_s, connected.sfd);
	}
	else { // HOST PLAYER
		sign_in(name, &sfd_s, &sfd_l, MODE_HOST); 
		printf("Waiting for an opponent to connect...\n");
		accept_player(sfd_l, &connected);
		wait_name(&connected);
		printf("Ready? Press [ENTER] to start the game.");
		getchar();
		send_start(sfd_s, connected);
	}
	printf("Game starts!\n===============================\n");
	sfd = connected.sfd; // Make the created sfd global

	setup_fleet(fleet);
	printf("Fleet set up.\n");
	reset_grid(primary);
	reset_grid(tracking);
	reset_grid(opponent);
	printf("Grids reset.\n");
	if(!mode) { // Host
		pthread_create(&th, NULL, receive_boat, NULL);
		printf("Thread created.\n");
	}
	
	for(i=0 ; i < FLEET_SIZE ; ++i) {
		print_grid(primary);
		printf(">> %s (%d)\n", fleet[i].name, fleet[i].id);
		select_boat_coord(&fleet[i], primary);
		place_boat(&fleet[i],primary);
		if(mode) { // Client
			send_boat(&fleet[i], i);
		}
	}
	if(!mode) { // Host
		pthread_join(th, NULL);
		printf("Thread joined.\n");
		while(1) {
			// Host turn
			print_grid(primary);
			printf("Primary\n——————————————————————————\n");
			print_grid(tracking);
			printf("Tracking\n——————————————————————————\n");
			printf("It's your turn!\n");
			fire = select_fire_coord(tracking);
			check_fire(fire, opponent, tracking);
			if(check_win(tracking, &fleet)) {
				send_win(0, sfd);
				break;
			}
			printf("End of your turn.\nWaiting for the opponent to fire...\n");
			// Client turn
			fire = wait_fire();
			check_fire(fire, primary, primary);
			if(check_win(primary, &fleet)) {
				send_win(1, sfd);
				break;
			}
		}
	}
	else { // Client
		while(1) {
			// Host turn
			printf("Waiting for the opponent to fire...\n");
			if(receive_fire(primary)) {
				printf("You lost!\n");
				break;
			}
			// Client turn
			print_grid(primary);
			printf("Primary\n——————————————————————————\n");
			print_grid(tracking);
			printf("Tracking\n——————————————————————————\n");
			printf("It's your turn!\n");
			fire = select_fire_coord(tracking);
			send_fire(fire);
			if(receive_fire(tracking)) {
				printf("You won!\n");
				break;
			}
			
			printf("End of your turn.\n");
		}
	}
	return EXIT_SUCCESS;
}
