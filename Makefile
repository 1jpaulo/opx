CC      			= gcc
CFLAGS 				= -Wall -Wextra -pedantic -std=c99 -Iinclude/
program 			= opx
OBJ     			= obj/main.o obj/file.o obj/console.o obj/unicode.o
UNICODE_TEST_OBJ	= obj/unicode.o obj/file.o tests/unicode/test_unicode.c
build_dir			= mkdir build

build: $(OBJ)
	$(CC) $(CFLAGS) -o build/main $(OBJ)

obj/main.o: src/main.c include/console.h
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/file.o: src/file.c
	$(CC) $(CFLAGS) -c src/file.c -o obj/file.o

obj/console.o: src/console.c
	$(CC) $(CFLAGS) -c src/console.c -o obj/console.o

obj/unicode.o: src/unicode.c
	$(CC) $(CFLAGS) -c src/unicode.c -o obj/unicode.o

.PHONY: clean
clean: 
	rm build/* obj/*