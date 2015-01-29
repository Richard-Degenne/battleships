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
	int addr_len;
	char name[MAX_NAME];//!< The opponent's name.
} opponent_t;

// Prototypes

/**
 * \brief	Sets everything up to connect to the game.
 * \details	Creates a listening socket, connects to the server and sends the address of the newly-created listening socket to it.
 * \param	nick	Player's nickname
 * \param	addr_str	The server IP address, as a "A.B.C.D" string.
 * \param	sfd_server	The dialog socket that will be established with the server
 * \param	This player's listening socket
 * \param	mode	0: Host / 1: Join
 */
void sign_in(char*, char*, int*, int*, int);

/**
 * \brief	Fetches the games from the server.
 * \details	Sends to the server a `GAMES` request and waits for `GAME` answers until `STOP`.
 * \param	sfd	The server dialog socket file descriptor
 * \param	tab_p	Array of games to fill
 * \return	The number of received games.
 */
int get_games(int, game_t[]);

/**
 * \brief	Prints games in a pretty way.
 * \details	Display each game's infos in a table.
 * \param	tab_p	Array of games to display
 * \param	n	Number of games to display
 */
void print_games(game_t[], int);

/**
 * \brief	Connects to the opponent.
 * \details	Creates a dialog socket to the selected opponent and send a `JOIN` request to the server to let it know the player has joined a game.
 * \param	game_p	The game to connect to
 * \param	o	A pointer to the opponent to fill
 * \param	sfd_s	The server dialog socket file descriptor.
 * \return	A file descriptor to the newly-created socket.
 */
void connect_player(game_t, opponent_t*, int);

/**
 * \brief	Waits for an opponent to connect.
 * \details	Waits for a connection the listening socket.
 * \return	A file descriptor to the newly-created socket.
 */
void accept_player(int, opponent_t*);

/**
 * \brief	Sends the player's name to the host.
 * \details	Sends a `NAME` request.
 * \param	sfd_p	The host socket file descriptor
 * \param	name_p	The name to send
 */
void send_name(int, char*);

/**
 * \brief	Waits for the joining player to send their name.
 * \details	Waits to receive a `NAME` request.
 * \param	opponent_p	Opponent whose name is expected to be received.
 */
void wait_name(opponent_t*);

/**
 * \brief	Signals that the game has started.
 * \details	Sends a `START` request to both the joining player and the server.
 * \param	sfd_s	The server dialog socket file descriptor
 * \param	o	The opponent to notify
 */
void send_start(int, opponent_t);

/**
 * \brief	Waits for the host to start the game.
 * \details	Waits to receive a `START` request.
 * \param	sfd_o	The socket on which to listen for the request
 */
void wait_start(int);

# endif
