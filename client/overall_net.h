/**
 * \file	overall_net.h
 * \brief	Header for the overall_net.c file.
 *
 * \author	Richard Degenne
 * \date	12-15-14
 *
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

// Defines and macros

/**
 * \brief	Port to the server listening socket
 */
# define SERVER_PORT 5000

/**
 * \brief	The player wants to join an existing game.
 */
# define MODE_JOIN 1

/**
 * \brief	The player wants to host a game.
 */
# define MODE_HOST 0

/**
 * \brief	The game is available.
 * \details	A game is considered available as soon as it has not been joined by another player.
 */
# define AVAILABLE_ST 0


/**
 * \brief	Maximum lenght for a request argument.
 */
# define MAX_ARG 30

/**
 * \brief	Maximum number of games.
 */
# define MAX_GAMES 100

/**
 * \brief	Maximum nickname lenght
 */
# define MAX_NAME 30

/**
 * \brief	Maximum lenght for a request.
 */
# define MAX_REQ 200

/**
 * \brief	Maximum number of replies for a request.
 * \deprecated
 */
# define MAX_RES 3

/**
 * \brief	Timeout for an answer (in seconds)
 */
# define TIMEOUT 60

/**
 * \brief	Checks that a system call does not fail
 */
# define check(sts,msg) if((sts) == -1) { \
	perror(msg); exit(EXIT_FAILURE);}

// Structures

/**
 * \brief	Structure describing a game entry.
 * \details	A game is an entry in the game list, seen when you want to join on from the "Join" menu.
 */
typedef struct game {
	char name[MAX_NAME];//!< Name of the opponent
	struct sockaddr_in addr;//!< The host's listening socket IP address
	int status;//!< The game current status
} game_t;

/**
 * \brief	Structure describing an opponent.
 * \details	An opponent variable describes the opponent and the way to get through to them.
 */
typedef struct opponent {
	int sfd;//!< The opponent's dialog socket file descriptor
	struct sockaddr_in addr;//!< The opponent's dialog socket IP address
	int addr_len;//!< The opponent's dialog socket address lenght
	char name[MAX_NAME];//!< The opponent's name.
} opponent_t;

// Prototypes

/**
 * \brief	Sets everything up to connect to the game.
 * \details	Creates a listening socket, connects to the server and sends the address of the newly-created listening socket to it.
 * \param	nick	Player's nickname
 * \param	addr_str	The server IP address, as a "A.B.C.D" string.
 * \param	sfd_server	The dialog socket that will be established with the server
 * \param	sfd_listen	This player's listening socket
 * \param	mode	0: Host / 1: Join
 */
void sign_in(char* nick, char* addr_str, int* sfd_server, int* sfd_listen, int mode);

/**
 * \brief	Fetches the games from the server.
 * \details	Sends to the server a `GAMES` request and waits for `GAME` answers until `STOP`.
 * \param	sfd	The server dialog socket file descriptor
 * \param	tab_p	Array of games to fill
 * \return	The number of received games.
 */
int get_games(int sfd, game_t tab_p[]);

/**
 * \brief	Prints games in a pretty way.
 * \details	Display each game's infos in a table.
 * \param	tab_p	Array of games to display
 * \param	n	Number of games to display
 */
void print_games(game_t tab_p[], int n);

/**
 * \brief	Connects to the opponent.
 * \details	Creates a dialog socket to the selected opponent and send a `JOIN` request to the server to let it know the player has joined a game.
 * \param	game_p	The game to connect to
 * \param	o	A pointer to the opponent to fill
 * \param	sfd_s	The server dialog socket file descriptor.
 * \return	A file descriptor to the newly-created socket.
 */
void connect_player(game_t game_p, opponent_t* o, int sfd_s);

/**
 * \brief	Waits for an opponent to connect.
 * \details	Waits for a connection the listening socket.
 * \param	sfd_l	The socket to listen to.
 * \param	opponent_p	A pointer to the opponent to fill
 * \return	A file descriptor to the newly-created socket.
 */
void accept_player(int sfd_l, opponent_t* opponent_p);

/**
 * \brief	Sends the player's name to the host.
 * \details	Sends a `NAME` request.
 * \param	sfd_p	The host socket file descriptor
 * \param	name_p	The name to send
 */
void send_name(int sfd_p, char* name_p);

/**
 * \brief	Waits for the joining player to send their name.
 * \details	Waits to receive a `NAME` request.
 * \param	opponent_p	Opponent whose name is expected to be received.
 */
void wait_name(opponent_t* opponent_p);

/**
 * \brief	Signals that the game has started.
 * \details	Sends a `START` request to both the joining player and the server.
 * \param	sfd_s	The server dialog socket file descriptor
 * \param	o	The opponent to notify
 */
void send_start(int sfd_s, opponent_t o);

/**
 * \brief	Waits for the host to start the game.
 * \details	Waits to receive a `START` request.
 * \param	sfd_o	The socket on which to listen for the request
 */
void wait_start(int sfd_o);

# endif
