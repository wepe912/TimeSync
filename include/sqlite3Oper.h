#ifndef	SQLITE3OPER_H
#define	SQLITE3OPER_H
#include <stdio.h>
#include <stdlib.h>
#include "../include/sqlite3.h"

//#define		DATABASENMAE	"./MyDatabase.sqlite3"

#define		SQLITE_OPENDATABAS_ERR	-0x0002
#define		SQLITE_CREATE_TABLE_ERR -0x0003
#define		SQLITE_DROP_TABLE_ERR	-0x0004
#define		SQLITE_INSERT_TABLE_ERR	-0x0005
#define		SQLITE_DELETE_TABLE_ERR	-0x0006
#define		SQLITE_UPDATE_DATA_ERR	-0x0007
#define		SQLITE_GET_DATA_ERR		-0x0008
#define		OUTPUT_SAPACE_ERR		-0x0009



#define		SQLITE_MALLOCERR		-0x000a


sqlite3 *sqlite3Db;
unsigned char* sqlite3_err;


int sqlite3_init(const char* databaseName);
void sqlite3_close_database(void);

int sqlite3_create_table(const char* tableName,const char* nameAndType);

int sqlite3_drop_table(const char* tableName);

int sqlite3_add_data(const char* tableName,const char*  rowAndValues);

int sqlite3_delete_data(const char* tableName,const char*  condition);

int sqlite3_change_data(const char* tableName,const char* rowAndValuesAndCon);

int sqlite3_get_alltable_name(unsigned char* tableName,int* tableNum);

int sqlite3_get_data(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen);

int sqlite3_get_lasterr(unsigned char* err,int errLen);
#endif