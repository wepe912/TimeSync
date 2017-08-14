#ifndef MYSQLOPER_H
#define MYSQLOPER_H
#include <stdio.h>
#include <stdlib.h>
#include "../include/mysql_include/mysql.h"

#define CONNECTERR  -0x0001



MYSQL mysql;
MYSQL_RES *res;
MYSQL_ROW row;

int initDatabase(const char* host,const char* usr,const char* pwd,unsigned int port,const char* unix_socket,unsigned long clientflag);

void closeConnect();

int createDatabase(const char* databasename);

int changeDatabase(const char* databasename);

int deleteDatabase(const char* databasename);

int createTable(const char* tableName,const char* nameAndType);

int dropTable(const char*tableName);

int addData(const char* tableName,const char*  rowAndValues);
#endif