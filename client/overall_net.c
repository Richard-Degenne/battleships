/*
 * overall_net.c
 *
 * Richard Degenne - Adrien Deprez
 * 15/12/14
 *
 * Overall network implementation.
 */

# include "overall_net.h"

/*
 * sign_in()
 *
 * Creates a listening socket, connects the main server and sends it the address of the listening socket.
 */
void sign_in(char* nick, int* sfd_server, int* sfd_listen) {
	struct sockaddr_in addr, addr_s, addr_l;
	int len = sizeof(addr_l);
	char buff[MAX_REQ];

	// Creating listening socket
	addr.sin_family = AF_INET;
	addr.sin_port = 0; // Automatic port
	addr.sin_addr.s_addr = INADDR_ANY; // All connected interfaces
	check(*sfd_listen = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	check(bind(*sfd_listen, (struct sockaddr*)&addr, sizeof(addr)), "Error binding");
	check(listen(*sfd_listen, 1), "Error listening");
	check(getsockname(*sfd_listen, (struct sockaddr*)&addr_l, &len), "Error getting socket name");
	printf("Listening socket created.\n");

	// Connecting to server
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_ADDR, &(addr_s.sin_addr));

	check(*sfd_server = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	check(connect(*sfd_server, (struct sockaddr*)&addr_s, sizeof(addr_s)), "Error connecting");
	printf("Connected to server.\n");

	// Sending listening socket info
	printf("Nick: \"%s\"\nPort:\"%d\"\n", nick, ntohs(addr_l.sin_port));
	sprintf(buff, "INFO %s %d", nick, ntohs(addr_l.sin_port)); // INFO <nick> <port>
	printf("Request built.\n");
	check(send(*sfd_server, buff, strlen(buff)+1, 0), "Error sending");
	printf("Sent: \"%s\"\n", buff);
}


/*
 * get_games()
 *
 * Ask the server for the currently available games. Returns the number of
 * received games.
 */
int get_games(int sfd, game_t tab_p[]) {
	char buff[MAX_REQ], buff_addr[MAX_ARG];
	int buff_port;
	int i=-1;
	
	// Sending a GAMES request
	check(send(sfd, "GAMES", strlen("GAMES")+1, 0), "Error sending");
	printf("Sent: \"GAMES\"\n");

	// Receiving GAME responses
	do {
		i++;
		check(recv(sfd, buff, MAX_REQ, 0), "Error receiving");
		printf("Game recieved: \"%s\"\n", buff);
		sscanf(buff, "GAME %s %s %d", tab_p[i].name, buff_addr, &buff_port);
		inet_aton(buff_addr, &(tab_p[i].addr.sin_addr));
		tab_p[i].addr.sin_port = htons(buff_port);
		tab_p[i].addr.sin_family = AF_INET;
	} while(strcmp(buff, "STOP") && i<MAX_GAMES);
	return i;
}


/*
 * print_games()
 *
 * Prints all received games.
 */
void print_games(game_t tab_p[], int n) {
	int i;
	for(i=0 ; i<n ; ++i) {
		printf("%s\t| %s\t| %d\n", tab_p[i].name, inet_ntoa(tab_p[i].addr.sin_addr), ntohs(tab_p[i].addr.sin_port));
	}
}


/*
 * connect_player()
 * 
 * Creates a dialog socket to the selected player. Returns the created socket file descriptor.
 */
void connect_player(game_t game_p, int* sfd_p) {
	check(*sfd_p = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	check(connect(*sfd_p, (struct sockaddr*)&(game_p.addr), sizeof(game_p.addr)), "Error connecting");
	printf("Client connected to %s\n", game_p.name);
}


/*
 * accept_player()
 *
 * Waits for another player to connect.
 */
void accept_player(int sfd_l, opponent_t* opponent_p) {
	opponent_p->addr_len = sizeof(opponent_p->addr);
	check(opponent_p->sfd = accept(sfd_l, (struct sockaddr*)&(opponent_p->addr), &(opponent_p->addr_len)), "Error accepting");
	printf("Accepted a player.\n");
}


/*
 * wait_name()
 *
 * Waits for the joining player to send his/her name
 */
void wait_name(opponent_t* opponent_p) {
	char buff[MAX_REQ];

	check(recv(opponent_p->sfd, buff, MAX_REQ, 0), "Error receiving");
	sscanf(buff, "NAME %s", opponent_p->name);
	printf("Opponent name is \"%s\"\n", opponent_p->name);
}


/*
 * send_name()
 *
 * Sends player's name to host.
 */
void send_name(int sfd_p, char* name_p) {
	char buff[MAX_REQ] = "";
	strcat(buff, "NAME ");
	strcat(buff, name_p);
	check(send(sfd_p, buff, strlen(buff)+1, 0), "Error sending.");
	printf("Sent: \"%s\"\n", buff);
}


int main(int argc, char* argv[]) {
	int sfd_s, sfd_l; // s: server, l: listening
	opponent_t opponent;
	int games_count;
	char name[] = "Richou";
	game_t games[MAX_GAMES];

	sign_in(name, &sfd_s, &sfd_l); 
	
	printf("Press enter to scan games...\n");
	getchar();

	games_count = get_games(sfd_s, games);
	printf("%d games received.\n", games_count);
	print_games(games, games_count);

	if(argc > 1) { // JOINING PLAYER
		// FIRST TRY
		int selected;
		printf("Select a game: ");
		scanf("%d", &selected);

		connect_player(games[selected], &(opponent.sfd));
		send_name(opponent.sfd, name);
	}
	else { // HOST PLAYER
		accept_player(sfd_l, &opponent);
		wait_name(&opponent);
	}
	return EXIT_SUCCESS;
}
