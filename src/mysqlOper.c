#include "../include/mysqlOper.h"

/*初始化mysql数据库,先连接默认的mysql库*/
int initDatabase(const char* host,const char* usr,const char* pwd,unsigned int port,const char* unix_socket,unsigned long clientflag){


	mysql_library_init(0, NULL, NULL);
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,host,usr,pwd,"mysql",port,unix_socket,clientflag)){
    	//printf("无法连接到数据库，错误原因：%s\n", mysql_error(&mysql));
    	//writeLog(mysql_error(&mysql));
    	return CONNECTERR;
    }else{
    	return 0;
    }

}

/*断开mysql数据库连接*/
void closeConnect(){
	mysql_close(&mysql);
}



/*新建数据库*/
int createDatabase(const char* databaseName) {
	char sqlStr[COMMENlEN] = { 0 };
	sprintf(sqlStr, "%s%s", "create database ", databaseName);
	int ret = mysql_query(&mysql, sqlStr);
	if(ret == 0){
		ret = changeDatabase(databaseName);
		return ret;
	}else{
		//writeLog(mysql_error(&mysql));
		return ret;
	}

}

/*删除数据库*/
int deleteDatabase(const char* databasename) {
	char sqlStr[COMMENlEN] = { 0 };
	sprintf(sqlStr, "%s%s", "drop database ", databasename);
	return mysql_query(&mysql, sqlStr);
}
/*切换数据库*/
int changeDatabase(const char* databaseName){
	char sqlStrChange[COMMENlEN] = { 0 };
	sprintf(sqlStrChange, "%s%s", "use ", databaseName);
	return mysql_query(&mysql, sqlStrChange);

}

/*创建表，nameAndType参数如："(id int, name varchar(32))"*/
int createTable(const char* tableName,const char* nameAndType) {
	char sqlStrCreate[MIDDLELEN] = { 0 };
	sprintf(sqlStrCreate, "%s%s%s", "create table ",tableName,nameAndType);
	return mysql_query(&mysql, sqlStrCreate);
		//printf("%s", mysql_error(&mysql));
}

/*删除表*/
int dropTable(const char*tableName) {
	char sqlStrDrop[MIDDLELEN] = { 0 };
	sprintf(sqlStrDrop, "%s%s", "drop table ", tableName);
	return mysql_query(&mysql, sqlStrDrop);
}

/*向表中插入数据*/
int addData(const char* tableName,const char*  rowAndValues){
	if(strlen(rowAndValues) < LONGLEN - strlen(tableName) - 16){
		unsigned char sqlStrInsert[LONGLEN] = { 0 };
		sprintf(sqlStrInsert,"%s%s%s","insert into ",tableName,rowAndValues);
		return mysql_query(&mysql,sqlStrInsert);
	}else{
		unsigned char* sqlStrInsert = NULL; 
		sqlStrInsert = (unsigned char*)calloc(strlen(rowAndValues) + strlen(tableName) + 16,sizeof(char));
		if(sqlStrInsert == NULL){
			return MALLOCERR;
		}	
		sprintf(sqlStrInsert,"%s%s%s","insert into ",tableName,rowAndValues);	
		int ret = mysql_query(&mysql,sqlStrInsert);
		free(sqlStrInsert);
		return ret;
	}
}


int deleteData(const char* tableName,const char*  condition){
	if(strlen(condition) < LONGLEN - strlen(tableName) - 16){
		unsigned char sqlStrDelete[LONGLEN] = { 0 };
		if(condition == NULL){
			sprintf(sqlStrDelete,"%s%s","delete from ",tableName);
		}else{
			sprintf(sqlStrDelete,"%s%s%s%s","delete from ",tableName," ",condition);
		}		
		return mysql_query(&mysql,sqlStrDelete);
	}else{
		unsigned char* sqlStrDelete = NULL; 
		if(condition == NULL){
			sqlStrDelete = (unsigned char*)calloc(strlen(tableName) + 16,1);
			if(sqlStrDelete == NULL){
				return MALLOCERR;
			}
			sprintf(sqlStrDelete,"%s%s","delete from ",tableName);
		}else{
			sqlStrDelete = (unsigned char*)calloc(strlen(condition) + strlen(tableName) + 16,1);
			if(sqlStrDelete == NULL){
				return MALLOCERR;
			}
			sprintf(sqlStrDelete,"%s%s%s%s","delete from ",tableName," ",condition);
		}	
		int ret = mysql_query(&mysql,sqlStrDelete);
		free(sqlStrDelete);
		return ret;
	}
}

int changeData(const char* tableName,const char* rowAndValuesAndCon){
	if(strlen(rowAndValuesAndCon) < LONGLEN - strlen(tableName) - 16){
		unsigned char sqlStrUpdate[LONGLEN] = { 0 };
		sprintf(sqlStrUpdate,"%s%s%s%s","update ",tableName," set ",rowAndValuesAndCon);
		return mysql_query(&mysql,sqlStrUpdate);
	}else{
		unsigned char* sqlStrUpdate = NULL; 
		sqlStrUpdate = (unsigned char*)calloc(strlen(rowAndValuesAndCon) + strlen(tableName) + 16,sizeof(char));
		if(sqlStrUpdate == NULL){
			return MALLOCERR;
		}	
		sprintf(sqlStrUpdate,"%s%s%s%s","update ",tableName," set ",rowAndValuesAndCon);	
		int ret = mysql_query(&mysql,sqlStrUpdate);
		free(sqlStrUpdate);
		return ret;
	}
}
int getAllTableName(unsigned char* tableName,int *tableNum){
	MYSQL_RES *resPtr;
	MYSQL_ROW sqlRow;
	int ret = mysql_query(&mysql,"show tables");
	if(ret != 0 ){
		return GETALLTABLENAMEERR;
	}
	resPtr = mysql_store_result(&mysql); 
	*tableNum = mysql_num_rows(resPtr);
	int i = 0;
	for( i = 0; i < *tableNum; i ++){
		sqlRow = mysql_fetch_row(resPtr);
		memcpy(tableName + i*32,sqlRow[0],strlen(sqlRow[0]));
	}
	return 0;
}

/*查询数据 ，selectArges是select参数，condition为select条件，rowNum为查询返回行数，fieldNum为字段数，interval为各字段间的存储间隔，data为select出的数据*/
int getData(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen){
		unsigned char* sqlStrSelect = NULL; 
		MYSQL_RES *resPtr;
		MYSQL_ROW sqlRow;
		int sqlLen = 0 ;
		//无where条件
		if(condition == NULL){
			sqlLen =  strlen(tableName) + strlen(selectArges)+ 16;
			sqlStrSelect = (unsigned char*)calloc(sqlLen,sizeof(char));
			if(sqlStrSelect == NULL){
				return MALLOCERR;
			}	
			sprintf(sqlStrSelect,"%s%s%s%s","select ",selectArges," from ", tableName);
		}else{
			//有where条件
			sqlLen = strlen(condition) + strlen(tableName) + strlen(selectArges)+ 16;
			sqlStrSelect = (unsigned char*)calloc(sqlLen,sizeof(char));
			if(sqlStrSelect == NULL){
				return MALLOCERR;
			}
			sprintf(sqlStrSelect,"%s%s%s%s%s","select ",selectArges," from ", tableName,condition);
		}
		int ret = mysql_query(&mysql,sqlStrSelect);
		if(ret != 0){
			free(sqlStrSelect);
			return ret;
		}
		resPtr = mysql_store_result(&mysql); 
		if(resPtr != NULL){
			*rowNum = mysql_num_rows(resPtr);
			*fieldNum = mysql_num_fields(resPtr);
			int simple = 0;
			//小于30记录全面记录进行采样，大于30条，三分之一的随机条目进行采样
			//以确定每条康的总长度以及字段间的间隔，便于存储和获取
			if(*rowNum < 30){
				simple = (*rowNum);
			}else{
				simple = (*rowNum) / 3;
			}
			//int simple = (*rowNum) / 3;
			int i = 0,j = 0;
			int randNum = 0;
			srand((unsigned)time(NULL));
			for(i = 0 ; i < simple;i ++){
				randNum = rand() % (*rowNum);
				//printf("%d\n",randNum );
				mysql_data_seek(resPtr, randNum);
				sqlRow = mysql_fetch_row(resPtr);
				for(j = 0; j < (*fieldNum); j++){
					if(strlen(sqlRow[j]) > interval[j]){
						interval[j] = strlen(sqlRow[j]);
					}
				}
			}
			j = 0;
			for(i = 0; i < (*fieldNum); i++){
				j += interval[i];
			}
			if(j*(*rowNum) > dataLen){
				return OUTPUTSAPACELESS;
			}

			mysql_data_seek(resPtr, 0);
			unsigned char* tmpData = data;
			for(i = 0; i < (*rowNum); i ++){
				sqlRow = mysql_fetch_row(resPtr);
				for (j = 0; j < (*fieldNum); j++){
					memcpy(tmpData,sqlRow[j],strlen(sqlRow[j]));
					tmpData += interval[j];
				}
			}
			free(sqlStrSelect);
			return 0;
		}else{
			free(sqlStrSelect);
			return GETDATAERR;
		}	
}


int transactionDeal(const char* transaction){
	int ret = mysql_query(&mysql,"SET AUTOCOMMIT =0");
	//设置为手动会影响其他sql的提交，慎用
	if(ret != 0){
		return -777;
	}
	ret = mysql_query(&mysql,"BEGIN;");
	ret = mysql_query(&mysql,transaction);
	if(ret != 0){
		printf("%s", mysql_error(&mysql));
		return -7777;
	}
	ret = mysql_query(&mysql,"COMMIT;");
	ret = mysql_query(&mysql,"SET AUTOCOMMIT =1");
	//设置会自动模式
	//printf("%s", mysql_error(&mysql));
	return ret;
}


int getLastErr(unsigned char* err,int errLen){
	//printf("%s", mysql_error(&mysql));
	int msqlErrLen = strlen(mysql_error(&mysql));
	if(errLen < msqlErrLen){
		memcpy(err,mysql_error(&mysql),errLen);
		//虽然存储不够，仍返回部分错误信息
		return OUTPUTSAPACELESS;
	}
	memcpy(err,mysql_error(&mysql),msqlErrLen);
	return 0;
}

/*
CREATE TRIGGER `test_trigger` AFTER INSERT ON `table111`
FOR EACH ROW BEGIN
     insert into tab2(tab2_id) values(new.tab1_id);
END;;
*/

int createTrigger(const char* triggerName,const char* triggerTable,int beforeOrafter,int triggerOper,const char* oper){
	unsigned char afterOrbefore[8] = { 0 };
	unsigned char triOper[8] = { 0 };
	unsigned char sqlTrigger[512] = { 0 };
	if(beforeOrafter == 0){
		memcpy(afterOrbefore,"before",6);
		switch(triggerOper){
			case 0:{
				memcpy(triOper,"insert",7);
			}
			break;
			case 1:{
				memcpy(triOper,"delete",6);
			}
			break;
			case 2:{
				memcpy(triOper,"update",6);
			}
			break;
			default:{
				return TRIGGEROPERERR;
			}
			break;
		}
	}else{
		memcpy(afterOrbefore,"after",6);
		switch(triggerOper){
			case 0:{
				memcpy(triOper,"insert",7);
			}
			break;
			case 1:{
				memcpy(triOper,"delete",6);
			}
			break;
			case 2:{
				memcpy(triOper,"update",6);
			}
			break;
			default:{
				return TRIGGEROPERERR;
			}
			break;
		}
	}
	sprintf(sqlTrigger,"%s%s%s%s%s%s%s%s%s%s%s","CREATE TRIGGER ",triggerName," ",afterOrbefore," ",triOper," ON ",triggerTable," FOR EACH ROW BEGIN ",oper,"END;");
	int ret = mysql_query(&mysql,sqlTrigger);
	if(ret == 0){
		return 0;
	}else{
		//printf("%s\n",mysql_error(&mysql) );
		return CREATETRIGGERERR;
	}
}


int dropTrigger(const char* triggerName){
	unsigned char sqlTrigger[64] = { 0 };
	sprintf(sqlTrigger,"%s%s%s","DROP TRIGGER ",triggerName,";");
	int ret = mysql_query(&mysql,sqlTrigger);
	if(ret == 0){
		return 0;
	}else{
		return DROPTRIGGERERR;
	}
}