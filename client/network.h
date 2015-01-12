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

// Defines and macros
# define SERVER_PORT 5000
# define PLACE_REQ 1
# define FIRE_REQ 2
# define HIT_REQ 3
# define MISS_REQ 4
# define SINK_REQ 5
# define TURN_REQ 6
# define WIN_REQ 7
# define LOSE_REQ 8

# define MAX_ARG 30
# define MAX_REQ 200
# define MAX_RES 3
# define TIMEOUT 60

# define check(sts,msg) if((sts) == -1) { \
	perror(msg); exit(EXIT_FAILURE);}

// Structures
typedef struct req_t {
	int sfd;
	int type;
	char args[5][MAX_ARG];
} req_t;

// Prototypes
void build_request(req_t*, char*);
void send_request(req_t*);
void wait_request(char*, int);
# endif
