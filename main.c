#include <stdio.h>

#define REFLECT_IMPLEMENTATION
#include "reflect.h"

int main() {
  printf("Hello, world!\n");

  ReflectLexer lexer;
  ReflectToken token;
  reflect_lexer_init(&lexer, "1 10 100");


  while (true) {
    printf("%s\n", lexer.source);
    printf("%s\n", lexer.stream);
    if (!reflect_lexer_token_next(&lexer, &token)) {
      printf("ERROR: %s\n", reflect_lexer_error_string_get(&lexer));
      break;
    }

    if (token.type == REFLECT_TOKEN_EOF) {
      break;
    }
    printf("%ld\n", token.integer);
  }
}