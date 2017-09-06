#include "../include/timeInit.h"
#include "../include/writeLogFile.h"
#include "../include/mysqlOper.h"
#include "../include/sqlite3Oper.h"

//初始化完成记得断开数据 连接，至于 后面要用，谁用谁去连接 
int initTimeSync(int DBType,const char* DBName,const char* host,const char* usr,const char* pwd,unsigned int port){
	//读取配置项
	unsigned char needtables[9][64] = {"config","client_record","ntp_client","timestamp_client","work_log","timestamp_record","tsync_black_white_list","counts_table"};
	//unsigned char key[32*50] = { 0 };//最多50个配置项
	//unsigned char value[64*50] = { 0 };
	//int configNum = 0;
	//int ret = 0;
	//ret = readConfig("./tsync.conf",key,value,&configNum);//读取配置文件
	//if(ret != 0){
	//	writeLog("read config error",WRITELOG_ERROR);
	//	return READCONFIGFILERR;
	//}else{
	//	writeLog("read config success",WRITELOG_SUCCESS);
	//}

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
						//int k = 0;
						int tableCount = 0;
						//配置文件中以tables为标识，表示有多少个表，tables=xxx,下面就是表名名称，如table1=config
						//while(strcmp(key + i*32,"tables" ) != 0){
						//	i ++;
						//}
						//int configTableNum = atoi(value + i*64);
						//i ++;
						//for(j = i ; j< i + configTableNum; j ++ ){
						//	for(k = 0; k < tableNum; k ++){
						//		if(strcmp(value + j*64,allTable + k*32) == 0){
						//			tableCount ++;
						//			break;
						//		}
						//	}
						//}
						//if(tableCount == configTableNum){
						for(i = 0 ; i < TABLENUM;i ++){
							for(j = 0; j < tableNum; j ++){
								if(strcmp(needtables[i],allTable + j*32) == 0){
									tableCount ++;
								}
							}
						}
						if(tableCount == TABLENUM){
							writeLog("existed database is the database we might used before ,now we can use it!",WRITELOG_SUCCESS);
							//初始化完成断不断开数据库？
							//closeConnect();
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
				int ret = createTable("config","(`Name`  varchar(32) NOT NULL ,`Value`  longtext NULL ,PRIMARY KEY (`Name`))");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table Config success",WRITELOG_SUCCESS);
				}
				ret = createTable("work_log","(`ID` int NOT NULL AUTO_INCREMENT,\
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
					writeLog("create table work_log success",WRITELOG_SUCCESS);
				}
				//ret = createTable();
				ret = createTable("ntp_client","(`ID`  int NOT NULL AUTO_INCREMENT ,\
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
					writeLog("create table ntp_client success",WRITELOG_SUCCESS);
				}

				ret = createTable("client_record","(`ID`  int NOT NULL AUTO_INCREMENT ,\
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
					writeLog("create table client_record success",WRITELOG_SUCCESS);
				}

				ret = createTable("timestamp_record","(`ID`  int NOT NULL AUTO_INCREMENT,\
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
					writeLog("create table timestamp_record success",WRITELOG_SUCCESS);
				}

				ret = createTable("timestamp_client","(`ID`  int NOT NULL AUTO_INCREMENT,\
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
					writeLog("create table timestamp_client success",WRITELOG_SUCCESS);
				}
				ret = createTable("tsync_black_white_list","(`ID`  int NOT NULL AUTO_INCREMENT ,\
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
					writeLog("create table tsync_black_white_list success",WRITELOG_SUCCESS);
				}

				ret = createTable("counts_table","(`TableName`  varchar(32) NOT NULL ,\
					`Counts`  int NULL DEFAULT 0 ,\
					PRIMARY KEY (`TableName`));");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETABLEERR;
				}else{
					writeLog("create table counts_table success",WRITELOG_SUCCESS);
				}
				writeLog("create all tables success,begin create triggers",WRITELOG_SUCCESS);
				//ret = createTrigger("testTrigger4","table111",1,2,"update Counts set Counts=Counts where id =1;");
				ret = createTrigger("client_record_trigger_insert","client_record",1,0,"declare lastIP varchar(32);\
					declare countIP int(32);\
					declare syncType varchar(16);\
					declare lastUpdatetime datetime;\
					select IP, Type,Time into lastIP,syncType,lastUpdatetime  from client_record where ID = (select max(ID) from client_record) ;\
					set countIP=(select count(*) from ntp_client where IP = lastIP );\
					if countIP = 0 then\
						if syncType = 'NTP' then\
							insert into ntp_client (IP,NTPcounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime );\
							insert into work_log(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);\
						else\
							insert into ntp_client (IP,NTPScounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime);\
							insert into work_log(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);\
						end if;\
					else\
						if syncType = 'NTP' then\
							update ntp_client set NTPcounts=NTPcounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;\
							insert into work_log(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);\
						else\
							update ntp_client set NTPScounts=NTPScounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;\
							insert into work_log(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);\
						end if;\
					end if;\
					update counts_table set Counts=Counts+1 where TableName = 'client_record_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{
					writeLog("create trigger client_record_trigger_insert success",WRITELOG_SUCCESS);
				}

				ret = createTrigger("client_record_trigger_delete","client_record",1,1,"update counts_table set Counts=Counts-1 where TableName = 'client_record_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{
					writeLog("create trigger client_record_trigger_delete success",WRITELOG_SUCCESS);
				}

				ret = createTrigger("timestamp_record_trigger_insert","timestamp_record",1,0,"declare lastIP varchar(32);\
					declare countIP int(32);\
					declare appName varchar(128);\
					declare lastUpdatetime datetime;\
					select IP, Name,Time into lastIP,appName,lastUpdatetime  from timestamp_record where ID = (select max(ID) from timestamp_record) ;\
					update timestamp_client set TimeStampSignCounts=TimeStampSignCounts + 1 , LastSignTime = lastUpdatetime where IP=lastIP and Name=appName;\
					insert into work_log(IP,Name,Content,Result,Time)values(lastIP,appName,'Signed New TimeStamp ','success',lastUpdatetime);\
					update counts_table set Counts=Counts+1 where TableName = 'timestamp_record_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{
					writeLog("create trigger timestamp_record_trigger_insert success",WRITELOG_SUCCESS);
				}

				ret = createTrigger("timestamp_record_trigger_delete","timestamp_record",1,1,"update counts_table set Counts=Counts-1 where TableName = 'timestamp_record_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{
					writeLog("create trigger timestamp_record_trigger_delete success",WRITELOG_SUCCESS);
				}
						//建Worklog_trigger_insert
				ret = createTrigger("work_log_trigger_insert","work_log",1,0,"update counts_table set Counts=Counts + 1 where TableName = 'work_log_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{

					writeLog("create trigger work_log_trigger_insert success",WRITELOG_SUCCESS);
					//sqlite3_close_database();
				}


				//建Worklog_trigger_delete
				ret = createTrigger("work_log_trigger_delete","work_log",1,1,"update counts_table set Counts=Counts-1 where TableName = 'work_log_counts';");
				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return CREATETRIGGERLEERR;
				}else{

					writeLog("create trigger work_log_trigger_delete success",WRITELOG_SUCCESS);
					//sqlite3_close_database();
				}
				writeLog("create all triggers success",WRITELOG_SUCCESS);
				ret = addData("counts_table","(TableName,Counts)values('client_record_counts',0),('timestamp_record_counts',0),('work_log_counts',0);");

				if(ret != 0){
					getLastErr(createTableErr,128);
					writeLog(createTableErr,WRITELOG_ERROR);
					closeConnect();
					return ADDDATAERR;
				}else{

					writeLog("Add data into counts_table success ",WRITELOG_SUCCESS);
					//sqlite3_close_database();
				}
				P_addData = addData;
				P_deleteData = deleteData;
				P_changeData = changeData;
				P_getData = getData;
				P_close = closeConnect;
				P_getLastErr = getLastErr;
				//write config into config table
				/**************code here*****************/

				//初始化完成断不断开数据库？
				//closeConnect();
				return 0;
			}
			 
		}
		break;
		case DBTYP_SQLIGHT3:
		{
			unsigned char sqlite3_err[128] = { 0 };
			int ret = sqlite3_init(DBName);
			if(ret != 0){
				sqlite3_get_lasterr(sqlite3_err,128);
				writeLog(sqlite3_err,WRITELOG_ERROR);
				return INITDATABASEERR;
			}else{
				writeLog("connect database success",WRITELOG_SUCCESS);
				unsigned char sqlite3_tables[32*20] = { 0 };
				int sqlite3_tables_num = 0;
				ret = sqlite3_get_alltable_name(sqlite3_tables,&sqlite3_tables_num);
				if( ret != 0){
					sqlite3_get_lasterr(sqlite3_err,128);
					writeLog(sqlite3_err,WRITELOG_ERROR);
					return INITDATABASEERR;
				}else{
					//检查是否是之前使用过的数据库，对比第一个表是否在配置文件中即可
					if(sqlite3_tables_num == 0){
						writeLog("it is a new database,we can use it to init system ...",WRITELOG_SUCCESS);
						/*code here to create tables and triggers*/
						//建Config表
						ret = sqlite3_create_table("config","(`Name`  varchar(32) NOT NULL ,`Value`  longtext NULL ,PRIMARY KEY (`Name`))");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table config success",WRITELOG_SUCCESS);
						}
						//建Worklog表
						ret = sqlite3_create_table("work_log","(`ID` INTEGER PRIMARY KEY AUTOINCREMENT,\
							`IP` varchar(32) NOT NULL,\
							`Name` varchar(128) DEFAULT NULL,\
							`Content` longtext NOT NULL,\
							`Result` varchar(16) NOT NULL,\
							`Time` datetime NOT NULL);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table work_log success",WRITELOG_SUCCESS);
						}
						//建NTPClient表
						ret = sqlite3_create_table("ntp_client","(`ID`  INTEGER PRIMARY KEY AUTOINCREMENT,\
							`IP`  varchar(32) NOT NULL ,\
							`NTPCounts`  int NOT NULL DEFAULT 0 ,\
							`NTPSCounts`  int NOT NULL DEFAULT 0 ,\
							`LastNTPType`  varchar(8) NOT NULL ,\
							`LastUpdateTime`  datetime NOT NULL);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table ntp_client success",WRITELOG_SUCCESS);
						}
						//建ClientRecord表
						ret = sqlite3_create_table("client_record","(`ID` INTEGER PRIMARY KEY AUTOINCREMENT ,\
							`IP`  varchar(32) NOT NULL ,\
							`Type`  varchar(16) NOT NULL ,\
							`Time`  datetime NOT NULL );");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table client_record success",WRITELOG_SUCCESS);
						}
						//建TimeStampRecord表
						ret = sqlite3_create_table("timestamp_record","(`ID` INTEGER PRIMARY KEY AUTOINCREMENT ,\
							`SerialNum`  varchar(40) NOT NULL ,\
							`IP`  varchar(32) NOT NULL ,\
							`Name`  varchar(128) NULL ,\
							`HashOid`  varchar(128) NOT NULL ,\
							`HashData`  longblob NOT NULL ,\
							`TimeStampContent`  longblob NOT NULL ,\
							`Time`  datetime NOT NULL);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table timestamp_record success",WRITELOG_SUCCESS);
						}
						//建TimeStampClient表
						ret = sqlite3_create_table("timestamp_client","(`ID` INTEGER PRIMARY KEY AUTOINCREMENT,\
							`IP`  varchar(32) NOT NULL ,\
							`Name`  varchar(128) NOT NULL ,\
							`TimeStampSignCounts`  int NOT NULL DEFAULT 0 ,\
							`LastSignTime`  datetime NULL ,\
							`TimeStampVerifyCounts`  int NOT NULL DEFAULT 0 ,\
							`LastVerifyTime`  datetime NULL ,\
							`Allowed`  int NOT NULL DEFAULT 0);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table timestamp_client success",WRITELOG_SUCCESS);
						}
						//建Tsync_Black_White_List表
						ret = sqlite3_create_table("tsync_black_white_list","(`ID` INTEGER PRIMARY KEY AUTOINCREMENT ,\
							`IP`  varchar(32) NOT NULL ,\
							`AllowedNTP`  int NOT NULL DEFAULT 0 ,\
							`AllowedNTPS`  int NOT NULL DEFAULT 0 ,\
							`LastChangeTime`  datetime NOT NULL);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table tsync_black_white_list success",WRITELOG_SUCCESS);
						}
						//建CountsTable表
						ret = sqlite3_create_table("counts_table","(`TableName`  varchar(32) NOT NULL ,\
							`Counts`  int NULL DEFAULT 0);");

						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETABLEERR;
						}else{
							writeLog("create table counts_table success",WRITELOG_SUCCESS);
						}
						writeLog("create all tables success,begin create triggers",WRITELOG_SUCCESS);
						//建ClientRecord_trigger_insert
						
						ret = sqlite3_createTrigger("client_record_trigger_insert","client_record",1,0," UPDATE ntp_client SET NTPCounts = NTPCounts + 1, \
							LastNTPType = 'NTP',LastUpdateTime = new.Time WHERE (SELECT IP FROM ntp_client WHERE IP = new.IP AND new.Type = 'NTP') \
							AND IP = new.IP;\
							UPDATE ntp_client SET NTPSCounts = NTPSCounts + 1,LastNTPType = 'NTPS',LastUpdateTime = new.Time WHERE (SELECT IP FROM ntp_client \
							WHERE IP = new.IP AND new.Type = 'NTPS') AND IP = new.IP; \
							INSERT INTO ntp_client (IP,LastNTPType,LastUpdateTime) SELECT IP,Type,\
							Time FROM client_record WHERE (SELECT IP FROM ntp_client WHERE IP = new.IP) ISNULL AND IP = new.IP;\
							UPDATE ntp_client SET NTPCounts = 1 WHERE (SELECT NTPCounts FROM ntp_client WHERE IP = new.IP)=0 AND \
							( SELECT NTPSCounts FROM ntp_client  WHERE IP = new.IP )= 0 AND new.Type = 'NTP' AND IP = new.IP;\
							UPDATE ntp_client SET NTPSCounts = 1 WHERE ( SELECT NTPCounts FROM ntp_client  WHERE IP = new.IP )= 0 \
							AND ( SELECT NTPSCounts FROM ntp_client WHERE IP = new.IP)=0 AND new.Type = 'NTPS' AND IP = new.IP;\
							INSERT INTO work_log (IP, Content, Result,Time) VALUES (new.IP,new.Type,'success',new.Time);\
							update counts_table set Counts=Counts + 1 where TableName = 'client_record_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger client_record_trigger_insert success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						//建ClientRecord_trigger_delete
						ret = sqlite3_createTrigger("client_record_trigger_delete","client_record",1,1,"update counts_table set Counts=Counts-1 where TableName = 'client_record_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger client_record_trigger_delete success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						//建TimeStampRecord_trigger_insert
						ret = sqlite3_createTrigger("timestamp_record_trigger_insert","timestamp_record",1,0,"update timeStamp_client set \
							TimeStampSignCounts=TimeStampSignCounts+1,LastSignTime=new.Time where IP=new.IP and Name=new.Name;\
							INSERT INTO work_log (IP,Name, Content, Result,Time) VALUES (new.IP,new.Name,'Sign new TimeStamp','success',new.Time);\
							update counts_table set Counts=Counts + 1 where TableName = 'timestamp_record_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger timestamp_record_trigger_insert success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						//建TimeStampRecord_trigger_delete
						ret = sqlite3_createTrigger("timestamp_record_trigger_delete","timestamp_record",1,1,"update counts_table set Counts=Counts-1 where TableName = 'timestamp_record_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger timestamp_record_trigger_delete success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						//建Worklog_trigger_insert
						ret = sqlite3_createTrigger("work_log_trigger_insert","work_log",1,0,"update counts_table set Counts=Counts + 1 where TableName = 'work_log_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger work_log_trigger_insert success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}


						//建Worklog_trigger_delete
						ret = sqlite3_createTrigger("work_log_trigger_delete","work_log",1,1,"update counts_table set Counts=Counts-1 where TableName = 'work_log_counts';");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return CREATETRIGGERLEERR;
						}else{

							writeLog("create trigger work_log_trigger_delete success",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						writeLog("create all triggers success",WRITELOG_SUCCESS);
						ret = sqlite3_add_data("counts_table","(TableName,Counts)values('client_record_counts',0),('timestamp_record_counts',0),('work_log_counts',0);");
						if(ret != 0){
							sqlite3_get_lasterr(sqlite3_err,128);
							writeLog(sqlite3_err,WRITELOG_ERROR);
							sqlite3_close_database();
							return ADDDATAERR;
						}else{

							writeLog("Add data into counts_table success ",WRITELOG_SUCCESS);
							//sqlite3_close_database();
						}
						P_addData = sqlite3_add_data;
						P_deleteData = sqlite3_delete_data;
						P_changeData = sqlite3_change_data;
						P_getData = sqlite3_get_data;
						P_close = sqlite3_close_database;
						P_getLastErr = sqlite3_get_lasterr;
					}else{
						//该数据库之前使用过，判断能不能继续使用，只要配置中有表有一个在已经存在的数据库中，表示该表被用过
						writeLog("exist samename database,now we check it !",WRITELOG_OTHERS);
						int i = 0;
						int tmpCountNum = 0;
						//while(strcmp(key + i*32,"tables" ) != 0){
						//		i ++;
						//}
						//int configTableNum = atoi(value + i*64);
						////i ++;
						//tmpCountNum = configTableNum + i;

						for (i = 0; i < TABLENUM; i++)
						{
							/* code */
							//if(strcmp(sqlite3_tables,value +i*64) == 0){
							if(strcmp(sqlite3_tables,needtables[i]) == 0){
								break;
							}
						}
						if(i < TABLENUM){
							writeLog("the existed database is used before by the system,we can use it !",WRITELOG_SUCCESS);
						}else{
							writeLog("the existed database is used by another system,we can not use it,please change the database name",WRITELOG_ERROR);
							sqlite3_close_database();
							return SAMENAMEDATABASEERR;
						}
					}
				}
			}
			break;
		}
		default:
			writeLog("unknow database type.",WRITELOG_ERROR);
			return UNKNOWNDATABASETYPE;
	}


	return 0;
}