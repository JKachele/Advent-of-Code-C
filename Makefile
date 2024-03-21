######################################################################
# @author      : Justin Kachele (justin@kachele.com)
# @file        : Makefile
# @created     : Monday Feb 26, 2024 13:43:29 EST
######################################################################

CC = gcc
CFLAGS = -g
LDFLAGS = -g

SRC = src/2023/Day$(DAY).c $(wildcard src/util/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all build dirs clean run runTest

all: dirs out release

build: dirs out run

testBuild: dirs test

buildRelease: dirs release

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/out

out: $(OBJ)
	$(CC) -o $(BIN)/out $^ $(LDFLAGS)

release: $(OBJ)
	$(CC) -o $(BIN)/release $^ -s

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: dirs
	$(CC) -o $(BIN)/test test.c $(LDFLAGS)

runTest: test
	$(BIN)/test

clean:
	rm -rf bin/

