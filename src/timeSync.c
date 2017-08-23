#include "../include/timeSync.h"
#include "../include/writeLogFile.h"
#include "../include/mysqlOper.h"


int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port){
	int ret = 0;
	switch(DBType){
		case 0:
			 ret = initDatabase(host,usr,pwd,port,NULL,0);
			if(ret != 0){

			}
			break;
		case 1:
			break;
		default:
			writeLog("unknow database type");
			return UNKNOWNDATABASETYPE;
	}


	return 0;
}