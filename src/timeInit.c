#include "../include/timeInit.h"
#include "../include/writeLogFile.h"
#include "../include/mysqlOper.h"

//初始化完成记得断开数据 连接，至于 后面要用，谁用谁去连接 
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port){
	//读取配置项
	unsigned char key[32*50] = { 0 };//最多50个配置项
	unsigned char value[64*50] = { 0 };
	int configNum = 0;
	int ret = 0;
	ret = readConfig("./tsync.conf",key,value,&configNum);//读取配置文件
	if(ret != 0){
		writeLog("read config error",WRITELOG_ERROR);
		return READCONFIGFILERR;
	}else{
		writeLog("read config success",WRITELOG_SUCCESS);
	}

	switch(DBType){
		case DBTYP_MYSQL:
		{
			int ret = initDatabase(host,usr, pwd, port,NULL,0);
			if(ret != 0){
				unsigned char mysqlDbErr[128] = {0};
				getLastErr(mysqlDbErr,128);
				writeLog(mysqlDbErr,WRITELOG_ERROR);
				return INITDATABASEERR;
			}else{
				writeLog("connect Mysql Database success.",WRITELOG_SUCCESS);
			}
			ret = createDatabase(DBName,NULL);
			if(ret != 0){
				//这种情况一般是数据库已经存在了的情况，所以需要进一步检查是不是应用期望的数据库
				//目前想法是通过表结构和重点表（config表）中的数据来检查，如果是以前的数据库，继续使用，如果不满足，提示改名
				unsigned char mysqlDbErr[128] = {0};
				getLastErr(mysqlDbErr,128);
				writeLog(mysqlDbErr,WRITELOG_WARNING);
				writeLog("now we change into the existed database to check! ",WRITELOG_OTHERS);
				ret = changeDatabase(DBName);
				if(ret != 0){
					getLastErr(mysqlDbErr,128);
					writeLog(mysqlDbErr,WRITELOG_ERROR);
					writeLog("change the existed database err.",WRITELOG_ERROR);
					closeConnect();
					return CHANGEDATABASEERR;
				}else{
					writeLog("change existed database success.",WRITELOG_SUCCESS);
					//检查同名数据库是否是以前使用过的，主要是检查同名库中的表名是否与配置文件中使用的表名相同
					/*code here*/
					unsigned char allTable[32*20]  = { 0 };//最多20个表
					int tableNum = 0;
					if(getAllTableName(allTable,&tableNum) != 0){
						writeLog("get all table  in existed database err!",WRITELOG_ERROR);
						closeConnect();
						return GETALLTABLENAMEERR;
					}else{
						//开始匹配同名数据库中的表与配置文件中的表名相同不
						int i = 0;
						int j = 0;
						int k = 0;
						int tableCount = 0;
						//配置文件中以tables为标识，表示有多少个表，tables=xxx,下面就是表名名称，如table1=config
						while(strcmp(key + i*32,"tables" ) != 0){
							i ++;
						}
						int configTableNum = atoi(value + i*64);
						i ++;
						for(j = i ; j< i + configTableNum; j ++ ){
							for(k = 0; k < tableNum; k ++){
								if(strcmp(value + j*64,allTable + k*32) == 0){
									tableCount ++;
									break;
								}
							}
						}
						if(tableCount == configTableNum){
							writeLog("existed database is the database we might used before ,now we can use it!",WRITELOG_SUCCESS);
							return 0;
						}else{
							writeLog("existed database is not the database we used before,please rename database to complete initial!",WRITELOG_ERROR);
							closeConnect();
							return SAMENAMEDATABASEERR;
						}

					}
				}
			}else{
				writeLog("createDatabase success.",WRITELOG_SUCCESS);
				//数据库创建成功后开始建表
				unsigned char createTableErr[128] = { 0 };
				int ret = createTable("Config","(`Name`  varchar(32) NOT NULL ,`Value`  longtext NULL ,PRIMARY KEY (`Name`))");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table Config success",WRITELOG_SUCCESS);
				}
				ret = createTable("Worklog","(`ID` int NOT NULL AUTO_INCREMENT,\
					`IP` varchar(32) NOT NULL,\
					`Name` varchar(128) DEFAULT NULL,\
					`Content` longtext NOT NULL,\
					`Result` varchar(16) NOT NULL,\
					`Time` datetime NOT NULL,\
					PRIMARY KEY (`ID`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table Worklog success",WRITELOG_SUCCESS);
				}
				//ret = createTable();
				ret = createTable("NTPClient","(`ID`  int NOT NULL AUTO_INCREMENT ,\
					`IP`  varchar(32) NOT NULL ,\
					`NTPCounts`  int NOT NULL DEFAULT 0 ,\
					`NTPSCounts`  int NOT NULL DEFAULT 0 ,\
					`LastNTPType`  varchar(8) NOT NULL ,\
					`LastUpdateTime`  datetime NOT NULL ,\
					PRIMARY KEY (`ID`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table NTPClient success",WRITELOG_SUCCESS);
				}

				ret = createTable("ClientRecord","(`ID`  int NOT NULL AUTO_INCREMENT ,\
					`IP`  varchar(32) NOT NULL ,\
					`Type`  varchar(16) NOT NULL ,\
					`Time`  datetime NOT NULL ,\
					PRIMARY KEY (`ID`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table ClientRecord success",WRITELOG_SUCCESS);
				}

				ret = createTable("TimeStampRecord","(`ID`  int NOT NULL AUTO_INCREMENT,\
					`SerialNum`  varchar(40) NOT NULL ,\
					`IP`  varchar(32) NOT NULL ,\
					`Name`  varchar(128) NULL ,\
					`HashOid`  varchar(128) NOT NULL ,\
					`HashData`  longblob NOT NULL ,\
					`TimeStampContent`  longblob NOT NULL ,\
					`Time`  datetime NOT NULL ,\
					PRIMARY KEY (`ID`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table TimeStampRecord success",WRITELOG_SUCCESS);
				}

				ret = createTable("TimeStampClient","(`ID`  int NOT NULL AUTO_INCREMENT,\
					`IP`  varchar(32) NOT NULL ,\
					`Name`  varchar(128) NOT NULL ,\
					`TimeStampSignCounts`  int NOT NULL DEFAULT 0 ,\
					`LastSignTime`  datetime NULL ,\
					`TimeStampVerifyCounts`  int NOT NULL DEFAULT 0 ,\
					`LastVerifyTime`  datetime NULL ,\
					`Allowed`  int NOT NULL DEFAULT 0,\
					PRIMARY KEY (`ID`) );");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table TimeStampClient success",WRITELOG_SUCCESS);
				}
				ret = createTable("Tsync_Black_White_List","(`ID`  int NOT NULL AUTO_INCREMENT ,\
					`IP`  varchar(32) NOT NULL ,\
					`AllowedNTP`  int NOT NULL DEFAULT 0 ,\
					`AllowedNTPS`  int NOT NULL DEFAULT 0 ,\
					`LastChangeTime`  datetime NOT NULL ,\
					PRIMARY KEY (`ID`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table Tsync_Black_White_List success",WRITELOG_SUCCESS);
				}

				ret = createTable("CountsTable","(`TableName`  varchar(32) NOT NULL ,\
					`Counts`  int NULL DEFAULT 0 ,\
					PRIMARY KEY (`TableName`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table CountsTable success",WRITELOG_SUCCESS);
				}
				writeLog("create all tables success,begin create triggers",WRITELOG_SUCCESS);
				//ret = createTrigger("testTrigger4","table111",1,2,"update Counts set Counts=Counts where id =1;");
				ret = createTrigger("ClientRecord_trigger_insert","ClientRecord",1,0,"declare lastIP varchar(32);\
					declare countIP int(32);\
					declare syncType varchar(16);\
					declare lastUpdatetime datetime;\
					select IP, Type,Time into lastIP,syncType,lastUpdatetime  from ClientRecord where ID = (select max(ID) from ClientRecord) ;\
					set countIP=(select count(*) from NTPClient where IP = lastIP );\
					if countIP = 0 then\
						if syncType = 'NTP' then\
							insert into NTPClient (IP,NTPcounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime );\
							insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);\
						else\
							insert into NTPClient (IP,NTPScounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime);\
							insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);\
						end if;\
					else\
						if syncType = 'NTP' then\
							update NTPClient set NTPcounts=NTPcounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;\
							insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);\
						else\
							update NTPClient set NTPScounts=NTPScounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;\
							insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);\
						end if;\
					end if;\
					update CountsTable set Counts=Counts+1 where TableName = 'ClientRecordCounts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{
					writeLog("create trigger ClientRecord_trigger_insert success",WRITELOG_SUCCESS);
				}
				//closeConnect();
			}
			 
		}
		break;
		case DBTYP_SQLIGHT3:
			break;
		default:
			writeLog("unknow database type.",WRITELOG_ERROR);
			return UNKNOWNDATABASETYPE;
	}


	return 0;
}