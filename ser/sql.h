#ifndef __SQL_H__
#define __SQL_H__

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include "public.h"

#define COMMAND_SIZE 512

int sql_init(sqlite3**);
int sql_insert_account(sqlite3*, user_info*);
int sql_insert_staff(sqlite3*, staff_info*);
int sql_delete_staff(sqlite3*, const char*);
int sql_delete_account(sqlite3*, const char*);
int sql_modify(sqlite3*, staff_info*);
int sql_search(sqlite3*, const char*, staff_info*);
int sql_check_account(sqlite3*, user_info*);
int sql_set_account_status(sqlite3*, int, const char*);
int sql_check_staff_status(sqlite3*, const char*);
int sql_check_account_status(sqlite3*, const char*);
int sql_set_staff_status(sqlite3*, int, const char*);
int sql_get_id_by_newfd(sqlite3*, int, char*);
int sql_get_account_status(sqlite3*, char*, int*);
void sql_reset_status(sqlite3*, char*);
int sql_check_existance(sqlite3*, const char*);

#endif
