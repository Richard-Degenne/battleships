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
	int addr_c_len;
	char buff[MAX_REQ];

	check(sfd_s = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");
	addr_s.sin_family = AF_INET;
	addr_s.sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_ADDR, &(addr_s.sin_addr));

	check(bind(sfd_s, (struct sockaddr*)&addr_s, sizeof(addr_s)), "Error binding");

	check(listen(sfd_s, 5), "Error listening");

	while(1) {
		check(sfd_c = accept(sfd_s, (struct sockaddr*)&addr_c, &addr_c_len), "Error accepting");
		check(recv(sfd_c, buff, MAX_REQ, 0), "Error recieving");
		printf("Received: \"%s\"\n%d bytes recieved.\n", buff, (int)strlen(buff)-1);
		close(sfd_c);
	}
	return EXIT_SUCCESS;
}
