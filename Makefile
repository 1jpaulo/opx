CC      = gcc
CFLAGS 	= -Wall -Wextra -pedantic -std=c99 -Iinclude/
program = exp

OBJ     = obj/main.o obj/file.o obj/console.o

build: $(OBJ)
	$(CC) $(CFLAGS) -o bin/main $(OBJ)

obj/main.o: src/main.c include/console.h
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/file.o: src/file.c
	$(CC) $(CFLAGS) -c src/file.c -o obj/file.o

obj/console.o: src/console.c
	$(CC) $(CFLAGS) -c src/console.c -o obj/console.o

.PHONY: clean

clean: 
	rm bin/* obj/*