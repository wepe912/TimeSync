#include <stdio.h>
//#include "../include/readConfig.h"
//#include "../include/checkPort.h"
#include <stdlib.h>
#include <pthread.h>
//#include "../include/mysql_include/mysql.h"
#include "../include/mysqlOper.h"

#include "../include/writeLogFile.h"
//#include "../include/sqlite3.h"
#include "../include/sqlite3Oper.h"

#include "../include/timeInit.h"
#include "../include/nowTime.h"
#include "../include/w_tsync.h"
#include <time.h>
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
    //ret = createDatabase("test2");
    //unsigned char mysqlDberr[128] = { 0 };
    //getLastErr(mysqlDberr,128);
    //printf("createDatabase err = %s\n",mysqlDberr );
    //printf("ret createDatabase=%d \n",ret);
    changeDatabase("test2");
    ret = createTable("table111","(id int, name varchar(32))");
    printf("ret createTable =%d\n",ret);
    //ret = changeDatabase("test1");
    //printf("ret =%d\n",ret);
    //ret = dropTable("table111");
    //printf("ret =%d\n",ret);
    //ret = deleteDatabase("test1");
    //printf("ret =%d\n",ret);
    /*************add_test***********************
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
    

    //
    ret = addData("table111",nameValue);

    printf("ret addData=%d\n",ret);
    /********************************************************
    ret = addData("table111","(id,name)values(1,'www'),(2,'qq'),(3,'test'),(4,'tt'),(5,'ewer'),(6,'pp'),(123,'tttttt'),(1111,'ddddd')");

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
    //ret = transactionDeal("CREATE TABLE testTrancation (id int,name char(32));");
    //printf("ret transactionDeal = %d\n",ret);
    unsigned char tableName[1024] = { 0 };
    int tableNum = 0;

    ret = getAllTableName(tableName,&tableNum);
    printf("ret getAllTableName = %d\n",ret);
    //测试trigger的影响

    printf("test 10000 insert no triger\n");



    struct timeval start;
    struct timeval end;
    unsigned long timer;
  
    int i = 0;
    gettimeofday(&start, NULL);
    while(i < 1){
            ret = addData("table111","(id,name)values(8,'ww')");
            i ++ ;
    }
   
 
    gettimeofday(&end, NULL);

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("timer = %ld us\n", timer);
    //int createTrigger(const char* triggerName,const char* triggerTable,int beforeOrafter,int triggerOper,const char* oper)
    ret = createTrigger("testTrigger1","table111",0,0,"update Counts set Counts=Counts+1 where id =1;");
    ret = createTrigger("testTrigger2","table111",1,0,"update Counts set Counts=Counts+1 where id =1;");
    ret = createTrigger("testTrigger3","table111",1,1,"update Counts set Counts=Counts-1 where id =1;");
    ret = createTrigger("testTrigger4","table111",1,2,"update Counts set Counts=Counts where id =1;");
    printf("createTrigger = %d \n", ret);
    ret = dropTrigger("testTrigger1");
    printf("dropTrigger = %d \n", ret);
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
    /**************test sqlite3Oper*****************
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
    *********************test sqlite3Oper*******************/
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
    /************createDatabase 2 test*************************
    int ret = initDatabase("192.168.0.31","root","P@ssw0rd",3306,NULL,0);
    ret = createDatabase("test2111",NULL);
    ************createDatabase 2 test*************************/
    /*********************test timeSync***************************


    int  ret = initTimeSync(DBTYP_SQLIGHT3,"testdb.sqlite3",NULL,NULL,NULL,0);

    P_sqlite3_addData = P_addData;
    P_sqlite3_deleteData = P_deleteData;
    P_sqlite3_changeData = P_changeData;
    P_sqlite3_getData = P_getData;
    P_sqlite3_close = P_close;
    P_sqlite3_getLastErr = P_getLastErr;
    ret = initTimeSync(DBTYP_MYSQL,"testdb_mysql12","192.168.0.59","root","123456",3306);
        printf("ret === %d\n", ret);
    if(ret == 0){
        accident_database = 0;
        pthread_t accidentDealThread;
        third_db.db_type = DBTYP_MYSQL;
        third_db.db_port = 3306;
        memcpy(third_db.db_name,"testdb_mysql12",16);
        memcpy(third_db.db_host,"192.168.0.59",16);
        memcpy(third_db.db_usr,"root",5);
        memcpy(third_db.db_passwd,"123456",7);
        ret = pthread_create(&accidentDealThread,NULL,(void *)accidentDeal,NULL);
        ret = pthread_detach(accidentDealThread);
        //start accidentDeal
    }else{
        return -1;
    }
    //ret = createTrigger("testTrigger4","table111",1,2,"update Counts set Counts=Counts where id =1;");

   
    P_BW_list = (BW_list*)calloc(3,sizeof(BW_list));
    memcpy(P_BW_list[0].IP,"192.168.111.111",16);
    P_BW_list[0].allowed_ntp = '0';
    P_BW_list[0].allowed_ntps = '1';
    memcpy(P_BW_list[1].IP,"192.168.111.222",15);
    P_BW_list[1].allowed_ntp = 'a';
    P_BW_list[1].allowed_ntps = 'b';
    printf("P_BW_list =%s\n",P_BW_list[1].IP);
    free(P_BW_list);
    int rowNum =0;
    int fieldNum = 0;
    int interval[8] = { 0 };
    unsigned char data[12] = { 0 };
    int i = 0;
    int errNum = 0;
    unsigned char sqlerr[128] = { 0 };
    unsigned char time1[32] = { 0 };
    getNowTime(time1,32);
    printf("time = %s\n", time1);
    while(1){
        //ret = P_getData("config","Value","where Name='tsync_black_white_list_on'",&rowNum,&fieldNum,interval,data,12);
        ret = P_addData("client_record","(IP,Type,Time)values('192.168.0.1','NTP','2017-09-12 11:43:12')");
        if(ret == 0){
           // printf(" tsync_black_white_list_on = %c\n",data[0] );
            printf("add data ok ！\n");
        }else {
            errNum = P_getLastErr(sqlerr,128);
            printf("errNum = %d\n",errNum );
            if(errNum == 2013){
                printf("database err :errNum=%d,detail:%s\n",errNum,sqlerr );
                printf("changeDatabase sqlite3\n");
                P_getData = P_sqlite3_getData;
                P_addData = P_sqlite3_addData;
                accident_database = 1;
            }
        }
        sleep(1);
        
    }

    //ret = initTimeSync(DBTYP_MYSQL,"testdb_mysql12","192.168.0.31","root","P@ssw0rd",3306);
    //int ret = initTimeSync(DBTYP_MYSQL,"testdb_mysql12","192.168.0.59","root","123456",3306);
    //ret = P_addData("counts_table","(TableName,Counts)values('client_record_counts111',0);");
    *********************test timeSync***************************/
    #define JAN_1970 2208988800UL       /* Unix base epoch */
    typedef struct {
        unsigned int Ul_i;
        unsigned int l_uf;
    } l_fp;

    struct timespec ts,ts1;
    int i = 0;
    do{
        int ret = clock_gettime(CLOCK_REALTIME, &ts);
        if(ts.tv_nsec > 1000000000){
             printf("ts.tv_sec = %ld,*** ts.tv_nsec= %ld\n", ts.tv_sec,ts.tv_nsec);
        }
        i ++ ;
    }while(i < 1000);
    printf("ts.tv_sec = %lu,*** ts.tv_nsec= %lu\n", ts.tv_sec,ts.tv_nsec);
    unsigned int sec = (unsigned int)ts.tv_sec;
    unsigned int n_sec = (unsigned int)ts.tv_nsec;
    long double  test = n_sec / 1000000000;
    printf("test = %lf\n",test );
    printf("%d -- %d\n",sec,n_sec );
    printf("sizeof(double) = %d\n", sizeof(double));



    int ret = messure_sys_precision();
    printf("sys_precision = %d\n",(int)sys_precision );
    printf("sizeof long = %d\n",sizeof(long) );
    printf("sizeof time_t = %d \n",sizeof(time_t));
    ts1 = normalize_tspec(ts);
    printf("ts1.tv_sec = %lu,*** ts1.tv_nsec= %lu\n", ts1.tv_sec,ts1.tv_nsec);
	return 0;
}