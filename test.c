#include <stdio.h>

#define REFLECT_IMPLEMENTATION
#include "reflect.h"

int main() {
  printf("Hello, world!\n");

  ReflectLexer lexer;
  ReflectToken token;
  reflect_lexer_init(&lexer, "1 10 100");
  reflect_lexer_token_next(&lexer, &token);
  reflect_lexer_token_current(&lexer, &token);
  printf("%d\n", token.integer);

  reflect_lexer_token_next(&lexer, &token);
  reflect_lexer_token_current(&lexer, &token);
  printf("%d\n", token.integer);

  reflect_lexer_token_next(&lexer, &token);
  reflect_lexer_token_current(&lexer, &token);
  printf("%d\n", token.integer);
}