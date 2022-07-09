CFLAGS=-std=c99 -Wall -Wextra -Wpedantic -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -fsanitize=address -fsanitize=undefined -fno-sanitize-recover

.PHONY: clean test all

all: main test

main: main.c reflect.h
	@gcc ${CFLAGS} -o main main.c

test: tests/lexer.test
	@./tests/lexer.test

tests/lexer.test: tests/lexer.c reflect.h 
	@gcc ${CFLAGS} -o tests/lexer.test tests/lexer.c

clean:
	rm -rf tests/*.test main
