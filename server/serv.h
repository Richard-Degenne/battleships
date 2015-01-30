/**
 * \file	serv.h
 * \brief	Header for the serv.c file.
 *
 * \author	Adrien Deprez
 * \date	12-09-14
 *
 */


#ifndef _SERV_H
#define _SERV_H 1

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>


/**
 * \brief	Makes sure a system call don't fail
 */
#define CHECK(sts,msg) if ((sts) == -1){ perror(msg); exit(-1);  }

/**
 *	\brief	Maximum lenght of a request
 */
#define MAXBUFF 1024

/**
 * Maximum number of connected players.
 */
#define MAXPLAYER 404

/**
 * \brief	Manimum lenght for a name
 */
#define MAXNAME 50

/**
 * \brief	The game is ready.
 * \details	A game is ready if it has been joined by someone by has not started yet.
 */
#define READY 1

/**
 * \brief	The game is available.
 * \details	A game is available if is has not been joined yet.
 *
 */
#define AVAILABLE 0

/**
 * \brief	The game is in progress.
 */
#define INGAME 2

// Strutures

/**
 * \brief	Structure describing a player
 */
struct player{
	struct sockaddr_in addr_d; //!< Client dialog socket address
	struct sockaddr_in addr_l; //!< Client listening socket address
	int sfd; //!<Dialog socket with the client
	int addr_len; //!< Lenght of the client's address
	char name[MAXNAME]; //!< Player's nickname
	int status; //!< Player's status
	int mode; //!< Player's mode
	struct player * opponent; //!< Player's opponent
}player;

// Prototypes

/**
 * \brief	Each client has its own thread.
 * \param	arg	Dummy argument, so the function signature matches a thread routine.
 * \return	Nothing, but has type void* for the same reason.
 */
void* routine_thread(void* );

#endif
