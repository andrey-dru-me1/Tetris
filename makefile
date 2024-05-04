CC=gcc
CFLAGS=-Werror -Wall -std=c11 -O2
LDFLAGS=-O2
DEBUGFLAGS=-g -fsanitize=address,undefined

SRC=$(shell find src -type f -name \*.c)
INC=inc

BUILD_DIR=build

OBJ_DIR=$(BUILD_DIR)/obj
OBJ_SUBDIRS=$(patsubst src/%, $(OBJ_DIR)/%, $(shell find src/* -type d))
OBJ=$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

TARGET=$(BUILD_DIR)/tetris

all: $(TARGET)

clean:
	rm -rf $(BUILD_DIR) *.out

rebuild: clean all

$(BUILD_DIR)/debug: $(OBJ)
	$(CC) $(LDFLAGS) $(DEBUGFLAGS) $^ -o $@

$(OBJ): OBJECT=$(@:$(OBJ_DIR)/%.o=src/%.c)
$(OBJ): $(OBJ_SUBDIRS)
	$(CC) -c $(CFLAGS) -I $(INC) $(OBJECT) -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_SUBDIRS):
	mkdir -p $@