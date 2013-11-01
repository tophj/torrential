CC=g++
CPFLAGS=-g -Wall -std=c++0x
LDFLAGS= -lssl -pthread -lcrypto


SRC= networkManager.c bencode.c  bt_client.c bt_lib.c bt_setup.c ../threadpool/threadpool.c ../threadpool/list.c ../tracker/TorrentTrackerCommManager.cpp ../tracker/SystemFunctionWrappers.cpp ../tracker/TcpTorrentTrackerComm.cpp ../tracker/TorrentTrackerComm.cpp ../tracker/UdpTorrentTrackerComm.cpp ../peerwire/peerwire.cpp
OBJ=$(SRC:.c=.o)
BIN=bt_client

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPFLAGS) $(OBJ) $(LDFLAGS) -o $(BIN) 


%.o:%.c
	$(CC) -c $(CPFLAGS) -o $@ $<  

$(SRC):

clean:
	rm -rf *.o bt_client
