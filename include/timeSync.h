#ifndef	TIMESYNC_H
#define	TIMESYNC_H
#include <stdio.h>
#include <stdlib.h>


#define		UNKNOWNDATABASETYPE	-0x0001
#define		INITDATABASEERR		-0x0002
#define		CHANGEDATABASEERR	-0x0003
#define		GETALLTABLENAMEERR	-0x0004
#define		SAMENAMEDATABASEERR	-0x0005
#define		READCONFIGFILERR	-0x0006
#define		CREATETABLEERR		-0x0007

#define		DBTYP_MYSQL			0x0000
#define		DBTYP_SQLIGHT3		0x0001


//数据库操作增删改查函数指针，目前程序一旦初始成功，后续的程序只会使用到下面几个数据库接口。
int (*P_addData)(const char* tableName,const char*  rowAndValues);
int (*P_deleteData)(const char* tableName,const char*  condition);
int (*P_changeData)(const char* tableName,const char* rowAndValuesAndCon);
int (*P_getData)(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen);


//DBType数据库类型0为Mysql,1为'sqlite3'
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port);


#endif