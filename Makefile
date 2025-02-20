######################################################################
# @author      : Justin Kachele (justin@kachele.com)
# @file        : Makefile
# @created     : Monday Feb 26, 2024 13:43:29 EST
######################################################################

CC = gcc
CFLAGS = -std=c11 -g
CFLAGS += -Wall -Wextra -Wpedantic 
CFLAGS += -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -Wno-stringop-truncation
LDFLAGS = -lm 

SRC = src/2024/Day$(DAY).c $(wildcard src/util/*.c) $(wildcard src/lib/*.c)
TEST = test.c $(wildcard src/util/*.c) $(wildcard src/lib/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all build dirs clean run runTest

all: dirs out

build: dirs out

testBuild: dirs test

rel:
	make -f Makefile.rel

relrun:
	make -f Makefile.rel run

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/out
	rm $(OBJ)

runWithTest: all
	$(BIN)/out TEST
	rm $(OBJ)

out: $(OBJ)
	$(CC) -o $(BIN)/out $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: dirs
	$(CC) -o $(BIN)/test $(TEST) $(LDFLAGS)

runTest: test
	$(BIN)/test

clean:
	rm -rf bin/
	rm $(OBJ)

