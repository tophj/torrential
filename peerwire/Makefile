CC=g++
CPFLAGS=-g -Wall -std=c++0x
LDFLAGS= -lcrypto -pthread


SRC= tests/peerwire_test.cpp Peerwire.cpp ../tracker/SystemFunctionWrappers.cpp ../threadpool/threadpool.c ../threadpool/list.c
OBJ=$(SRC:.c=.o)
BIN=launchPeerwire

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) $(CPFLAGS) $(LDFLAGS) -o $(BIN) $(OBJ)


%.o:%.c
		$(CC) -c $(CPFLAGS) -o $@ $<

clean:
		rm -rf *.o launchPeerwire
