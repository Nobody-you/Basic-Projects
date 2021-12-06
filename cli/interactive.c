#include "interactive.h"

int Logon_Interface(int sfd)
{
	int res, i;
	user_info info = {};
	pack_info ack = {};
	pack_info pack = {};

	pack.opt_code = REG;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code != ACK) {
		printf("Error: %s\n", pack.data);
		return -1;
	}

	printf("请输入工号> ");
	scanf("%s", info.id);
	while (getchar() != 10);
	printf("请输入密码> ");
	scanf("%s", info.password);
	while (getchar() != 10);

	for (i=1; i<=2; i++) {
		memset(&pack, 0, PACK_SIZE);
		pack.opt_code = REG;
		pack.pack_num = i;
		if (i == 1) {
			strcpy(pack.data, info.id);
		}
		else {
			strcpy(pack.data, info.password);
		}
		res = send(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		if (pack.opt_code == ERR) {
			printf("%s", pack.data);
			return 0;
		}
		else if (pack.pack_num != i) {
			printf("Wrong pack number\n");
			return -1;
		}
		res = recv(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ACK) {
		printf("注册成功！\n");
	}
	else if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		return 0;
	}
	return 0;
}

int Login_Interface(int sfd)
{
	int res, i;
	user_info info;
	pack_info ack;
	pack_info pack;

	pack.opt_code = LOG;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		return 0;
	}

	printf("请输入工号> ");
	scanf("%s", info.id);
	while (getchar() != 10);
	printf("请输入密码> ");
	scanf("%s", info.password);
	while (getchar() != 10);


	for (i=1; i<=2; i++) {
		pack.opt_code = LOG;
		pack.pack_num = i;
		if (i == 1) {
			strcpy(pack.data, info.id);
		}
		else {
			strcpy(pack.data, info.password);
		}
		res = send(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		res = recv(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		if (pack.opt_code == ERR) {
			printf("Error: %s\n", pack.data);
			return 0;
		}
	}
	send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ACK) {
		printf("登录成功！\n");
		ack.opt_code = ACK;
		res = send(sfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		if (strcmp(pack.data, "0") == 0)
			return 1;
		else
			return 2;
	}
	else if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		send(sfd, &ack, PACK_SIZE, 0);
		return 0;
	}
}

int Begin_interface(int sfd)
{
	int flag = 0;

	system("clear");
	puts("*************************");
    puts("**| 员 工 管 理 系 统 |**");
    puts("*************************");
    puts("*                       *");
    puts("*        1.注册         *");
    puts("*        2.登录         *");
    puts("*        3.退出         *");
    puts("*                       *");
    puts("*************************");
    puts("*----请输入你的选择-----*");
    char choice;
Login_again:
	choice = getchar();
	while (getchar() != 10);
	switch (choice) {
	case '1':
		flag = Logon_Interface(sfd);
		break;
	case '2':
		flag = Login_Interface(sfd);
		if (flag == 1) {
			while (1) {
				if (admin_interface(sfd) == -1)
					break;
			}
		}
		else if (flag == 2) {
			while (1) {
				if (staff_interface(sfd) == -1)
					break;
			}
		}
		break;
	case '3':
		puts("感谢使用，再见！\n");
		flag = -1;
		break;
	default:
		puts("输入有误！请重新输入: ");
		goto Login_again;
	}
	if (flag != -1) {
		printf("按下任意键继续> ");
		while (getchar() != 10);
	}
	return flag;
}


int admin_interface(int sfd)
{
	int flag = 0;
	char id[ID_SIZE] = "0";

	system("clear");
	puts("********************************");
    puts("**|  员  工  管  理  系  统  |**");
    puts("********************************");
    puts("*                              *");
    puts("*        1.添加员工信息        *");
    puts("*        2.删除员工信息        *");
    puts("*        3.修改员工信息        *");
    puts("*        4.查找员工信息        *");
    puts("*        5.   退  出           *");
    puts("*                              *");
    puts("********************************");
    puts("*--------请输入你的选择--------*");
    char choice;
Login_again:
	choice = getchar();
	while (getchar() != 10);
	switch (choice) {
	case '1':
		flag = add_staff(sfd, id);
		break;
	case '2':
		flag = delete_staff(sfd);
		break;
	case '3':
		flag = modify_staff(sfd, id);
		break;
	case '4':
		flag = search_staff(sfd, id);
		break;
	case '5':
	{
		int res;
		flag = -1;
		pack_info pack;
		pack.opt_code = OUT;
		res = send(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		else if (res == 0) {
			return -1;
		}
		res = recv(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		if (pack.opt_code == ERR) {
			printf("Error: %s\n", pack.data);
			if (strcmp(pack.data, "Permission denied!") == 0) {
				exit(0);
			}
			return 0;
		}

		break;
	}
	default:
		puts("输入有误！请重新输入: ");
		goto Login_again;
	}
	if (flag != -1) {
		printf("按下任意键继续> ");
		while (getchar() != 10);
	}
	return flag;
}


int staff_interface(int sfd)
{
	int res, flag = 0;
	char id[ID_SIZE] = "";
	pack_info pack;
	
	pack.opt_code = ACK;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	strcpy(id, pack.data);
	printf("id = %s\n", id);

	system("clear");
	puts("********************************");
    puts("**|  员  工  管  理  系  统  |**");
    puts("********************************");
    puts("*                              *");
    puts("*        1.添加员工信息        *");
    puts("*        2.修改员工信息        *");
    puts("*        3.查找员工信息        *");
    puts("*        4.   退  出           *");
    puts("*                              *");
    puts("********************************");
    puts("*--------请输入你的选择--------*");
    char choice;
Login_again:
	choice = getchar();
	while (getchar() != 10);
	switch (choice) {
	case '1':
		flag = add_staff(sfd, id);
		break;
	case '2':
		flag = modify_staff(sfd, id);
		break;
	case '3':
		flag = search_staff(sfd, id);
		break;
	case '4':
	{
		int res;
		flag = -1;
		pack_info pack;
		pack.opt_code = OUT;
		res = send(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		res = recv(sfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		if (pack.opt_code == ERR) {
			printf("Error: %s\n", pack.data);
			if (strcmp(pack.data, "Permission denied!") == 0) {
				exit(0);
			}
			return 0;
		}

		break;
	}
	default:
		puts("输入有误！请重新输入: ");
		goto Login_again;
	}
	if (flag != -1) {
		printf("按下任意键继续> ");
		while (getchar() != 10);
	}
	return flag;
}

int add_staff(int sfd, char* id)
{
	int res;
	pack_info pack, ack;
	staff_info sinfo;
	
	pack.opt_code = ADD;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}

	printf("请输入工号> ");
	scanf("%s", sinfo.id);
	while (getchar() != 10);
	printf("请输入姓名> ");
	scanf("%s", sinfo.name);
	while (getchar() != 10);
	printf("请输入性别> ");
	scanf("%c", &sinfo.sex);
	while (getchar() != 10);
	printf("请输入年龄> ");
	scanf("%d", &sinfo.age);
	while (getchar() != 10);
	printf("请输入地址> ");
	scanf("%s", sinfo.address);
	while (getchar() != 10);
	printf("请输入工资> ");
	scanf("%d", &sinfo.salary);
	while (getchar() != 10);
	printf("请输入电话> ");
	scanf("%s", sinfo.phone);
	while (getchar() != 10);
	if (strcmp(id, "0") == 0) {
		sinfo.status = 0;
	}
	else {
		sinfo.status = 1;
	}

	pack.opt_code = ADD;
	sprintf(pack.data, "%s %s %c %d %s %d %s %d", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.address, sinfo.salary, sinfo.phone, sinfo.status);
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	else if (pack.opt_code == ACK) {
		printf("%s\n", pack.data);
		return 0;
	}
}

int delete_staff(int sfd)
{
	int res;
	pack_info pack, ack;
	staff_info sinfo;

	pack.opt_code = DEL;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}	
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}

	printf("请输入工号> ");
	scanf("%s", pack.data);
	while (getchar() != 10);

	pack.opt_code = DEL;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	else if (pack.opt_code == ACK) {
		printf("%s\n", pack.data);
		return 0;
	}
	
	return 0;	
}

int modify_staff(int sfd, char* id)
{
	int res;
	pack_info pack, ack;
	staff_info sinfo;

	pack.opt_code = MOD;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}

	printf("请输入工号> ");
	scanf("%s", sinfo.id);
	while (getchar() != 10);
	printf("请输入姓名> ");
	scanf("%s", sinfo.name);
	while (getchar() != 10);
	printf("请输入性别> ");
	scanf("%c", &sinfo.sex);
	while (getchar() != 10);
	printf("请输入年龄> ");
	scanf("%d", &sinfo.age);
	while (getchar() != 10);
	printf("请输入地址> ");
	scanf("%s", sinfo.address);
	while (getchar() != 10);
	printf("请输入工资> ");
	scanf("%d", &sinfo.salary);
	while (getchar() != 10);
	printf("请输入电话> ");
	scanf("%s", sinfo.phone);
	while (getchar() != 10);
	if (strcmp(id, "0") == 0) {
		sinfo.status = 0;
	}

	pack.opt_code = MOD;
	sprintf(pack.data, "%s %s %c %d %s %d %s", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.address, sinfo.salary, sinfo.phone);
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	else if (pack.opt_code == ACK) {
		printf("%s\n", pack.data);
		return 0;
	}

	return 0;	
}

int search_staff(int sfd, char* id)
{
	int res;
	pack_info pack, ack;
	staff_info sinfo;

	pack.opt_code = SOR;
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	printf("请输入工号> ");
	scanf("%s", sinfo.id);
	while (getchar() != 10);

	pack.opt_code = SOR;
	strcpy(pack.data, sinfo.id);
	res = send(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(sfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	if (pack.opt_code == ERR) {
		printf("Error: %s\n", pack.data);
		if (strcmp(pack.data, "Permission denied!") == 0) {
			exit(0);
		}
		return 0;
	}
	
	if (strcmp(id, sinfo.id) == 0 || strcmp(id, "0") == 0) {
		sscanf(pack.data, "%s %s %c %d %s %d %s %d", sinfo.id, sinfo.name, &sinfo.sex, &sinfo.age, sinfo.address, &sinfo.salary, sinfo.phone, &sinfo.status);
		puts("****************************************************************");
		printf("id\tname\tsex\tage\taddress\tsalary\tphone\tstatus\n");
		puts("****************************************************************");
		printf("%s\t%s\t\%c\t%d\t%s\t%d\t%s\t%d\n", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.address, sinfo.salary, sinfo.phone, sinfo.status);
		puts("****************************************************************");
	}
	else {
		sscanf(pack.data, "%s %s %c %d %s", sinfo.id, sinfo.name, &sinfo.sex, &sinfo.age, sinfo.phone);
		puts("****************************************************************");
		printf("id\tname\tsex\tage\t\tphone\n");
		puts("****************************************************************");
		printf("%s\t%s\t\%c\t%d\t%s\n", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.phone);
		puts("****************************************************************");
	}
	return 0;	
}
