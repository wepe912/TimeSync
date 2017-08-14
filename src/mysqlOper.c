#include "../include/mysqlOper.h"

/*初始化mysql数据库,先连接默认的mysql库*/
int initDatabase(const char* host,const char* usr,const char* pwd,unsigned int port,const char* unix_socket,unsigned long clientflag){


	mysql_library_init(0, NULL, NULL);
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,host,usr,pwd,"mysql",port,unix_socket,clientflag)){
    	printf("无法连接到数据库，错误原因：%s\n", mysql_error(&mysql));
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
		changeDatabase(databaseName);
		return ret;
	}else{
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
	if(strlen(rowAndValues) < 10){
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


