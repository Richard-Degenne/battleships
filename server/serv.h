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

// Macrofunction(s) definition
#define CHECK(sts,msg) if ((sts) == -1){ perror(msg); exit(-1);  }
#define MAXBUFF 1024
#define MAXPLAYER 404
#define MAXNAME 50
#define READY 1
#define AVAILABLE 0
#define INGAME 2

// Librarie(s) insertion
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>

// Struture(s) declaration

struct player{
	struct sockaddr_in addr_d; // Client dialog socket addr
	struct sockaddr_in addr_l; // Client listening socket addr
	int sfd; // Dialog socket between server and clients
	int addr_len;
	char name[MAXNAME];
	int status;
	int mode;
	struct player * opponent;
}player;

// Function(s) Prototype(s) declaration

void* routine_thread(void* );
#endif
