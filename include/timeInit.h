#ifndef	TIMESYNC_H
#define	TIMESYNC_H
#include <stdio.h>
#include <stdlib.h>


#define		UNKNOWNDATABASETYPE	-0x0001
#define		INITDATABASEERR		-0x0002
#define		CHANGEDATABASEERR	-0x0003
#define		INIGETALLTABLENAMEERR	-0x0004
#define		SAMENAMEDATABASEERR	-0x0005
#define		READCONFIGFILERR	-0x0006
#define		CREATETABLEERR		-0x0007
#define		CREATETRIGGERLEERR	-0x0008
#define		ADDDATAERR			-0x0009

#define		DBTYP_MYSQL			0x0001
#define		DBTYP_SQLIGHT3		0x0000
#define 	TABLENUM			0x0008

#define		DBTYPNUM			0x0002

//******************配置项************************//
//黑白名单结构体
typedef struct tsync_black_white_list
{
	/* data */
	unsigned char IP[16];
	unsigned char allowed_ntp;
	unsigned char allowed_ntps;
}BW_list;
//黑白名单控制开关
unsigned char tsync_black_white_list_on;
//黑白名单内存地址
BW_list* P_BW_list;

//******************配置项************************//

//***********数据库意外标识***********************//
int accident_database;

//*********数据库连接参数************************//
typedef struct  database_args
{
	/* data */
	int db_type;
	int db_port;
	unsigned char db_name[32];
	unsigned char db_host[32];
	unsigned char db_usr[16];
	unsigned char db_passwd[32]; 
}DB_args;
DB_args third_db;


//数据库操作增删改查函数指针，目前程序一旦初始成功，后续的程序只会使用到下面几个数据库接口。根据不同类型的数据库，初始化的时候会指向不同的函数
int (*P_addData)(const char* tableName,const char*  rowAndValues);
int (*P_deleteData)(const char* tableName,const char*  condition);
int (*P_changeData)(const char* tableName,const char* rowAndValuesAndCon);
int (*P_getData)(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen);
void (*P_close)(void);
int (*P_getLastErr)(unsigned char* err,int errLen);

//防灾数据库函数指针
int (*P_sqlite3_addData)(const char* tableName,const char*  rowAndValues);
int (*P_sqlite3_deleteData)(const char* tableName,const char*  condition);
int (*P_sqlite3_changeData)(const char* tableName,const char* rowAndValuesAndCon);
int (*P_sqlite3_getData)(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen);
void (*P_sqlite3_close)(void);
int (*P_sqlite3_getLastErr)(unsigned char* err,int errLen);


//DBType数据库类型0为Mysql,1为'sqlite3'
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port);

void accidentDeal(void);

#endif