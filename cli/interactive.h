#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

#include "public.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

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

#define ADMIN 1
#define USER 0

int Begin_interface(int);
int admin_interface(int);
int staff_interface(int);
int Logon_Interface(int);
int Login_Interface(int);
int add_staff(int, char*);
int delete_staff(int);
int modify_staff(int, char*);
int search_staff(int, char*);

#endif
