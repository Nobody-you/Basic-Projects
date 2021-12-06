#include "interactive.h"

void circle(sqlite3* db, int newfd, char* id)
{
	close(newfd);
	sql_reset_status(db, id);
}

int log_interactive(sqlite3* db, int newfd)
{
	int res, flag;
	pack_info pack;

	res = recv(newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		pthread_exit(NULL);
	}
	else if (res == 0) {
		close(newfd);
		return -1;
	}
	switch (pack.opt_code) {
	case REG:
		flag = Regist_account(db, newfd);
		break;
	case LOG:
		flag = Login_account(db, newfd);
		break;
	case QUI:
		flag = -1;
		break;
	}

	return flag;
}

int user_interactive(sqlite3* db, int newfd)
{
	int res, flag = 0;
	pack_info pack, ack;
	user_info user;

	res = recv(newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		pthread_exit(NULL);
	}
	else if (res == 0) {
		circle(db, user.newfd, user.id);
		return -1;
	}
	sql_get_id_by_newfd(db, newfd, ack.data);
	user.newfd = newfd;
	ack.opt_code = ACK;
	strcpy(user.id, ack.data);
	res = send(newfd, &ack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	res = recv(newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		pthread_exit(NULL);
	}
	else if (res == 0) {
		circle(db, user.newfd, user.id);
		return -1;
	}
	ack.opt_code = ACK;
	res = send(newfd, &ack, PACK_SIZE, 0);
	if (res < 0) {
		pthread_exit(NULL);
	}

	switch (pack.opt_code) {
	case ADD:
		Add_account(db, &user);
		break;
	case MOD:
		Modify_account(db, &user);
		break;
	case SOR:
		Search_account(db, &user);
		break;
	case OUT:
		flag = -1;
		sql_reset_status(db, user.id);
		break;
	}

	return flag;
}

int admin_interactive(sqlite3* db, int newfd)
{
	int res, flag = 0;
	pack_info pack, ack;
	user_info admin;
	strcpy(admin.id, "0");
	admin.newfd = newfd;
	
	res = recv(newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		pthread_exit(NULL);
	}
	else if (res == 0) {
		circle(db, newfd, admin.id);
		return -1;
	}
	res = send(newfd, &ack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}


	switch (pack.opt_code) {
	case ADD:
		Add_account(db, &admin);
		break;
	case DEL:
		Delete_account(db, &admin);
		break;
	case MOD:
		Modify_account(db, &admin);
		break;
	case SOR:
		Search_account(db, &admin);
		break;
	case OUT:
		flag = -1;
		sql_reset_status(db, admin.id);
		break;
	}
	return flag;
}

int Regist_account(sqlite3* db, int newfd)
{
	int res, i;
	user_info info;
	pack_info pack, ack;

	ack.opt_code = ACK;
	send(newfd, &ack, PACK_SIZE, 0);

	for (i=1; i<=2; i++) {
		res = recv(newfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		else if (res == 0) {
			close(newfd);
			return -1;
		}
		if (pack.opt_code != REG) {
			ack.opt_code = ERR;
			sprintf(ack.data, "Wrong pack opration code");
			send(newfd, &ack, PACK_SIZE, 0);
			return 0;
		}
		if (pack.pack_num != i) {
			ack.opt_code = ERR;
			sprintf(ack.data, "Wrong pack number");
			send(newfd, &ack, PACK_SIZE, 0);
			return 0;
		}
		if (i == 1) {
			strcpy(info.id, pack.data);
		}
		else {
			strcpy(info.password, pack.data);
		}
		ack.opt_code = ACK;
		pack.pack_num = i;
		res = send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}

	info.newfd = 0;
	res = sql_insert_account(db, &info);
	if (res == -1) {
		ack.opt_code = ERR;
		strcpy(ack.data, sqlite3_errmsg(db));
		send(newfd, &ack, PACK_SIZE, 0);
		return 0;
	}
	else {
		ack.opt_code = ACK;
		res = send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;
}

int Login_account(sqlite3*db, int newfd)
{
	int res, i;
	user_info info;
	pack_info pack, ack;

	ack.opt_code = ACK;
	res = send(newfd, &ack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("send");
		return -1;
	}
	//读入id和password
	for (i=1; i<=2; i++) {
		res = recv(newfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		else if (res == 0) {
			close(newfd);
			return -1;
		}
		if (pack.opt_code != LOG) {
			ack.opt_code = ERR;
			sprintf(ack.data, "Wrong pack code");
			res = send(newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
		if (pack.pack_num != i) {
			ack.opt_code = ERR;
			sprintf(ack.data, "Wrong pack number");
			res = send(newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
		if (i == 1) {
			strcpy(info.id, pack.data);
		}
		else {
			strcpy(info.password, pack.data);
		}
		ack.opt_code = ACK;
		ack.pack_num = i;
		res = send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}
	recv(newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	else if (res == 0) {
		close(newfd);
		return -1;
	}

	//检查账号是否存在
	res = sql_check_account(db, &info);
	switch (res) {
	case -1:
		ack.opt_code = ERR;
		sprintf(ack.data, "Unknow username");

		res = send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		res = recv(newfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		else if (res == 0) {
			close(newfd);
			return -1;
		}
		return 0;
	case 0:
		ack.opt_code = ERR;
		sprintf(ack.data, "Wrong password");
		res = send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		res = recv(newfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		else if (res == 0) {
			close(newfd);
			return -1;
		}
		return 0;
	case 1:
	{
		//检查账号是否已经登录
		int status;
		sql_get_account_status(db, info.id, &status);
		if (status > 0) {
			ack.opt_code = ERR;
			sprintf(ack.data, "Account already login");
			send(newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
			else if (res == 0) {
				close(newfd);
				return -1;
			}
			return 0;
		}
		//发送id
		memset(&ack, 0, PACK_SIZE);
		ack.opt_code = ACK;
		strcpy(ack.data, info.id);
		send(newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		recv(newfd, &pack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("recv");
			return -1;
		}
		else if (res == 0) {
			close(newfd);
			return -1;
		}
		//设置账户状态
		sql_set_account_status(db, newfd, info.id);
		if (strcmp(info.id, "0") == 0)
			return 1;
		else
			return 2;
	}
	default:
		return 0;
	}
}

int Add_account(sqlite3* db, user_info* user)
{
	int res;
	pack_info ack;
	pack_info pack;
	staff_info sinfo = {};

	//接收员工信息
	res = recv(user->newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	else if (res == 0) {
		circle(db, user->newfd, user->id);
		return -1;
	}
	if (pack.opt_code != ADD) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Wrong pack code");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return -1;
	}
	res = sscanf(pack.data, "%s %s %c %d %s %d %s %d", sinfo.id, sinfo.name, &sinfo.sex, &sinfo.age, sinfo.address, &sinfo.salary, sinfo.phone, &sinfo.status);
	if (res != 8) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Illigal parameter");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return 0;

	}
	//检查
	//账号存在
	if (sql_check_existance(db, sinfo.id) == 0) {
		//账号未绑定
		if (sql_check_staff_status(db, sinfo.id) == 0) {
			//当前用户是普通用户
			if (strcmp(user->id, "0") != 0) {
				sinfo.salary = 3000;
				sql_modify(db, &sinfo);
				ack.opt_code = ACK;
				sprintf(ack.data, "Staff binds successfully");
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
			else {
				ack.opt_code = ERR;
				sprintf(ack.data, "Staff already exists");
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
		}
		else {
			//当前用户是普通用户
			if (strcmp(user->id, "0") != 0) {
				ack.opt_code = ERR;
				sprintf(ack.data, "Staff has been bound");
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
			else {
				ack.opt_code = ERR;
				sprintf(ack.data, "Staff already exists");
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
		}
	}
	else {
		//注册账号和当前账号是否匹配
		if (strcmp(user->id, sinfo.id) != 0) {
			sinfo.status = 0;
			sql_insert_staff(db, &sinfo);
			ack.opt_code = ACK;
			sprintf(ack.data, "Staff creates successfully");
			res = send(user->newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
		else {
			sql_insert_staff(db, &sinfo);
			ack.opt_code = ACK;
			sprintf(ack.data, "Staff binds successfully");
			res = send(user->newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
	}
	return 0;
}

int Delete_account(sqlite3* db, user_info* user)
{
	int res;
	pack_info ack;
	pack_info pack;
	staff_info sinfo = {};
	user_info uinfo = {};

	//接收员工信息
	res = recv(user->newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	else if (res == 0) {
		circle(db, user->newfd, user->id);
		return -1;
	}
	if (pack.opt_code != DEL) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Wrong pack code");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return 0;
	}
	strcpy(sinfo.id, pack.data);

	//检查
	//账号存在
	strcpy(uinfo.id, sinfo.id);
	if (sql_check_account(db, &uinfo) >= 0) {
		int target_newfd;
		//检查用户是否存在
		if (sql_check_existance(db, sinfo.id) == 0) {
			sql_delete_staff(db, sinfo.id);
		}
		ack.opt_code = ERR;
		sprintf(ack.data, "Permission denied!");
		target_newfd = sql_check_account_status(db, sinfo.id);
		if (target_newfd != 0) {
			res = send(target_newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
			close(target_newfd);
		}
		sql_delete_account(db, sinfo.id);
		ack.opt_code = ACK;
		bzero(ack.data, DATA_SIZE);
		sprintf(ack.data, "Staff & Account have been deleted");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}
	else {
		if (sql_check_existance(db, sinfo.id) == 0) {
			sql_delete_staff(db, sinfo.id);
			ack.opt_code = ACK;
			bzero(ack.data, DATA_SIZE);
			sprintf(ack.data, "Staff has been deleted");
			res = send(user->newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
		else {
			ack.opt_code = ERR;
			sprintf(ack.data, "Staff or account do not exists");
			res = send(user->newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
	}
	return 0;
}

int Modify_account(sqlite3* db, user_info* user)
{
	int res;
	pack_info ack;
	pack_info pack;
	staff_info sinfo = {}, sinfo_old;

	//接收员工信息
	res = recv(user->newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	else if (res == 0) {
		circle(db, user->newfd, user->id);
		return -1;
	}
	if (pack.opt_code != MOD) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Wrong pack code");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return -1;
	}
	res = sscanf(pack.data, "%s %s %c %d %s %d %s", sinfo.id, sinfo.name, &sinfo.sex, &sinfo.age, sinfo.address, &sinfo.salary, sinfo.phone);
	if (res != 7) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Illigal parameter");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return 0;
	}

	//检查
	//账号存在
	if (sql_search(db, sinfo.id, &sinfo_old) == 0) {
		//当前用户是普通用户
		if (strcmp(user->id, "0") != 0) {
			//修改的不是自己的信息
			if (strcmp(user->id, sinfo.id) != 0) {
				ack.opt_code = ERR;
				sprintf(ack.data, "Permission denied");
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
			sinfo.salary = sinfo_old.salary;
			sinfo.status = sinfo_old.status;
			sql_modify(db, &sinfo);
		}
		else {
			sql_modify(db, &sinfo);
		}
		ack.opt_code = ACK;
		sprintf(ack.data, "Staff infomation modified successfully");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}
	else {
		ack.opt_code = ERR;
		sprintf(ack.data, "Staff does not exists");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}
	return 0;
}

int Search_account(sqlite3* db, user_info* user)
{
	int res;
	pack_info ack;
	pack_info pack;
	staff_info sinfo = {};

	res = recv(user->newfd, &pack, PACK_SIZE, 0);
	if (res < 0) {
		ERR_MSG("recv");
		return -1;
	}
	else if (res == 0) {
		circle(db, user->newfd, user->id);
		return -1;
	}
	if (pack.opt_code != SOR) {
		ack.opt_code = ERR;
		sprintf(ack.data, "Wrong pack code");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
		return -1;
	}

	if (sql_search(db, pack.data, &sinfo) == 0) {
		//当前用户是普通用户
		if (strcmp(user->id, "0") != 0) {
			//查询的是自己的信息
			if (strcmp(user->id, sinfo.id) == 0) {
				ack.opt_code = ACK;
				sprintf(ack.data, "%s %s %c %d %s %d %s %d", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.address, sinfo.salary, sinfo.phone, sinfo.status);
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
			//查询的是别人的信息
			else {
				ack.opt_code = ACK;
				sprintf(ack.data, "%s %s %c %d %s", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.phone);
				res = send(user->newfd, &ack, PACK_SIZE, 0);
				if (res < 0) {
					ERR_MSG("send");
					return -1;
				}
			}
		}
		else {
			ack.opt_code = ACK;
			sprintf(ack.data, "%s %s %c %d %s %d %s %d", sinfo.id, sinfo.name, sinfo.sex, sinfo.age, sinfo.address, sinfo.salary, sinfo.phone, sinfo.status);
			res = send(user->newfd, &ack, PACK_SIZE, 0);
			if (res < 0) {
				ERR_MSG("send");
				return -1;
			}
		}
	}
	else {
		ack.opt_code = ERR;
		sprintf(ack.data, "Staff does not exists");
		res = send(user->newfd, &ack, PACK_SIZE, 0);
		if (res < 0) {
			ERR_MSG("send");
			return -1;
		}
	}

	return 0;
}
