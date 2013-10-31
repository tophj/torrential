CC=g++
CPFLAGS=-g -Wall -std=c++0x
LDFLAGS= -lcrypto -pthread


SRC= peerwire/peerwire.cpp
OBJ=$(SRC:.c=.o)
BIN=bencoding/launchPeerwire

all: $(BIN)

$(BIN): $(OBJ)
        $(CC) $(CPFLAGS) $(LDFLAGS) -o $(BIN) $(OBJ)


%.o:%.c
        $(CC) -c $(CPFLAGS) -o $@ $<

$(SRC):

clean:
        rm -rf $(OBJ) $(BIN)
