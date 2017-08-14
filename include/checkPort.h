#ifndef CHECKPORT_H
#define CHECKPORT_H
#include <stdio.h>  
#include <stdlib.h>  
   
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>   

#define PORTERR -0x0004
int checkPort(int port);
#endif