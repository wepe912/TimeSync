#include "../include/timeSync.h"
#include "../include/writeLogFile.h"
#include "../include/mysqlOper.h"


int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port){
	switch(DBType){
		case 0:
		{
			int ret = initDatabase(host,usr, pwd, port,NULL,0);
			if(ret != 0){
				unsigned char mysqlDbErr[128] = {0};
				getLastErr(mysqlDbErr,128);
				writeLog(mysqlDbErr,WRITELOG_ERROR);
				return INITDATABASEERR;
			}else{
				writeLog("initDatabase success",WRITELOG_SUCCESS);
			}
			ret = createDatabase(DBName);
			if(ret != 0){
				//这种情况一般是数据库已经存在了的情况，所以需要进一步检查是不是应用期望的数据库
				//目前想法是通过表结构和重点表（config表）中的数据来检查，如果是以前的数据库，继续使用，如果不满足，提示改名
				unsigned char mysqlDbErr[128] = {0};
				getLastErr(mysqlDbErr,128);
				writeLog(mysqlDbErr,WRITELOG_WARNING);
			}else{
				writeLog("createDatabase success",WRITELOG_SUCCESS);
			}
			 
		}
		break;
		case 1:
			break;
		default:
			writeLog("unknow database type",WRITELOG_ERROR);
			return UNKNOWNDATABASETYPE;
	}


	return 0;
}