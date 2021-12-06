#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#include "public.h"
#include "sql.h"
#include <sqlite3.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

#define PACK_SIZE 512
#define DATA_SIZE 510

//TCP包
typedef struct {
	char opt_code;
	char pack_num;
	char data[DATA_SIZE];
}pack_info;

//封装操作数
#define ERR 0
#define REG 1
#define LOG 2
#define QUI 3
#define ADD 4
#define DEL 5
#define MOD 6
#define SOR 7
#define OUT 8
#define ACK 9

int log_interactive(sqlite3*, int);
int user_interactive(sqlite3*, int);
int admin_interactive(sqlite3*, int);
int Regist_account(sqlite3*, int);
int Login_account(sqlite3*, int);
int Add_account(sqlite3*, user_info*);
int Delete_account(sqlite3*, user_info*);
int Modify_account(sqlite3*, user_info*);
int Search_account(sqlite3*, user_info*);
void circle(sqlite3*, int, char*);

#endif
