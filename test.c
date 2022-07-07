#include <stdio.h>

#define REFLECT_IMPLEMENTATION
#include "reflect.h"

int main() {
  printf("Hello, world!\n");

  ReflectLexer lexer;
  ReflectToken token;
  reflect_lexer_init(&lexer, "1 10 100");

  while (reflect_lexer_token_next(&lexer, &token)) {
    if (token.type == REFLECT_TOKEN_EOF) {
      break;
    }
    printf("%ld\n", token.integer);
  }
}