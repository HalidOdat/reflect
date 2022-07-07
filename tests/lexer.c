#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define REFLECT_IMPLEMENTATION
#include "../reflect.h"

void lexer_test(const char* test_name, const char* source, ReflectToken test_cases[]) {
  printf("  Running Test: %s\n", test_name);

  ReflectLexer lexer;
  ReflectToken token;

  reflect_lexer_init(&lexer, source);
  int test_case_number = 1;
  while (test_cases->type != REFLECT_TOKEN_EOF) {
    printf("    Assertion #%d: ", test_case_number);

    if (!reflect_lexer_token_next(&lexer, &token)) {
      printf("FAILED - could not lex\n");
      return;
    }

    if (test_cases->type != token.type) {
      // TODO: Display type name.
      printf("FAILED - type mismatch\n");
      return;
    }

    switch (token.type) {
      case REFLECT_TOKEN_INTEGER:
        printf("PASSED!\n");
        break;
      case REFLECT_TOKEN_EOF:
        assert(false && "Don't test agains EOF - it is checked at the end");
        break;
      default:
        assert(false && "Unreachable: Unknown token type");
    }

    test_case_number++;
    test_cases++;
  }
}

int main() {
  printf("Lexer Tests:\n");
  lexer_test(
    "integer lexing",
    "1 2 3 100000",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_INTEGER, .integer = 1 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 2 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 3 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 100000 },
      { 0 }
    }
  );
  return 0;
}