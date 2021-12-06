#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#define ID_SIZE 32
#define NAME_SIZE 32
#define ADDRESS_SIZE 128
#define PHONE_SIZE 20
#define PASSWD_SIZE 32

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

//用户信息
typedef struct {
	char id[ID_SIZE];
	char password[PASSWD_SIZE];
	int newfd;
}user_info;

//用于函数报错
#define ERR_MSG(msg) do{\
	fprintf(stderr, "__%d__", __LINE__);\
	perror(msg);\
}while(0)

#endif
