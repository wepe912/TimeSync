#include "../include/timeSync.h"
#include "../include/writeLogFile.h"
#include "../include/mysqlOper.h"


int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port){
	
	unsigned char key[32*50] = { 0 };//最多50个配置项
	unsigned char value[64*50] = { 0 };
	int configNum = 0;
	int ret = 0;
	ret = readConfig(key,value,configNum);//读取配置文件
	if(ret != 0){
		writeLog("read config error",WRITELOG_ERROR);
	}else{
		writeLog("read config success",WRITELOG_SUCCESS);
	}

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
				writeLog("connect Database success.",WRITELOG_SUCCESS);
			}
			ret = createDatabase(DBName);
			if(ret != 0){
				//这种情况一般是数据库已经存在了的情况，所以需要进一步检查是不是应用期望的数据库
				//目前想法是通过表结构和重点表（config表）中的数据来检查，如果是以前的数据库，继续使用，如果不满足，提示改名
				unsigned char mysqlDbErr[128] = {0};
				getLastErr(mysqlDbErr,128);
				writeLog(mysqlDbErr,WRITELOG_WARNING);
				ret = changeDatabase(DBName);
				if(ret != 0){
					getLastErr(mysqlDbErr,128);
					writeLog(mysqlDbErr,WRITELOG_ERROR);
					writeLog("this database is not the one we have used ! please change another database name.",WRITELOG_OTHERS);
					closeConnect();
					return CHANGEDATABASEERR;
				}else{
					writeLog("changeDatabase success.",WRITELOG_SUCCESS);
					//检查同名数据库是否是以前使用过的，主要是检查同名库中的表名是否与配置文件中使用的表名相同
					/*code here*/
					unsigned char allTable[32*20]  = { 0 };//最多20个表
					int tableNum = 0;
					if(getAllTableName(allTable,&tableNum) != 0){
						writeLog("get same_name_database all table err!",WRITELOG_ERROR);
						closeConnect();
						return GETALLTABLENAMEERR;
					}else{
						//开始匹配同名数据库中的表与配置文件中的表名相同不
					}
				}
			}else{
				writeLog("createDatabase success.",WRITELOG_SUCCESS);
			}
			 
		}
		break;
		case 1:
			break;
		default:
			writeLog("unknow database type.",WRITELOG_ERROR);
			return UNKNOWNDATABASETYPE;
	}


	return 0;
}