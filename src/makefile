SRC=$(shell find . -type f -name \*.c)

install: $(SRC)
	gcc  -g -fsanitize=address,undefined $(SRC) -lncurses -I ../inc
