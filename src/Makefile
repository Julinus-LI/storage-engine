DIR=.
SRC=$(wildcard $(DIR)/*.c)
OBJ=$(patsubst %.c,%.o,$(SRC))
CFLAG= -g -Wall -I./ -I../include
LFLAG= -L../lib -lrt -lpthread -lconfig -lzlog -lpcap
BIN=./test

all:$(BIN)

$(DIR)/%.o:$(DIR)/%.c
	gcc -c $< -o $@ $(CFLAG)
$(BIN):$(OBJ)
	gcc -o $@ $^ $(LFLAG)
.PHONY:clean rebuild
clean:
	rm -rf $(BIN) $(OBJ) ../bin/* ../data/*
rebuild: clean all

output:
	cp -rf ./test ../bin/
