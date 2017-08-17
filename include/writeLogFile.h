#ifndef	WRITELOGFILE_H
#define	WRITELOGFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	 LOGFILE	"./Mylog.log"

#define OPENFILEERR	-0x0002

int writeLog(const char* logs);
#endif