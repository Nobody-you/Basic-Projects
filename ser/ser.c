#include "ser.h"
#include "public.h"
#include "interactive.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

int ser_create(sqlite3* db)
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0) {
		ERR_MSG("socket");
		return -1;
	}

	int reuse = 1;
	if ( setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0) {
		ERR_MSG("setsockopt");
		return -1;
	}

	struct sockaddr_in sin;
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(IP);

	if ( bind(sfd, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		ERR_MSG("bind");
		return -1;
	}

	if (listen(sfd, MAX_CLI) < 0) {
		ERR_MSG("listen");
		return -1;
	}
	printf("正在监听...\n");
	
	struct sockaddr_in cin;
	socklen_t socklen = sizeof(cin);
	pthread_t tid;

	while (1) {
		int newfd = accept(sfd, (struct sockaddr*)&cin, &socklen);
		if (newfd < 0) {
			ERR_MSG("accept");
			return -1;
		}
		printf("[%s:%d] newfd = %d 链接成功\n", inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), newfd);
		data info = {newfd, db};
		if ( pthread_create(&tid, NULL, callBack_cli, (void*)&info) != 0) {
			ERR_MSG("pthread_create");
			return -1;
		}
	}
	close(sfd);

	return 0;
}

void* callBack_cli(void* arg)
{
	int ret;
	pthread_detach(pthread_self());
	data info = *(data*)arg;
	while(1) {
		ret = log_interactive(info.db, info.newfd);
		if (ret == -1)
			break;
		else if (ret == 1) {
			while (1) {
				if (admin_interactive(info.db, info.newfd) == -1)
					break;
			}
		}
		else if (ret == 2) {
			while (1) {
				if (user_interactive(info.db, info.newfd) == -1)
					break;
			}
		}
	}
	close(info.newfd);
	pthread_exit(NULL);
}
