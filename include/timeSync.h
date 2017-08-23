#ifndef	TIMESYNC_H
#define	TIMESYNC_H
#include <stdio.h>
#include <stdlib.h>


#define		UNKNOWNDATABASETYPE	-0x0001
#define		INITDATABASEERR		-0x0002



//DBType数据库类型0为Mysql,1为'sqlite3'
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port);


#endif