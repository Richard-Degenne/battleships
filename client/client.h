#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHECK(sts,msg) if ((sts) == -1){ perror(msg); exit(-1);  }

#define MAXBUFF 1024

