#include "../include/nowTime.h"

int getNowTime(unsigned char* nowTime,int len){
	struct tm *t;
 	time_t tt;
 	time(&tt);
 	t = localtime(&tt);
 	if(len < 20){
 		return -1;
 	}
 	sprintf(nowTime,"%4d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
}