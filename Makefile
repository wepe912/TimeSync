DIR_INC = ./include
DIR_BIN = ./bin
DIR_OBJ = ./obj
DIR_SRC = ./src
DIR_LIB = ./lib
SRC = $(wildcard ${DIR_SRC}/*.c)
OBJ = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TESTMAINOBJ = $(DIR_OBJ)/main.o
WTIMESSOBJ = $(DIR_OBJ)/wtimess.o

TRGS = testMain
TRGS_TS = W_TimeSS
LINKLIBMYSQL = mysqlclient
LINKLIBSQLITE3 = sqlite3

BIN_TARGET_S = ${DIR_BIN}/${TRGS}
BIN_TARGET_W = ${DIR_BIN}/${TRGS_TS}


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
LINKFLAGSMYSQL = -l${LINKLIBMYSQL}  -L./lib/msyql_lib/ 
LINKSQLITE3FLAGS = -l${LINKLIBSQLITE3} -L./lib/sqlite3/ -ldl -pthread



ALLTARGET:$(BIN_TARGET_S) $(BIN_TARGET_W)
$(BIN_TARGET_S):$(filter-out $(WTIMESSOBJ),$(OBJ))
	$(CC) $(CFLAGS) $(filter-out $(WTIMESSOBJ),$(OBJ)) -o $@ $(LINKFLAGSMYSQL) $(LINKSQLITE3FLAGS)
$(BIN_TARGET_W):$(filter-out $(TESTMAINOBJ),$(OBJ))
	$(CC) $(CFLAGS) $(filter-out $(TESTMAINOBJ),$(OBJ)) -o $@ $(LINKFLAGSMYSQL) $(LINKSQLITE3FLAGS)
$(OBJ):$(DIR_OBJ)/%.o : $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -fPIC -c $^ -o $@ 
clean:
	rm -rf $(DIR_BIN)/$(TRGS)
	rm -rf $(DIR_OBJ)/*
	#rm -rf $(DIR_LIB)/
	