/**
 * \file	overall_net.c
 * \brief	Overall network implementation.
 *
 * \author	Richard Degenne
 * \date	12-15-14
 *
 */

# include "overall_net.h"


void sign_in(char* nick, char* addr_str, int* sfd_server, int* sfd_listen, int mode) {
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

	// Connecting to server
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);
	inet_aton(addr_str, &(addr_s.sin_addr));

	check(*sfd_server = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	check(connect(*sfd_server, (struct sockaddr*)&addr_s, sizeof(addr_s)), "Error connecting");

	// Sending listening socket info
	sprintf(buff, "HOST %s %d %d", nick, ntohs(addr_l.sin_port), mode); // HOST <nick> <port> <mode>
	check(send(*sfd_server, buff, strlen(buff)+1, 0), "Error sending");
}


int get_games(int sfd, game_t tab_p[]) {
	char buff[MAX_REQ], buff_addr[MAX_ARG];
	int buff_port;
	int i=-1;
	
	// Sending a GAMES request
	check(send(sfd, "GAMES", strlen("GAMES")+1, 0), "Error sending");

	// Receiving GAME responses
	do {
		i++;
		check(recv(sfd, buff, MAX_REQ, 0), "Error receiving");
		sscanf(buff, "GAME %s %s %d %d", tab_p[i].name, buff_addr, &buff_port, &(tab_p[i].status));
		inet_aton(buff_addr, &(tab_p[i].addr.sin_addr));
		tab_p[i].addr.sin_port = htons(buff_port);
		tab_p[i].addr.sin_family = AF_INET;
		check(send(sfd, "OK", strlen("OK"), 0), "Error sending");
	} while(strcmp(buff, "STOP") && i<MAX_GAMES);
	return i;
}


void print_games(game_t tab_p[], int n) {
	int i;
	printf("#\t| Name\t| Address\t| Port\t| Available?\n======================================================\n");
	for(i=0 ; i<n ; ++i) {
		printf("%d\t| %s\t| %s\t| %d\t| %s\n", i, tab_p[i].name, inet_ntoa(tab_p[i].addr.sin_addr), ntohs(tab_p[i].addr.sin_port), (tab_p[i].status == AVAILABLE_ST)?"Yes":"No");
	}
}


void connect_player(game_t game_p, opponent_t* o, int sfd_s) {
	char buff[MAX_REQ] = "";
	strcpy(o->name, game_p.name);
	o->addr = game_p.addr;

	// Connect to host
	check(o->sfd = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	check(connect(o->sfd, (struct sockaddr*)&(o->addr), sizeof(o->addr)), "Error connecting");
	printf("You are connected to %s\n", game_p.name);

	// Send a JOIN request to the server
	sprintf(buff, "JOIN %s %s %d", game_p.name, inet_ntoa(game_p.addr.sin_addr), ntohs(game_p.addr.sin_port));
	check(send(sfd_s, buff, strlen(buff)+1, 0), "Error sending");
}


void accept_player(int sfd_l, opponent_t* opponent_p) {
	opponent_p->addr_len = sizeof(opponent_p->addr);
	check(opponent_p->sfd = accept(sfd_l, (struct sockaddr*)&(opponent_p->addr), &(opponent_p->addr_len)), "Error accepting");
}


void send_name(int sfd_p, char* name_p) {
	char buff[MAX_REQ] = "";
	strcat(buff, "NAME ");
	strcat(buff, name_p);
	check(send(sfd_p, buff, strlen(buff)+1, 0), "Error sending.");
}


void wait_name(opponent_t* opponent_p) {
	char buff[MAX_REQ];

	check(recv(opponent_p->sfd, buff, MAX_REQ, 0), "Error receiving");
	sscanf(buff, "NAME %s", opponent_p->name);
	printf("%s has joined you.\n", opponent_p->name);
}


void send_start(int sfd_s, opponent_t o) {
	char buff[MAX_REQ] = "";
	
	strcat(buff, "START");
	check(send(sfd_s, buff, strlen(buff)+1, 0), "Error sending");
	check(send(o.sfd, buff, strlen(buff)+1, 0), "Error sending");
}


void wait_start(int sfd_o) {
	char buff[MAX_REQ] = "";

	do {
		check(recv(sfd_o, buff, MAX_REQ, 0), "Error receiving");
	} while(strcmp(buff, "START"));
}
