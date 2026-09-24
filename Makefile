######################################################################
# @author      : Justin Kachele (justin@kachele.com)
# @file        : Makefile
# @created     : Monday Feb 26, 2024 13:43:29 EST
######################################################################

CC = gcc
CFLAGS = -std=c2x -g
CFLAGS += -Wall -Wextra -Wpedantic 
CFLAGS += -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -Wno-stringop-truncation
LDFLAGS = -lm # -lz3 # Uncomment for 2025 Day 10
AR = ar
ARFLAGS = rcs

BIN := bin
BINLIB := bin/lib
BUILD_DIR := build

SRC  := $(FILE)
OBJ  := $(SRC:%=$(BUILD_DIR)/%.o)

TESTSRC := src/test.c
TESTOBJ := $(TESTSRC:%=$(BUILD_DIR)/%.o)

LIBSRC := $(wildcard src/util/*.c)# $(wildcard src/util/*/*.c)
LIBSRC += $(wildcard src/lib/*.c)
LIBOBJ := $(LIBSRC:%=$(BUILD_DIR)/%.o)

OUT := $(BIN)/out
UTILS := $(BINLIB)/libutils.a
TEST := $(BIN)/test

.PHONY: build clean run runTest

build: $(UTILS) $(OUT)

lib: $(UTILS)

testBuild: dirs test

rel:
	make -f Makefile.rel

relrun:
	make -f Makefile.rel run

run: $(OUT)
	stdbuf -oL $(OUT) | tee $(BIN)/output.log

runWithTest: $(OUT)
	stdbuf -oL $(OUT) TEST | tee $(BIN)/output.log

runTest: $(TEST)
	stdbuf -oL $(TEST)

$(OUT): $(OBJ) $(UTILS)
	@mkdir -p $(dir $@)
	$(CC) -o $(OUT) $(OBJ) $(UTILS) $(LDFLAGS)

$(TEST): $(TESTOBJ) $(UTILS)
	@mkdir -p $(dir $@)
	$(CC) -o $(TEST) $(TESTOBJ) $(UTILS) $(LDFLAGS)

$(UTILS): $(LIBOBJ)
	@mkdir -p $(dir $@)
	$(AR) $(ARFLAGS) $(UTILS) $^

$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf bin/

