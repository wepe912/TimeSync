#ifdef CONTROL_H
#include "./timeInit.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <netdb.h> 





int control_center(void);
int send_command(int command);

#endif CONTROL_H
