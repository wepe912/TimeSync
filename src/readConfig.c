#include "../include/readConfig.h"

int readConfig(unsigned char* key,unsigned char* value,int* configNum){
	FILE* configFp = NULL;
	unsigned char contentLine[CONTENTlEN];
	int keyOffset = 0;
	int valueOffset = 0;

	*configNum =0;
	if((configFp = fopen("./tsync.conf","r")) == NULL){
		return OPENFILEERR;
	} 
	while((fgets(contentLine,CONTENTlEN,configFp)) != NULL){
		int conLen = strlen(contentLine);
		if('A' <= contentLine[0]&& contentLine[0] <= 'z'){
			unsigned char* keytmp = &contentLine[1];
			int keyLen = 1;
			while((*keytmp) != '='){
				keytmp ++;
				keyLen ++;
			}
			memset(key + keyOffset,0,keyLen + 1 );	
			memcpy(key + keyOffset,contentLine,keyLen);
			//过滤特殊字符
			while(contentLine[conLen -1] < 0x2F){
				conLen --;
			}
			
			memset(value + valueOffset,0,conLen -  keyLen) ;
			memcpy(value + valueOffset,contentLine + keyLen + 1,conLen - 1 - keyLen );
			
			
			keyOffset += KEYOFFSET;
			valueOffset += VALUEOFFSET;
			(*configNum) ++;
		}
	}
	fclose(configFp);
	return 0;
}