/**
 * \file	serv.c
 * \brief	Contains functions for the host server.
 *
 * \author	Adrien Deprez
 * \date	12-09-14
 *
 */

#include "serv.h"

/*
 * A player's address is the address of his listening socket
 */

struct player player_tab[MAXPLAYER];

int main(int argc, char* argv[])
{
	// Variable declaration
	int serv_sock; 
	struct sockaddr_in serv_addr;
	int i=0;
	int port = 5000;
	pthread_t thread;

	// Construction of the Adress
	CHECK(serv_sock = socket(AF_INET, SOCK_STREAM,0), "Error : socket");
	// inet_aton("127.0.0.1", &(serv_addr.sin_addr));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	memset(&serv_addr.sin_zero,0,8);
	
	// Bind listening socket with Threadhis adress
	CHECK( (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))), "Error : bind" );
	
	// Listening
	CHECK(listen(serv_sock, 5), "Error : listen");

	while(1)
	{
		// Waiting for receiving
		player_tab[i].addr_len = sizeof(player_tab[i].addr_d); //!\\ Initialisation of each player
		CHECK(player_tab[i].sfd = accept(serv_sock,(struct sockaddr *)&(player_tab[i].addr_d), &(player_tab[i].addr_len)),"Error : accept");
		// Thread creation
		pthread_create(&thread, NULL, routine_thread, (void*)&(player_tab[i]));
		i++;
	}
}

void* routine_thread(void* arg)
{
	// Variable declaration
	struct player* arg_pl = (struct player*)arg;
	arg_pl->status = 0;
	char buff[MAXBUFF];
	int sts;
	char cmd[50];
	char addr[50];
	int port;
	int mode;
	char name[50];
	int i = 0;
	arg_pl->status = AVAILABLE;
	while(1)
	{
		// Receiving a request
		CHECK(sts = recv(arg_pl->sfd, buff, MAXBUFF, 0), "Error : read");
		sscanf(buff,"%s",cmd);
		printf("Reçu : %s \n",buff);
		// Compare the request and execute it
		if (!strcmp("HOST",cmd))
		{
			// Informations about the player
			sscanf(buff,"HOST %s %d %d",arg_pl->name,&port,&mode);
			arg_pl->addr_l = arg_pl->addr_d;
			arg_pl->addr_l.sin_port = htons(port);
			arg_pl->mode = mode;
			//** TEST **//
			printf("Name : %s \t Address : %s \t Port : %d \t Hote/Joueur : %d \n",arg_pl->name,inet_ntoa(arg_pl->addr_l.sin_addr),ntohs(arg_pl->addr_l.sin_port),arg_pl->mode);
			//** FIN TEST **//
			
		}
		else if (!strcmp("GAMES",cmd))
		{
			// Informations about the other players
			for (i=0;i<MAXPLAYER;i++)
			{
				if (player_tab[i].mode == 0 && strlen(player_tab[i].name) != 0 && player_tab[i].status != INGAME)
				{
					strcpy(addr,inet_ntoa(player_tab[i].addr_l.sin_addr));
					sprintf(buff,"GAME %s %s %d %d",player_tab[i].name,addr,ntohs(player_tab[i].addr_l.sin_port),player_tab[i].status);
					CHECK(send(arg_pl->sfd,buff,strlen(buff)+1,0),"Error : write");
					CHECK(sts = recv(arg_pl->sfd, buff, MAXBUFF, 0), "Error : read"); //!\\ Synchronisme 
					
				}
			}
			CHECK(send(arg_pl->sfd,"STOP",strlen("STOP")+1,0),"Error : write");
			
		}
		else if (!strcmp("JOIN",cmd))
		{
			sscanf(buff,"%s %s %s %d",cmd,name,addr,&port);
			for (i=0;i<MAXPLAYER;i++)
			{
				if (!strcmp(player_tab[i].name,name) && htons(port)==player_tab[i].addr_l.sin_port && !strcmp(inet_ntoa(player_tab[i].addr_l.sin_addr),addr))
				{
					player_tab[i].status = READY;
					player_tab[i].opponent = arg_pl;
					arg_pl->opponent = &player_tab[i];
				}
			}
			arg_pl->status = READY;
			//** TEST **//
			printf("%s Status : %d \t %s Status : %d \n",arg_pl->name,arg_pl->status,arg_pl->opponent->name,arg_pl->opponent->status);
			//** FIN TEST **//
			// Switch the player to "Ready" status		
		}
		else if (!strcmp("START",cmd))
		{
			// Switch the player to "IG" status		
			arg_pl->status = INGAME;
			arg_pl->opponent->status = INGAME;
			//** TEST **//
			printf("%s Status : %d \t %s Status : %d \n",arg_pl->name,arg_pl->status,arg_pl->opponent->name,arg_pl->opponent->status);
			//** FIN TEST **//
		}
	}	
	pthread_exit(NULL);
}
