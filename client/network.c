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
 * build_request()
 *
 * Build the string that will be sent to the server
 */

void build_request(req_t* req_p, char* buffer) {
	int argc, i;

	// Building request buffer up
	switch(req_p->type) { // Request type
		case PLACE_REQ:
			strcpy(buffer, "PLACE ");
			argc = 5;
			break;
		case FIRE_REQ:
			strcpy(buffer, "FIRE ");
			argc = 2;
			break;
		case HIT_REQ:
			strcpy(buffer, "HIT ");
			argc = 2;
			break;
		case MISS_REQ:
			strcpy(buffer, "MISS ");
			argc = 2;
			break;
		case SINK_REQ:
			strcpy(buffer, "SINK ");
			argc = 1;
			break;
		case TURN_REQ:
			strcpy(buffer, "TURN");
			argc = 0;
			break;
		case WIN_REQ:
			strcpy(buffer, "WIN");
			argc = 0;
			break;
		case LOSE_REQ:
			strcpy(buffer, "LOSE");
			argc = 0;
			break;
		default:
			printf("Invalid request code.\n");
			exit(EXIT_FAILURE);
	}
	
	for(i=0 ; i<argc ; ++i) { // Arguments
		strcat(buffer, req_p->args[i]);
		if(i!=argc-1) {
			strcat(buffer, " ");
		}
	}
}


/*
 * send_request()
 *
 * Sends a request. If response_p == 1, will wait for responses from the
 * server until a `TURN` request and will store them in buff_p.
 */
void send_request(req_t* req_p) {
	char buff[MAX_REQ];

	build_request(req_p, buff);
	check(send(req_p->sfd, buff, strlen(buff)+1, 0), "Error sending");
	printf("Sent: \"%s\"\n", buff);
}


/*
 * wait_request()
 *
 * Waits for a request on a socket.
 */
void wait_request(char* buffer, int sfd) {
	check(recv(sfd, buffer, MAX_REQ, 0), "Error receiveing");
	printf("Received: \"%s\"\n", buffer);
}
