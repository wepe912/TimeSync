#ifndef	WRITELOGFILE_H
#define	WRITELOGFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	 LOGFILE	"./Mylog.log"

#define OPENFILEERR	-0x0002
#define	LOGTYPEERR	-0x0003

#define	WRITELOG_SUCCESS		0x0000
#define	WRITELOG_WARNING		0x0001
#define	WRITELOG_ERROR		0x0002
#define	WRITELOG_OTHERS		0x0003

int writeLog(const char* logs,int logType);
#endif