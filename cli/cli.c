#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include "cli.h"

int cli_create()
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		ERR_MSG("sockid");
		return -1;
	} 

	int reuse = 1;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		ERR_MSG("setsockopt");
		return -1;
	}

	struct sockaddr_in sin;
	sin.sin_family  	= AF_INET;
	sin.sin_port 		= htons(PORT);
	sin.sin_addr.s_addr = inet_addr(IP);

	if (connect(sfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		ERR_MSG("connect");
		return -1;
	}

	return sfd;
}
