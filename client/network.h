/**
 * \file	network.h
 * \brief	Header for the network.c file.
 *
 * \author	Richard Degenne - Adrien Deprez
 * \date	12-09-14
 *
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
/**
 * \brief	Port to the server listening socket
 */
# define SERVER_PORT 5000

/**
 * \brief	Code for a `PLACE` request.
 */
# define PLACE_REQ 1

/**
 * \brief	Code for a `PLACE` request.
 */
# define FIRE_REQ 2

/**
 * \brief	Code for a `HIT` request.
 */
# define HIT_REQ 3

/**
 * \brief	Code for a `MISS` request.
 */
# define MISS_REQ 4

/**
 * \brief	Code for a `SINK` request.
 */
# define SINK_REQ 5

/**
 * \brief	Code for a `TURN` request.
 */
# define TURN_REQ 6

/**
 * \brief	Code for a `WIN` request.
 */
# define WIN_REQ 7

/**
 * \brief	Code for a `LOSE` request.
 */
# define LOSE_REQ 8

/**
 * \brief	Maximum lenght for a request argument.
 */
# define MAX_ARG 30

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
 * \brief	Structure containing data to describe a request
 * \details	To send a request, one has to initalize a req_t variable with the appropriate values.
 */
typedef struct req_t {
	int sfd; //!< Socket file descriptor

	/**
	 * \brief	Type of the request.
	 * \details	Must be one of the following: `PLACE_REQ`, `FIRE_REQ`, `HIT_REQ`, `MISS_REQ`, `SINK_REQ`, `WIN_REQ`, `LOSE_REQ`.
	 */
	int type;
	char args[5][MAX_ARG];//!< The request arguments
} req_t;

// Prototypes

/**
 * \brief	Builds a request.
 * \details	Uses the `req_p` values to compute a string that will be copied to `buffer`.
 * \param	req_p	The request to send
 * \param	buffer	The string to store the generated request.
 */
void build_request(req_t*, char*);

/**
 * \brief	Sends a request.
 * \details	This function builds the actual request and sends it through the request's socket.
 * \param	req_p	The request to send
 * \see	build_request
 */
void send_request(req_t*);

/**
 * \brief	Waits for a request to be sent on a socket.
 * \details	A call to this function will be blocking until a new content arrives on the socket.
 * \param	buffer	A string where the received request will be copied
 * \param	sfd	File descriptor to the socket to listen to.
 */
void wait_request(char*, int);

# endif
