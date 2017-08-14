DIR_INC = ./include
DIR_BIN = ./bin
DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_LIB = ./lib
SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TRGS = testMain
LINKLIB = mysqlclient


BIN_TARGET_S = ${DIR_BIN}/${TRGS}


ver=_DEBUG
ifeq ($(ver), _DEBUG)
CFLAGS=-g -D$(ver) -Wall -I${DIR_INC} 
LIBFLAGS =  -shared -D$(ver)
else
CFLAGS=-Wall -I${DIR_INC}
LIBFLAGS =  -shared
endif

CLP=PC
ifeq ($(CLP),PC)
CC=gcc
else
CC=arm-xilinx-linux-gnueabi-gcc
endif
#CC=arm-xilinx-linux-gnueabi-gcc
#CFLAGS=-g -Wall -I${DIR_INC}
#LIBFLAGS =  -shared
LINKFLAGS = -l${LINKLIB} -L./lib/msyql_lib/




ALLTARGET:$(BIN_TARGET_S) 
$(BIN_TARGET_S):$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LINKFLAGS)

$(OBJ):$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@ 
clean:
	rm -rf $(DIR_BIN)/$(TRGS)
	rm -rf $(DIR_OBJ)/*
	#rm -rf $(DIR_LIB)/
	