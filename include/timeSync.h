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


#define		DBTYP_MYSQL			0x0000
#define		DBTYP_SQLIGHT3		0x0001

//DBType数据库类型0为Mysql,1为'sqlite3'
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port);


#endif