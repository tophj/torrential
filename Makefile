CC=g++
CPFLAGS=-g -Wall
LDFLAGS= -lcrypto


SRC= peerwire/peerwire.cpp 
OBJ=$(SRC:.c=.o)
BIN=peerwire

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPFLAGS) $(LDFLAGS) -o $(BIN) $(OBJ)


%.o:%.c
	$(CC) -c $(CPFLAGS) -o $@ $<

$(SRC):

clean:
	rm -rf $(OBJ) $(BIN)
