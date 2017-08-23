#include <stdio.h>
//#include "../include/readConfig.h"
//#include "../include/checkPort.h"
#include <stdlib.h>
//#include "../include/mysql_include/mysql.h"
#include "../include/mysqlOper.h"

#include "../include/writeLogFile.h"
//#include "../include/sqlite3.h"
#include "../include/sqlite3Oper.h"
int main(){
	/*********************************
	unsigned char key[24] = { 0 };
	unsigned char value[24] = { 0 };
	int configNum = 0;
	int i = 0;
	int ret = readConfig( key, value, &configNum);
	if(ret == 0){
		printf("configNum = %d \n",configNum );
		for(i = 0; i < configNum; i++){
			printf("%s = %s\n",key + i*KEYOFFSET,value + i*VALUEOFFSET );
		}
	}
	int checkport = checkPort(123);
	printf("%d\n",checkport );
	
	MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    char sql[100];
    *****************************************/
    /*******test mysql***********************
    int ret = initDatabase("192.168.0.31","root","P@ssw0rd",3306,NULL,0);
    printf("ret initDatabase =%d \n",ret);
    ret = createDatabase("test1");
    unsigned char mysqlDberr[128] = { 0 };
    getLastErr(mysqlDberr,128);
    printf("createDatabase err = %s\n",mysqlDberr );
    printf("ret createDatabase=%d \n",ret);
    ret = createTable("table111","(id int, name varchar(32))");
    printf("ret createTable =%d\n",ret);
    //ret = changeDatabase("test1");
    //printf("ret =%d\n",ret);
    //ret = dropTable("table111");
    //printf("ret =%d\n",ret);
    //ret = deleteDatabase("test1");
    //printf("ret =%d\n",ret);

    int i = 0,j = 0;
    int randNum = 0;
    int randChar = 0;
    char nameValue[1024*8] = { 0 };
    sprintf(nameValue,"%s","(id,name)values(1,'www')");
    char* nameValueP = nameValue + strlen(nameValue);
    
    for(i = 0; i < 100; i ++){
        srand((unsigned)time(NULL) + i);
        randNum = rand() % 100;
        randChar = rand() % 32;
        if(randChar == 0){
            randChar = 1;
        }
        char tmp[32] = { 0 };
        for(j = 0;j < randChar; j++){
            tmp[j] = 'w';
        }
        sprintf(nameValueP,"%s%d%s%s%s",",(",randNum,",'",tmp,"')");
        nameValueP += strlen(nameValueP);

    }
    

    //ret = addData("table111","(id,name)values(1,'www'),(2,'qq'),(3,'test'),(4,'tt'),(5,'ewer'),(6,'pp'),(123,'tttttt'),(1111,'ddddd')");
    ret = addData("table111",nameValue);
    printf("ret addData=%d\n",ret);
    ret = deleteData("table111"," where id = 2");
    printf("ret deleteData =%d\n",ret);

    ret = changeData("table111","name = 'testupdate' where id = 3");
    printf("ret changeData =%d\n",ret);


    int rowNum = 0;
    int fieldNum = 0;
    int interval[16] = {0};
    unsigned char data[1024 *8] = {0};
    int len = 1024;
    ret = getData("table111","*",NULL,&rowNum,&fieldNum,interval,data,len*8);
    printf("ret getData =%d\n",ret);

    ret = transactionDeal("CREATE TABLE testTrancation (id int,name char(32));");
    printf("ret transactionDeal = %d\n",ret);
    unsigned char tableName[1024] = { 0 };
    int tableNum = 0;
    ret = getAllTableName(tableName,&tableNum);
    printf("ret getAllTableName = %d\n",ret);
    closeConnect();
    ************test initDatabase****************/
    /***********test writeLog********************
    int ret = 0;
    ret = writeLog("every thing is ok!",0);
    printf("ret writeLog1 =%d\n",ret);
    ret = writeLog("some thing happen!",1);
    printf("ret writeLog2 =%d\n",ret);
    ret = writeLog("bad thing happen!",2);
    printf("ret writeLog3 =%d\n",ret);
    ret = writeLog("other thing happen!",3);
    printf("ret writeLog3 =%d\n",ret);
    ***********test writeLog*********************/

    /*********test sqlite3*************************
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
    }else{
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        fprintf(stderr, "Opened database successfully\n");
    }
    sqlite3_close(db);
    ***************test sqlite3*********************/
    /**************test sqlite3Oper*****************/
    int ret = sqlite3_init("./testdb.sqlite3");
    printf("sqlite3_init =%d\n",ret);
    ret = sqlite3_create_table("table111","(id int, name varchar(32))");
    ret = sqlite3_create_table("tablee222","(id int, name varchar(32))");
    unsigned char sqlite3_err[128] = { 0 };

    printf(" sqlite3_create_table =%d\n",ret);
    sqlite3_get_lasterr(sqlite3_err,128);
    printf("sqlite3_get_lasterr :%s\n",sqlite3_err );
    //ret = sqlite3_drop_table("table111");
    //printf(" sqlite3_drop_table =%d\n",ret);
    ret = sqlite3_add_data("table111","(id,name)values(1,'www'),(2,'qq'),(3,'test'),(4,'tt'),(5,'ewer'),(6,'pp'),(123,'tttttt'),(1111,'ddddd')");
    printf(" sqlite3_add_data =%d\n",ret);
    ret = sqlite3_delete_data("table111","where id = 2");
    printf(" sqlite3_delete_data =%d\n",ret);
    ret = sqlite3_change_data("table111","name = 'testupdate****' where id = 3");
    printf("sqlite3_change_data = %d\n",ret );

    int rowNum = 0;
    int fieldNum = 0;
    int interval[16] = {0};
    unsigned char data[1024 *8] = {0};
    int len = 1024;
    ret = sqlite3_get_data("table111","*",NULL,&rowNum,&fieldNum,interval,data,len*8);
    printf("sqlite3_get_data =%d\n",ret);
    unsigned char tableName[128] = { 0 };
    int tableNum = 0;
    ret = sqlite3_get_alltable_name(tableName,&tableNum);
    printf("sqlite3_get_alltable_name =%d\n",ret);
    sqlite3_close_database();
    /*********************test sqlite3Oper*******************/
    /********************
    unsigned char testStr[128] = { 0 };
    unsigned char *ptest = NULL;
    unsigned char **pptest = NULL;
    unsigned char ***ppptest = NULL;
    memcpy(testStr,"hello world!",13);
    ptest = testStr;
    pptest[0] = &testStr[0];
    pptest[1] = &testStr[1];
    ppptest = testStr;
    printf("pptest[0] = %s\n",pptest[0] );
    printf("pptest[1] = %s\n",pptest[1]);
    //printf("ptest = %c\n",ptest[1] );
    //printf("pptest = %c\n", pptest[1][0]);
    //printf("ppptest = %c\n",ppptest[1][1][1] );
    ***********************/
	return 0;
}