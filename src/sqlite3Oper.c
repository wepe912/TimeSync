#include "../include/sqlite3Oper.h"


int sqlite3_init(const char* databaseName){
	int ret = 0;
	ret = sqlite3_open(databaseName,&sqlite3Db);
	if(ret != 0){
		return SQLITE_OPENDATABAS_ERR;
	}
	sqlite3_err = (unsigned char*)calloc(512,1);
	if(sqlite3_err == NULL){
		return SQLITE_MALLOCERR;
	}
	return 0;
}

void sqlite3_close_database(void){
	sqlite3_close(sqlite3Db);
	free(sqlite3_err);
}


int sqlite3_create_table(const char* tableName,const char* nameAndType){
	char *sqlStrCreate = (char*)calloc(strlen(tableName) + strlen(nameAndType) + 16,1);
	sprintf(sqlStrCreate, "%s%s%s", "create table ",tableName,nameAndType);

	int callback_create(void *NotUsed, int argc, char **argv, char **azColName){
   		return 0;
	}

	char *zErrMsg = 0;
   	int  ret = 0;


   	ret = sqlite3_exec(sqlite3Db, sqlStrCreate, callback_create, 0, &zErrMsg);
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	free(sqlStrCreate);
      	return SQLITE_CREATE_TABLE_ERR;
   	}
   	free(sqlStrCreate);
	return 0;
}

int sqlite3_drop_table(const char* tableName){
	char sqlStrDrop[64] = { 0 };
	sprintf(sqlStrDrop, "%s%s", "drop table ", tableName);
	int callback_drop(void *NotUsed, int argc, char **argv, char **azColName){
   		return 0;
	}

	char *zErrMsg = 0;
   	int  ret = 0;


   	ret = sqlite3_exec(sqlite3Db, sqlStrDrop, callback_drop, 0, &zErrMsg);
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	return SQLITE_DROP_TABLE_ERR;
   	}
	return 0;
}
int sqlite3_add_data(const char* tableName,const char*  rowAndValues){
	unsigned char* sqlStrInsert = NULL; 
	sqlStrInsert = (unsigned char*)calloc(strlen(rowAndValues) + strlen(tableName) + 16,1);
	if(sqlStrInsert == NULL){
			return SQLITE_MALLOCERR;
	}	
	sprintf(sqlStrInsert,"%s%s%s","insert into ",tableName,rowAndValues);	
	int callback_insert(void *NotUsed, int argc, char **argv, char **azColName){
   		return 0;
	}

	char *zErrMsg = 0;
   	int  ret = 0;


   	ret = sqlite3_exec(sqlite3Db, sqlStrInsert, callback_insert, 0, &zErrMsg);
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	free(sqlStrInsert);
      	return SQLITE_INSERT_TABLE_ERR;
   	}
	free(sqlStrInsert);
	return 0;
}

int sqlite3_delete_data(const char* tableName,const char*  condition){
	unsigned char* sqlStrDelete = NULL; 
	if(condition == NULL){
		sqlStrDelete = (unsigned char*)calloc( strlen(tableName) + 16,1);
		if(sqlStrDelete == NULL){
			return SQLITE_MALLOCERR;
		}
		sprintf(sqlStrDelete,"%s%s","delete from ",tableName);	
	}else{
		sqlStrDelete = (unsigned char*)calloc(strlen(condition) + strlen(tableName) + 16,1);
		if(sqlStrDelete == NULL){
			return SQLITE_MALLOCERR;
		}
		sprintf(sqlStrDelete,"%s%s%s%s","delete from ",tableName," ",condition);
	}


	int callback_delete(void *NotUsed, int argc, char **argv, char **azColName){
   		return 0;
	}

	char *zErrMsg = 0;
   	int  ret = 0;


   	ret = sqlite3_exec(sqlite3Db, sqlStrDelete, callback_delete, 0, &zErrMsg);
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	free(sqlStrDelete);
      	return SQLITE_DELETE_TABLE_ERR;
   	}

	free(sqlStrDelete);
	return 0;
}

int sqlite3_change_data(const char* tableName,const char* rowAndValuesAndCon){
	unsigned char* sqlStrUpdate = NULL; 
	sqlStrUpdate = (unsigned char*)calloc(strlen(rowAndValuesAndCon) + strlen(tableName) + 16,1);
	if(sqlStrUpdate == NULL){
		return SQLITE_MALLOCERR;
	}	
	sprintf(sqlStrUpdate,"%s%s%s%s","update ",tableName," set ",rowAndValuesAndCon);	
	int callback_update(void *NotUsed, int argc, char **argv, char **azColName){
   		return 0;
	}

	char *zErrMsg = 0;
   	int  ret = 0;


   	ret = sqlite3_exec(sqlite3Db, sqlStrUpdate, callback_update, 0, &zErrMsg);
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	free(sqlStrUpdate);
      	return SQLITE_UPDATE_DATA_ERR;
   	}
	free(sqlStrUpdate);
	return 0;
}

//获取数据库中所有表的名称
int sqlite3_get_alltable_name(unsigned char* tableName,int* tableNum){
	char *zErrMsg = 0;
	char** pResult = NULL; 
   	int ret = 0;
   	int fieldNum = 0;
   	ret = sqlite3_get_table(sqlite3Db,"SELECT name FROM sqlite_master WHERE type='table' order by name",&pResult,tableNum,&fieldNum,&zErrMsg); 
   	if( ret != 0 ){
   		//fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	return SQLITE_GET_DATA_ERR;
   	}
   	int i = 0;
	for(i = 0; i < (*tableNum); i ++){
		memcpy(tableName + i*32,pResult[i + fieldNum],strlen(pResult[i + fieldNum]));
	}
	return 0;
}



int sqlite3_get_data(const char* tableName,const char* selectArges,const char* condition,int* rowNum,int* fieldNum, int* interval , unsigned char* data,int dataLen){
	unsigned char* sqlStrSelect = NULL; 
	int sqlLen = 0 ;
	//无where条件
	if(condition == NULL){
		sqlLen =  strlen(tableName) + strlen(selectArges)+ 16;
		sqlStrSelect = (unsigned char*)calloc(sqlLen,sizeof(char));
		if(sqlStrSelect == NULL){
			return SQLITE_MALLOCERR;
		}	
		sprintf(sqlStrSelect,"%s%s%s%s","select ",selectArges," from ", tableName);
	}else{
		//有where条件
		sqlLen = strlen(condition) + strlen(tableName) + strlen(selectArges)+ 16;
		sqlStrSelect = (unsigned char*)calloc(sqlLen,sizeof(char));
		if(sqlStrSelect == NULL){
			return SQLITE_MALLOCERR;
		}
		sprintf(sqlStrSelect,"%s%s%s%s%s","select ",selectArges," from ", tableName,condition);
	}

	/*int callback_select(void *NotUsed, int argc, char **argv, char **azColName){
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

   		return 0;
	}*/

	char *zErrMsg = 0;
	char** pResult = NULL; 
   	int ret = 0;
   	ret = sqlite3_get_table(sqlite3Db,sqlStrSelect,&pResult,rowNum,fieldNum,&zErrMsg); 
   	if( ret != 0 ){
   		///fprintf(stderr, "SQL error: %s\n", zErrMsg);
   		memcpy(sqlite3_err,zErrMsg,strlen(zErrMsg));
      	sqlite3_free(zErrMsg);
      	free(sqlStrSelect);
      	return SQLITE_GET_DATA_ERR;
   	}

   	int simple  = 0;


	if(*rowNum < 30){
		simple = (*rowNum);
	}else{
		simple = (*rowNum) / 3;
	}
	//int simple = (*rowNum) / 3;
	int i = 0,j = 0;
	int randNum = 0;
	int indexField = 0;
	srand((unsigned)time(NULL));
	for(i = 0 ; i < simple;i ++){
		randNum = rand() % (*rowNum);
		int destNum = randNum*(*fieldNum);
		indexField = (*fieldNum);
		for(j = 0; j < (*fieldNum); j++){
			if(strlen(pResult[  destNum + indexField]) > interval[j]){
				interval[j] = strlen(pResult[destNum + indexField]);
				indexField ++;
			}else{
				indexField ++;
			}
		}
	}
	j = 0;
	for(i = 0; i < (*fieldNum); i++){
		j += interval[i];
	}
	if(j*(*rowNum) > dataLen){
		free(sqlStrSelect);
		sqlite3_free_table(pResult);
		return OUTPUT_SAPACE_ERR;
	}

	unsigned char* tmpData = data;

	for(i = 0; i < (*rowNum); i ++){
		indexField = (*fieldNum);
		int destNum = i*(*fieldNum);
		for (j = 0; j < (*fieldNum); j++){
			memcpy(tmpData,pResult[destNum + indexField],strlen(pResult[destNum + indexField]));
			tmpData += interval[j];
			indexField ++;

		}
	}
	free(sqlStrSelect);
	sqlite3_free_table(pResult);
	return 0;	
}


int sqlite3_get_lasterr(unsigned char* err,int errLen){
	int errRealLen = strlen(sqlite3_err);
	if(errRealLen > errLen){
		memcpy(err,sqlite3_err,errLen);
		return OUTPUT_SAPACE_ERR;
	}else{
		memcpy(err,sqlite3_err,errRealLen);
		return 0;
	}
}