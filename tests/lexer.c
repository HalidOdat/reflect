#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define REFLECT_IMPLEMENTATION
#include "../reflect.h"

static bool silent = true;

void lexer_test(const char* test_name, const char* source, ReflectToken test_cases[]) {
  printf("  Running Test: %s\n", test_name);

  ReflectLexer lexer;
  ReflectToken token;

  reflect_lexer_init(&lexer, source);
  int test_case_number = 1;
  while (test_cases->type != REFLECT_TOKEN_EOF) {
    if (!reflect_lexer_token_next(&lexer, &token)) {
      printf("    Assertion #%d: FAILED - Lex Error: %s\n", test_case_number, reflect_lexer_error_string_get(&lexer));
      return;
    }

    if (test_cases->type != token.type) {
      // TODO: Display type name.
      printf(
        "    Assertion #%d: FAILED - type mismatch - expected: '%s', got '%s'n",
        test_case_number,
        reflect_token_type_to_string(test_cases->type),
        reflect_token_type_to_string(token.type)
      );
      return;
    }

    switch (token.type) {
      case REFLECT_TOKEN_EOF:
        assert(false && "Unreachable: This check if in the loop");
        break;
      case REFLECT_TOKEN_INTEGER:
        if (token.as.integer != test_cases->as.integer){
          printf("    Assertion #%d: FAILED - Expected integer %ld, got %ld\n", test_case_number, test_cases->as.integer, token.as.integer);
        } else if (!silent) {
          printf("    Assertion #%d: PASSED\n", test_case_number);
        }
        break;
      default:
        if (!silent) {
          printf("    Assertion #%d: PASSED\n", test_case_number);
        }
        break;
    }

    test_case_number++;
    test_cases++;
  }

  if (silent) {
    printf("    All Test Assertions Passed!\n");
  }
}

int main(int argc, const char* argv[]) {
  if (argc > 1 && strcmp(argv[1], "--verbose")) {
    silent = false;
  }

  printf("Lexer Tests:\n");
  lexer_test(
    "Simple Integer Lexing",
    "1 2 3 100000",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 1 },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 2 },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 3 },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 100000 },
      { 0 }
    }
  );

  lexer_test(
    "Single Character Punctuator Lexing",
    "[ ] ( ) { } . & * + - ~ ! / % < > ^ | ? : ; = #",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_LBRACKET, },
      { .type = REFLECT_TOKEN_RBRACKET, },
      { .type = REFLECT_TOKEN_LPAREN, },
      { .type = REFLECT_TOKEN_RPAREN, },
      { .type = REFLECT_TOKEN_LBRACE, },
      { .type = REFLECT_TOKEN_RBRACE, },
      { .type = REFLECT_TOKEN_DOT, },
      { .type = REFLECT_TOKEN_AMPERSAND, },
      { .type = REFLECT_TOKEN_STAR, },
      { .type = REFLECT_TOKEN_PLUS, },
      { .type = REFLECT_TOKEN_MINUS, },
      { .type = REFLECT_TOKEN_TILDE, },
      { .type = REFLECT_TOKEN_NOT, },
      { .type = REFLECT_TOKEN_SLASH, },
      { .type = REFLECT_TOKEN_PERCENT, },
      { .type = REFLECT_TOKEN_LESS, },
      { .type = REFLECT_TOKEN_GREATER, },
      { .type = REFLECT_TOKEN_CARET, },
      { .type = REFLECT_TOKEN_PIPE, },
      { .type = REFLECT_TOKEN_QUESTION, },
      { .type = REFLECT_TOKEN_COLON, },
      { .type = REFLECT_TOKEN_SEMICOLON, },
      { .type = REFLECT_TOKEN_ASSIGN, },
      { .type = REFLECT_TOKEN_HASH, },
      { 0 }
    }
  );

  lexer_test(
    "Double Character Punctuator Lexing",
    // "-> ++ -- << >> <= >= == != && || *= /= %= += -= &= ^= |= ##",
    "<= >= == != *= /= %= += -= ^= ##",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_LESS_EQUAL, },
      { .type = REFLECT_TOKEN_GREATER_EQUAL, },
      { .type = REFLECT_TOKEN_EQUALS, },
      { .type = REFLECT_TOKEN_NOT_EQUALS, },
      { .type = REFLECT_TOKEN_MUL_ASSIGN, },
      { .type = REFLECT_TOKEN_DIV_ASSIGN, },
      { .type = REFLECT_TOKEN_MOD_ASSIGN, },
      { .type = REFLECT_TOKEN_ADD_ASSIGN, },
      { .type = REFLECT_TOKEN_SUB_ASSIGN, },
      { .type = REFLECT_TOKEN_XOR_ASSIGN, },
      { .type = REFLECT_TOKEN_HASH_HASH, },
      { 0 }
    }
  );
  return 0;
}