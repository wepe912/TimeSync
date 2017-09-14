#ifndef W_TSYNC_H
#define	W_TSYNC_H
#include "./timeInit.h"




void* tsync_start(void* arg);
//void* tsync(void* arg);
void* agreement_key(void* arg);

int tsync_client(int tsync_type,const char* server_addr);


#endif