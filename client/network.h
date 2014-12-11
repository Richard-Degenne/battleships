/*
 * network.h
 *
 * Richard Degenne - Adrien Deprez
 * 09/12/14
 *
 * Header for the network.c file.
 */

# ifndef _NETWORK_H
# define _NETWORK_H 1


// Includes
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/in.h>
# include <poll.h>

// Defines and macros
# define SERVER_ADDR "127.0.0.1"
# define SERVER_PORT 5000
# define PLACE_REQ 1
# define FIRE_REQ 2

# define MAX_ARG 30
# define MAX_REQ 200
# define TIMEOUT 60

# define check(sts,msg) if((sts) == -1) { \
	perror(msg); exit(EXIT_FAILURE);}

// Structures
typedef struct req_t {
	int type; // PLACE_REQ or FIRE_REQ
	char args[5][MAX_ARG];
} req_t;

// Prototypes
void build_request(req_t*, char[MAX_REQ]);
void init_connection(int*, struct sockaddr_in*);
void send_request(req_t*, char[MAX_REQ]);
void parse_request(req_t*);

# endif
