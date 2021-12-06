#ifndef __SER_H__
#define __SER_H__

#include <sqlite3.h>

#define PORT 8888
#define IP 	 "192.168.1.154"
#define MAX_CLI 32

typedef struct {
	int newfd;
	sqlite3* db;
}data;

int ser_create(sqlite3*);
void* callBack_cli(void*);

#endif
