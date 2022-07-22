#include <stdio.h>

#define REFLECT_IMPLEMENTATION
#include "reflect.h"

int main() {
  ReflectLexer lexer;
  ReflectToken token;
  reflect_lexer_init(&lexer, "1u 10ull 100ll\n1l\n  1lu");

  uint32_t count = 1;
  while (true) {
    if (!reflect_lexer_token_next(&lexer, &token)) {
      printf("ERROR: %s\n", reflect_lexer_error_string_get(&lexer));
      continue;
    }

    if (token.type == REFLECT_TOKEN_EOF) {
      break;
    }

    // printf(
    //   "Token (%s) :%u:%u: %ld\n",
    //   reflect_token_type_to_string(token.type),
    //   token.location.line,
    //   token.location.column,
    //   token.as.integer
    // );

    count++;
  }

  return 0;
}