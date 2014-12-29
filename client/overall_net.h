/*
 * overall_net.h
 *
 * Richard Degenne - Adrien Deprez
 * 15/12/14
 *
 * Header for the overall_net.c file.
 */

# ifndef _OVERALL_NET_H
# define _OVERALL_NET_H 1


// Includes
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>

// Defines and macros
# define SERVER_ADDR "127.0.0.1"
# define SERVER_PORT 5001
# define PLACE_REQ 1
# define FIRE_REQ 2

# define MAX_ARG 30
# define MAX_REQ 200
# define MAX_NAME 50
# define MAX_GAMES 100
# define TIMEOUT 60

# define check(sts,msg) if((sts) == -1) { \
	perror(msg); exit(EXIT_FAILURE);}

// Structures
typedef struct game {
	char name[MAX_NAME];
	struct sockaddr_in addr;
} game_t;

typedef struct opponent {
	int sfd;
	struct sockaddr_in addr;
	int addr_len;
	char name[MAX_NAME];
} opponent_t;
// Prototypes
void sign_in(char*, int*, int*);
int get_games(int, game_t[]);
void print_games(game_t[], int);

# endif
