SRC=$(shell find . -type f -name \*.c)

install: $(SRC)
	gcc -O0 -g -fsanitize=address,undefined $(SRC) -lncurses -I ../inc
