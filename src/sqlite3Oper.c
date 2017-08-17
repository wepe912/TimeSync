#include "../include/sqlite3Oper.h"


int sqlite3_init(const char* databaseName){
	int ret = 0;
	ret = sqlite3_open(databaseName,&sqlite3Db);
	if(ret){
		return SQLITE_OPENDATABAS_ERR;
	}
	return 0;
}

void sqlite3_close_database(void){
	sqlite3_close(sqlite3Db);
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
      	sqlite3_free(zErrMsg);
      	free(sqlStrDelete);
      	return SQLITE_DELETE_TABLE_ERR;
   	}

	free(sqlStrDelete);
	return 0;
}