/**
 * \file	client/main.c
 * \brief	Main program
 *
 * \author	Richard Degenne
 * \date	12-06-14
 *
 */

# include "game.h"
# include "overall_net.h"

int sfd;//!< Dialog socket file descriptor between players
grid opponent;//!< Opponent grid for the host

/**
 * \brief	The client's main function.
 * \details	This functions coordinates the different libraries to create the actual game. It will prompt the client for the server's address, the player's nickname and if they want to host or join a game.
 * \param	argc	Command-line arguments count. Not used.
 * \param	argv	Command-line arguments. Not used.
 * \return	`EXIT_FAILURE` in case of error, `EXIT_SUCCESS` otherwise.
 */
int main(int argc, char* argv[]) {
	int sfd_s, sfd_l; // s: server, l: listening
	opponent_t connected;
	int games_count;
	char name[MAX_NAME];
	char addr_str[16];
	int mode = argc - 1, win = 0;
	grid primary, tracking;
	coord fire;
	boat fleet[FLEET_SIZE];

	int i;
	pthread_t th;
	
	// Information prompts
	printf("Server address: ");
	scanf("%s", addr_str);
	flush();
	printf("Nickname: ");
	scanf("%s", name);
	flush();

	do {
		printf("0. Host\n1. Join\n");
		scanf("%d", &mode);
		flush();
	} while(mode != 0 && mode != 1);
	
	game_t games[MAX_GAMES];
	
	// Sign in to the server
	sign_in(name, addr_str, &sfd_s, &sfd_l, mode); 
	if(mode) { // JOINING PLAYER
		printf("Press [ENTER] to scan games.\n");
		getchar();
		
		// Fetch and select a game
		games_count = get_games(sfd_s, games);
		printf("%d games received.\n", games_count);
		print_games(games, games_count);

		int selected;
		do {
			printf("Select a game: ");
			scanf("%d", &selected);
			flush();
		} while(selected >= games_count || games[selected].status != AVAILABLE_ST);

		connect_player(games[selected], &connected, sfd_s);
		send_name(connected.sfd, name);
	printf("Waiting for the host to start the game...\n");
		wait_start(connected.sfd);
	}
	else { // HOST PLAYER
		printf("Waiting for an opponent to connect...\n");
		accept_player(sfd_l, &connected);
		wait_name(&connected);
		printf("Ready? Press [ENTER] to start the game.");
		getchar();
		
		send_start(sfd_s, connected);
	}
	printf("Game starts!\n===============================\n");
	sfd = connected.sfd; // Make the created socket file descriptor global

	setup_fleet(fleet);
	reset_grid(primary);
	reset_grid(tracking);
	reset_grid(opponent);
	if(!mode) { // Host
		pthread_create(&th, NULL, receive_boat, NULL);
	}
	
	for(i=0 ; i < FLEET_SIZE ; ++i) {
		print_grid(primary);
		printf(">> %s \n", fleet[i].name);
		select_boat_coord(&fleet[i], primary);
		place_boat(&fleet[i],primary);
		if(mode) { // Client
			send_boat(&fleet[i], i);
		}
	}
	if(!mode) { // Host
		printf("Waiting for the opponent to finish placing his boats...\n");
		pthread_join(th, NULL);
		while(1) {
			// Host turn
			print_grid(primary);
			printf("%s\n——————————————————————————\n", name);
			print_grid(tracking);
			printf("%s\n——————————————————————————\n", connected.name);
			printf("It's your turn!\n");
			fire = select_fire_coord(tracking);
			check_fire(fire, opponent, tracking);
			if(check_win(opponent, fleet)) {
				send_win(0);
				printf("+----------+\n| You won! |\n+----------+\n");
				break;
			}
			print_grid(primary);
			printf("Primary\n——————————————————————————\n");
			print_grid(tracking);
			printf("Tracking\n——————————————————————————\n");
			printf("End of your turn.\nWaiting for the opponent to fire...\n");
			// Client turn
			fire = wait_fire();
			check_fire(fire, primary, primary);
			if(check_win(primary, fleet)) {
				send_win(1);
				printf("+-----------+\n| You lost! |\n+-----------+\n");
				break;
			}
		}
	}
	else { // Client
		while(1) {
			// Host turn
			print_grid(primary);
			printf("%s\n——————————————————————————\n", name);
			print_grid(tracking);
			printf("%s\n——————————————————————————\n", connected.name);
			printf("Waiting for the opponent to fire...\n");
			if(receive_fire(primary)) {
				printf("+-----------+\n| You lost! |\n+-----------+\n");
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
				printf("+----------+\n| You won! |\n+----------+\n");
				break;
			}
			
			printf("End of your turn.\n");
		}
	}
	printf("Thanks for playing Battleships! Check http://richarddegenne.wordpress.com for more about this game!\n");
	return EXIT_SUCCESS;
}
