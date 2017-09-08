#ifndef MYSQLOPER_H
#define MYSQLOPER_H
#include <stdio.h>
#include <stdlib.h>
#include "./mysql_include/mysql.h"
#include "./writeLogFile.h"


#define CONNECTERR  -0x0001
#define MALLOCERR   -0x0002
#define GETDATAERR	-0x0008
#define OUTPUTSAPACELESS	-0x0009
#define	GETALLTABLENAMEERR	-0x000a
#define	TRIGGEROPERERR		-0x000b
#define	CREATETRIGGERERR	-0x000c
#define	DROPTRIGGERERR		-0x000d


#define COMMENlEN	0x0040
#define MIDDLELEN	0x0080
#define LONGLEN		0x0400


MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;

int initDatabase(const char* host,const char* usr,const char* pwd,unsigned int port,const char* unix_socket,unsigned long clientflag);

void closeConnect();

int createDatabase(const char* databasename,const char* arg);

int changeDatabase(const char* databasename);

int deleteDatabase(const char* databasename);

int createTable(const char* tableName,const char* nameAndType);

int dropTable(const char*tableName);

int addData(const char* tableName,const char*  rowAndValues);

int deleteData(const char* tableName,const char*  condition);

int changeData(const char* tableName,const char* rowAndValuesAndCon);

int getData(const char* tableName,const char* selectArges,const char* condition,int *rowNum,int*fieldNum, int* interval , unsigned char* data,int dataLen);

int getAllTableName(unsigned char* tableName,int* tableNum);

int transactionDeal(const char* transaction);

int getLastErr(unsigned char* err,int errLen);

int createTrigger(const char* triggerName,const char* triggerTable,int beforeOrafter,int triggerOper,const char* oper);

int dropTrigger(const char* triggerName);
#endif