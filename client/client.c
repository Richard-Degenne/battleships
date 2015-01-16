#include "client.h"

void flush();

int main(int argc, char* argv[])
{
	char buff[MAXBUFF], addr[16], mess[MAXBUFF];
	int socket_dialog = 0;
	int port =5000;
	socklen_t servlen = sizeof(struct sockaddr);	
	char rep='y';
	struct sockaddr_in serv;
	
	CHECK(socket_dialog = socket(AF_INET, SOCK_STREAM,0), "Error : socket");
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton("127.0.0.1", &(serv.sin_addr));
	memset(&serv.sin_zero,0,8);

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
			CHECK(send(socket_dialog,"OK",strlen("OK")+1,0),"Error : send");
			while(strcmp(buff,"STOP")!=0)
			{
				CHECK(recv(socket_dialog,buff,MAXBUFF,0),"Error : recv");
				printf("Reçu : %s \n",buff);
				CHECK(send(socket_dialog,"OK",strlen("OK")+1,0),"Error : send");
			}
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
