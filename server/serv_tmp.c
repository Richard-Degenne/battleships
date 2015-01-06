/*
 * serv_tmp.c
 *
 * Richard Degenne - Adrien Deprez
 * 10/12/14
 * 
 * Temporary server, only for test purposes.
 */

# include "serv_tmp.h"

int main(int argc, char* argv[]) {
	int sfd_s, sfd_c;
	struct sockaddr_in addr_s, addr_c;
	int addr_c_len = sizeof(addr_c);
	char buff[MAX_REQ], cmd[MAX_ARG];
	int i=0, pid;

	// Creating listening socket
	check(sfd_s = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);

	inet_aton(SERVER_ADDR, &(addr_s.sin_addr));
	check(bind(sfd_s, (struct sockaddr*)&addr_s, sizeof(addr_s)), "Error binding");
	check(listen(sfd_s, 5), "Error listening");
	// Accepting connection
	do {
		printf("Waiting...\n");
		check(sfd_c = accept(sfd_s, (struct sockaddr*)&addr_c, &addr_c_len), "Error accepting");
		check(pid = fork(), "Error forking");
	} while(pid != 0);

	// Main loop
	while(1) {

		// Receiving request
		check(recv(sfd_c, buff, MAX_REQ, 0), "Error recieving");
		printf("Received: \"%s\"\n%d bytes recieved from %s:%d.\n", buff, (int)strlen(buff), inet_ntoa(addr_c.sin_addr), ntohs(addr_c.sin_port));

		// Sending response (if necessary)
		sscanf(buff, "%s", cmd);
		if(!strcmp(cmd, "FIRE") || !strcmp(cmd, "GAMES")) { // If cmd needs an answer
			i=-1;
			do {
				printf("Reply n°%d: ", ++i);
				fgets(buff, MAX_REQ, stdin);
				*strchr(buff, '\n') = '\0';
				check(send(sfd_c, buff, strlen(buff)+1, 0), "Error sending");
			} while(strcmp(buff, "TURN") && strcmp(buff, "STOP"));
			printf("End of this response.\n");
		}
	}
	return EXIT_SUCCESS;
}
