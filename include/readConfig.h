#ifndef READCONFIG_H
#define READCONFIG_H
#include <stdlib.h>
#include <stdio.h>

#define CONTENTlEN		0x0080
#define KEYOFFSET		0x0020
#define VALUEOFFSET		0x0040


#define OPENFILEERR		-0x0002

int readConfig(unsigned char* key,unsigned char* value,int* configNum);



#endif