#include "ser.h"
#include "sql.h"

int main(int argc, const char *argv[])
{
	sqlite3* db = NULL;
	if (sql_init(&db) == -1) {
		printf("数据库初始化失败!\n");
		return -1;
	}
	int sfd = ser_create(db);
	if (sfd = -1) {
		printf("创建服务器失败!\n");
		return -1;
	}

	return 0;
}
