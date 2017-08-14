#include "../include/checkPort.h"

int checkPort(int port){
    struct sockaddr_in sin; 
    bzero(&sin,sizeof(sin));  
    sin.sin_family=AF_INET;  
    sin.sin_addr.s_addr=htonl(INADDR_ANY);  
    sin.sin_port=htons(port);   
    int socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);  
    if(bind(socket_descriptor,(struct sockaddr *)&sin,sizeof(sin)) == NULL){
    	close(socket_descriptor);
    	return PORTERR;
    } else{
    	close(socket_descriptor);   
    	return 0;
    }
		
}