SRC=$(shell find . -type f -name \*.c)

install: $(SRC)
	gcc -O0 -g -fsanitize=address,undefined $(SRC) -lncurses -I ../inc

profile: $(SRC)
	gcc -O0 -fprofile-arcs -pg $(SRC) -lncurses -I ../inc -o prof
