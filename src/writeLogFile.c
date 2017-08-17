#include "../include/writeLogFile.h"


int writeLog(const char* logs){

 	struct tm *t;
 	time_t tt;
 	time(&tt);
 	t = localtime(&tt);
 	char timeNow[32] = { 0 };
 	sprintf(timeNow,"[%4d年%02d月%02d日 %02d:%02d:%02d]:", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
 	FILE* logFileP = NULL;
	if((logFileP = fopen(LOGFILE,"a+")) == NULL){
		return OPENFILEERR;
	} 
	fprintf(logFileP,"%s %s\n",timeNow,logs);
	//fwrite(timeNow,strlen(timeNow),1,logFileP);
	fclose(logFileP);
	return 0;
}