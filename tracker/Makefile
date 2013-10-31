CC=g++
CPFLAGS= -g -Wall
LDFLAGS= -lcrypto -lpthread


SRC= *.cpp
OBJ=$(SRC:.c=.o)
BIN=../objects

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPFLAGS) $(LDFLAGS) -o $(BIN) $(OBJ)


%.o:%.c
	$(CC) -c $(CPFLAGS) -o $@ $<

$(SRC):

clean:
	rm -rf $(OBJ) $(BIN)
