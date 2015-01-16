#include "client.h"

void flush();

int main(int argc, char* argv[])
{
	char buff[MAXBUFF], addr[16], mess[MAXBUFF];
	int socket_dialog = 0;
	int port =5000;
	int servlen = 0;	
	char rep;
	struct sockaddr_in serv;
	
	CHECK(socket_dialog = socket(AF_INET, SOCK_STREAM,0), "Error : socket");
	CHECK(serv.sin_addr.s_addr = INADDR_ANY, "sin_addr");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	memset(&serv.sin_zero,0,8);
	servlen = sizeof(serv);
	
	CHECK(connect(socket_dialog,(struct sockaddr *)&serv, servlen),"Error : connect");
	
	while(1)
	{
		printf("Saisir une requête : ");
		fgets(buff, MAXBUFF,stdin);
		*strchr(buff,'\n') = '\0';
		flush();
		CHECK(send(socket_dialog,buff,strlen(buff)+1,0),"Error : send");
		printf("Attendre une réponse ? y/n \t");
		scanf("%c",&rep);
		flush();
		if (rep == 'y')
		{
			CHECK(recv(socket_dialog,buff,MAXBUFF,0),"Error : recv");
			printf("Reçu : %s \n",buff);
		}
		
	}

	close(socket_dialog);
	
	return 0;
}

void flush ()
{
	char c;
	do c = getchar();
	while (c!='\n' && c!=EOF);
}
