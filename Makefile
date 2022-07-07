
.PHONY: clean test all

all: test

test: tests/lexer.test
	@./tests/lexer.test

tests/lexer.test: tests/lexer.c reflect.h 
	@gcc -o tests/lexer.test tests/lexer.c

clean:
	rm -rf tests/*.test