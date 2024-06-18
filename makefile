CC=gcc
CFLAGS=-Werror -Wall -std=gnu11 -O3
LDFLAGS=-O3
DEBUGFLAGS=-g -fsanitize=address,undefined

SRC_DIR=src

BACK_SRC=$(shell find src/brick_game -type f -name \*.c)
FRONT_SRC=$(shell find src/gui -type f -name \*.c)
TEST_SRC=$(shell find src/test -type f -name \*.c)
INC=inc

BUILD_DIR=build

OBJ_DIR:=$(BUILD_DIR)/obj

BACK_OBJ:=$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(BACK_SRC))
FRONT_OBJ:=$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(FRONT_SRC))
TEST_OBJ:=$(patsubst src/%.c, $(OBJ_DIR)/%.o, $(TEST_SRC))

TARGET:=$(BUILD_DIR)/tetris

all: $(TARGET)

clean:
	rm -rf $(BUILD_DIR) *.out

rebuild: clean all

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -I $(INC) $< -o $@

$(TARGET): $(BACK_OBJ) $(FRONT_OBJ)
	$(CC) $(LDFLAGS) $^ -lncurses -o $@

test: $(BUILD_DIR)/test

$(BUILD_DIR)/test: $(BACK_OBJ) $(TEST_OBJ)
	$(CC) $(LDFLAGS) $^ -lcheck -o $@

GCOV_DIR:=$(BUILD_DIR)/gcov
GCOV_OBJ_DIR:=$(GCOV_DIR)/obj
GCOV_REPORT_DIR:=$(GCOV_DIR)/report

GCOV_OBJ:=$(patsubst src/%.c, $(GCOV_OBJ_DIR)/%.o, $(BACK_SRC) $(TEST_SRC))
GCOV_EXEC:=$(GCOV_DIR)/gcov_test

gcov_report: $(GCOV_DIR)/gcov.info
	genhtml -o $(GCOV_REPORT_DIR) $<

$(GCOV_DIR)/gcov.info: $(GCOV_EXEC)
	./$<
	lcov -c -d $(GCOV_DIR) -o $@

$(GCOV_EXEC): $(GCOV_OBJ)
	$(CC) -O0 --coverage $^ -lcheck -o $@

$(GCOV_OBJ_DIR)/test/%.o: $(SRC_DIR)/test/%.c
	mkdir -p $(@D)
	$(CC) -c -O3 -I $(INC) $< -o $@

$(GCOV_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) -c -O0 --coverage -I $(INC) $< -o $@
