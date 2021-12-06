#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#define ID_SIZE 32
#define NAME_SIZE 32
#define ADDRESS_SIZE 128
#define PHONE_SIZE 20
#define PASSWD_SIZE 32

#define PACK_SIZE 512
#define DATA_SIZE 510

//员工信息
typedef struct {
	char id[ID_SIZE];
	char name[NAME_SIZE];
	char sex;
	int age;
	char address[ADDRESS_SIZE];
	int salary;
	char phone[PHONE_SIZE];
	int status;
}staff_info;

//登录的用户信息
typedef struct {
	char id[ID_SIZE];
	char password[PASSWD_SIZE];
	int newfd;
}user_info;

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

//用于函数报错
#define ERR_MSG(msg) do{\
	fprintf(stderr, "__%d__", __LINE__);\
	perror(msg);\
}while(0)

#endif
