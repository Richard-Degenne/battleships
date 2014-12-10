/*
 * network.c
 *
 * Richard Degenne - Adrien Deprez
 * 09/12/14
 *
 * Header for the network.c file.
 */

# include "network.h"
/*
int main(int argc, char* argv[]) {
	req_t request;
	char out_buffer[MAX_REQ];
	int sfd;
	struct sockaddr_in addr_s;
	request.type = PLACE_REQ;
	strcpy(request.args[0], "4");
	strcpy(request.args[1], "2");
	strcpy(request.args[2], "5");
	strcpy(request.args[3], "2");
	strcpy(request.args[4], "9");
	init_connection(&sfd, &addr_s);

	build_request(&request, out_buffer);

	check(send(sfd, out_buffer, strlen(out_buffer)-1, 0), "Error sending");
	return EXIT_SUCCESS;
}
*/

/*
 * build_request()
 *
 * Build the string that will be sent to the server
 */

void build_request(req_t* req_p, char buffer[MAX_REQ]) {
	char buff[MAX_REQ];
	int cursor = 0;
	int argc, i;

	// Building request buffer up
	switch(req_p->type) { // Request type
		case PLACE_REQ:
			strcpy(buff, "PLACE ");
			cursor = 6;
			argc = 5;
			break;
		case FIRE_REQ:
			strcpy(buff, "FIRE ");
			cursor = 5;
			argc = 2;
			break;
		default:
			printf("Invalid request code.\n");
			exit(EXIT_FAILURE);
	}
	
	for(i=0 ; i<argc ; ++i) { // Arguments
		strcpy(buff + cursor*sizeof(char),strcat(req_p->args[i],(i == argc-1)?"":" "));
		cursor += strlen(req_p->args[i]);
	}
	strcpy(buffer, buff);
}


/*
 * init_connection()
 *
 * Opens a socket to the host server.
 */
void init_connection(int* sfd_p, struct sockaddr_in* addr_p) {
	
	check(*sfd_p = socket(AF_INET, SOCK_STREAM, 0), "Error creating socket");

	addr_p->sin_family = AF_INET;
	addr_p->sin_port = htons(SERVER_PORT);
	inet_aton(SERVER_ADDR, &(addr_p->sin_addr));
	
	check(connect(*sfd_p, (struct sockaddr*)addr_p, sizeof(*addr_p)), "Error connecting");
}

/*
 * send_request()
 *
 * Sends a request.
 */
void send_request(req_t* req_p) {
	char buff[MAX_REQ];
	int sfd;
	struct sockaddr_in addr_s;

	build_request(req_p, buff);
	printf("Request built: \"%s\"\n", buff);
	init_connection(&sfd, &addr_s);
	check(send(sfd, buff, strlen(buff), 0), "Error sending");
	printf("PLACE request sent.\n%d bytes sent.\n", (int) strlen(buff));
	close(sfd);
}
