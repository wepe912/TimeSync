#include <stdio.h>
//#include "../include/readConfig.h"
//#include "../include/checkPort.h"
#include <stdlib.h>
//#include "../include/mysql_include/mysql.h"
#include "../include/mysqlOper.h"

int main(){
	/*
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
    */
    /*
    mysql_library_init(0, NULL, NULL);
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql,"192.168.0.31","root","P@ssw0rd","test",0,NULL,0)){
        printf("无法连接到数据库，错误原因：%s\n", mysql_error(&mysql));
    }

    else {
        printf("连接数据库成功\n");
        sprintf(sql, "insert into students(name) values('eric')");
        mysql_query(&mysql, sql);
        sprintf(sql, "select name from students");
        int t = mysql_query(&mysql, sql);
        if (0 != t) {
            printf("查询数据库失败%s\n", mysql_error(&mysql));
        }
        else {
            res = mysql_store_result(&mysql);
            while (row = mysql_fetch_row(res)) {
                int r = 0;
                for ( r = 0;r < mysql_num_fields(res); r++) {
                    printf("%s\n", row[r]);
                }
            }
        }

    }*/
    int ret = initDatabase("192.168.0.31","root","P@ssw0rd",3306,NULL,0);
    printf("ret initDatabase =%d \n",ret);
    ret = createDatabase("test1");
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
        srand((unsigned)time(NULL));
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
    closeConnect();
    

	return 0;
}