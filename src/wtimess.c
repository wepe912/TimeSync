#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/mysqlOper.h"
#include "../include/writeLogFile.h"
#include "../include/sqlite3Oper.h"
#include "../include/timeInit.h"
#include <time.h>




int main(int argc ,char* argv[]){
	//argv[1] dbtype,dbname,dbhost,dbusr,dbpasswd,dbport
	const char dbtype_set[DBTYPNUM][16] = {"sqlite3","mysql"};
	if(argc == 1){
		writeLog("Too few parameters! ",WRITELOG_ERROR);
		return -1;
	}
   int ret = initTimeSync(DBTYP_SQLIGHT3,"WtimessEmergency.sqlite3",NULL,NULL,NULL,0);
   if(ret == 0){
   	P_sqlite3_addData = P_addData;
    P_sqlite3_deleteData = P_deleteData;
    P_sqlite3_changeData = P_changeData;
    P_sqlite3_getData = P_getData;
    P_sqlite3_close = P_close;
    P_sqlite3_getLastErr = P_getLastErr;
    writeLog("create WtimessEmergency.sqlite3 success",WRITELOG_SUCCESS);
   }else{
   		writeLog("create WtimessEmergency.sqlite3 fail!",WRITELOG_ERROR);
   		return -1;
   }
   int dbtype = 0;
   for(;dbtype < DBTYPNUM;dbtype ++){
   	if(strcmp(argv[1],dbtype_set[dbtype]) == 0){
   		break;
   	}
   }
   switch(dbtype){
   	case DBTYP_SQLIGHT3:
   	{
   		if(argc < 3){
   			writeLog("Too few parameters! ",WRITELOG_ERROR);
   			return -1;
   		}
   		ret = initTimeSync(DBTYP_SQLIGHT3,argv[2],NULL,NULL,NULL,0);
   		if(ret != 0){
   			unsigned char tmpErr[128] = { 0 };
   			sprintf(tmpErr,"%s%s%s","init sqlite3 database '",argv[2],"' fail !");
   			writeLog(tmpErr,WRITELOG_ERROR);
   		}else{
   			unsigned char tmpErr[128] = { 0 };
   			sprintf(tmpErr,"%s%s%s","init sqlite3 database '",argv[2],"' success !");
   			writeLog(tmpErr,WRITELOG_SUCCESS);
   		}
   		break;
   	}
   	case DBTYP_MYSQL:
   	{
   		int mysqlPort = 3306;
   		if(argc < 6 ){
   			//端口可以不填
   			writeLog("Too few parameters! ",WRITELOG_ERROR);
   			return -1;
   		}
   		if(argc == 7){
   			mysqlPort = atoi(argv[6]);
   		}

   		ret = initTimeSync(DBTYP_MYSQL,argv[2],argv[3],argv[4],argv[5],mysqlPort);
   		if(ret != 0){
   			unsigned char tmpErr[128] = { 0 };
   			sprintf(tmpErr,"%s%s%s","init mysql database '",argv[2],"' fail !");
   			writeLog(tmpErr,WRITELOG_ERROR);
   		}else{
   			unsigned char tmpErr[128] = { 0 };
   			sprintf(tmpErr,"%s%s%s","init mysql database '",argv[2],"' success !");
   			writeLog(tmpErr,WRITELOG_SUCCESS);
   		}

   		accident_database = 0;
        pthread_t accidentDealThread;
        third_db.db_type = DBTYP_MYSQL;
        third_db.db_port = mysqlPort;
        memcpy(third_db.db_name,argv[2],32);
        memcpy(third_db.db_host,argv[3],32);
        memcpy(third_db.db_usr,argv[4],16);
        memcpy(third_db.db_passwd,argv[5],32);
        ret = pthread_create(&accidentDealThread,NULL,(void *)accidentDeal,NULL);
        if(ret != 0){
        	writeLog("create thread accidentDealThread fail!",WRITELOG_ERROR);
        	return -1;
        }else{
        	writeLog("create thread accidentDealThread success!",WRITELOG_SUCCESS);
        }
        ret = pthread_detach(accidentDealThread);
        if(ret != 0){
        	writeLog("pthread_detach thread accidentDealThread fail!",WRITELOG_ERROR);
        	return -1;
        }else{
        	writeLog("pthread_detach thread accidentDealThread success!",WRITELOG_SUCCESS);
        }
   		break;
   	}
   	default:
   		writeLog("unknow database type.",WRITELOG_ERROR);
		return UNKNOWNDATABASETYPE;
   }



	return 0;
}