#include "sql.h"

int sql_init(sqlite3** pdb)
{
	//创建数据库
	if (sqlite3_open("./EMS.db", pdb) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*pdb));
		return -1;
	}
	//创建账户表
	char sql[COMMAND_SIZE] = "create table if not exists account(id int primary key,password char,status int);";
	char *errmsg = NULL;
	if (sqlite3_exec(*pdb, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;	
	}
	//创建员工信息表
	bzero(sql, COMMAND_SIZE);
	strcpy(sql, "create table if not exists staff_info(id char primary key,name char,sex char,age int,address char,salary int,phone char,status int);");
	if (sqlite3_exec(*pdb, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;	
	}

	//复位账户状态
	sprintf(sql, "update account set status=0;");
	if (sqlite3_exec(*pdb, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;		
	}

	//添加管理员用户
	sprintf(sql, "insert into account values(0,'admin',0);");
	sqlite3_exec(*pdb, sql, NULL, NULL, &errmsg);

	return 0;
}

int sql_insert_account(sqlite3* db, user_info* info)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "insert into account values('%s','%s',0);",\
			info->id, info->password);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("__%d__%s\n", __LINE__, errmsg);
		return -1;		
	}

	return 0;
}

int sql_insert_staff(sqlite3* db, staff_info* info)
{
	char sql[COMMAND_SIZE] = "";
	sprintf(sql, "insert into staff_info values('%s','%s','%c',%d,'%s',%d,'%s',%d);",\
			info->id, info->name, info->sex, info->age, info->address, info->salary, info->phone, info->status);
	char *errmsg = NULL;
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("__%d__%s\n", __LINE__, errmsg);
		return -1;		
	}

	return 0;
}

int sql_delete_staff(sqlite3* db, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "delete from staff_info where id='%s';", id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;		
	}

	return 0;
}

int sql_delete_account(sqlite3* db, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "delete from account where id='%s';", id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("_%s\n", errmsg);
		return -1;		
	}

	return 0;
}

int sql_modify(sqlite3* db, staff_info* info)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "update staff_info set name='%s',sex='%c',age=%d,address='%s',salary=%d,phone='%s',status=%d where id='%s'",\
			info->name, info->sex, info->age, info->address, info->salary, info->phone, info->status, info->id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;		
	}

	return 0;
}

int sql_search(sqlite3* db, const char* id, staff_info* info)
{
	char sql[COMMAND_SIZE] = "";
	char** result = NULL;
	char* errmsg = NULL;
	int row, column;

	sprintf(sql, "select * from staff_info where id='%s';", id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n", __LINE__, errmsg);
		return -1;
	}
	if (row == 0) {
		sqlite3_free_table(result);
		return -1;
	}
	else {
		strcpy(info->id, result[8]);
		strcpy(info->name, result[9]);
		info->sex = *(char*)result[10];
		info->age = atoi(result[11]);
		strcpy(info->address, result[12]);
		info->salary = atoi(result[13]);
		strcpy(info->phone, result[14]);
		info->status = atoi(result[15]);
	}
	sqlite3_free_table(result);

	return 0;
}

int sql_check_existance(sqlite3* db, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char** result = NULL;
	char* errmsg = NULL;
	int row, column;

	sprintf(sql, "select * from staff_info where id='%s';", id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK)
	{
		printf("__%d__%s\n", __LINE__, errmsg);
		return -1;
	}
	if (row == 0) {
		sqlite3_free_table(result);
		return -1;
	}
	else {
		sqlite3_free_table(result);
		return 0;
	}
}

int sql_check_account(sqlite3* db, user_info* info)
{
	char sql[COMMAND_SIZE] = "";
	char* errmsg = NULL;
	char** result = NULL;
	int row, column;

	sprintf(sql, "select password from account where id='%s';", info->id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;		
	}
	if (row == 0) {
		sqlite3_free_table(result);
		return -1;
	}
	else {
		if (strcmp(info->password, result[1]) != 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
}

int sql_check_staff_status(sqlite3* db, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char* errmsg = NULL;
	char** result = NULL;
	int row, column, status;
	
	sprintf(sql, "select status from staff_info where id='%s';", id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;
	}
	status = atoi(result[1]);
	return status;
}

int sql_check_account_status(sqlite3* db, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char* errmsg = NULL;
	char** result = NULL;
	int row, column, status;
	
	sprintf(sql, "select status from account where id='%s';", id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;
	}
	status = atoi(result[1]);
	return status;
}

int sql_set_staff_status(sqlite3* db, int status, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "update staff_info set status=%d where id='%s';",\
			status, id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;		
	}

	return 0;
}



int sql_set_account_status(sqlite3* db, int newfd, const char* id)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "update account set status=%d where id='%s';",\
			newfd, id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("%s\n", errmsg);
		return -1;		
	}

	return 0;
}

int sql_get_id_by_newfd(sqlite3* db, int newfd, char* id)
{
	char sql[COMMAND_SIZE] = "";
	char* errmsg = NULL;
	char** result = NULL;
	int row, column;
	sprintf(sql, "select id from account where status=%d;", newfd);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;		
	}
	strcpy(id, result[1]);
	sqlite3_free_table(result);

	return 0;
}

int sql_get_account_status(sqlite3* db, char* id, int* status)
{
	char sql[COMMAND_SIZE] = "";
	char* errmsg = NULL;
	char** result = NULL;
	int row, column;
	sprintf(sql, "select status from account where id='%s';", id);
	if (sqlite3_get_table(db, sql, &result, &row, &column, &errmsg) != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
		return -1;		
	}
	*status = atoi(result[1]);
	sqlite3_free_table(result);

	return 0;
}

void sql_reset_status(sqlite3* db, char* id)
{
	char sql[COMMAND_SIZE] = "";
	char *errmsg = NULL;
	sprintf(sql, "update account set status=0 where id='%s';", id);
	if (sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK) {
		printf("__%d__%s\n", __LINE__, errmsg);
	}

}
