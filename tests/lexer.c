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
      printf("    Assertion #%d: FAILED - could not lexn", test_case_number);
      return;
    }

    if (test_cases->type != token.type) {
      // TODO: Display type name.
      printf("    Assertion #%d: FAILED - type mismatchn", test_case_number);
      return;
    }

    switch (token.type) {
      case REFLECT_TOKEN_INTEGER:
        if (token.integer != test_cases->integer){
          printf("    Assertion #%d: FAILED - Expected integer %ld, got %ldn", test_case_number, test_cases->integer, token.integer);
        } else if (!silent) {
          printf("    Assertion #%d: PASSED!n", test_case_number);
        }
        break;
      case REFLECT_TOKEN_LBRACKET:
      case REFLECT_TOKEN_RBRACKET:
      case REFLECT_TOKEN_LPAREN:
      case REFLECT_TOKEN_RPAREN:
      case REFLECT_TOKEN_LBRACE:
      case REFLECT_TOKEN_RBRACE:
      case REFLECT_TOKEN_DOT:
      case REFLECT_TOKEN_AMPERSAND:
      case REFLECT_TOKEN_STAR:
      case REFLECT_TOKEN_PLUS:
      case REFLECT_TOKEN_MINUS:
      case REFLECT_TOKEN_TILDE:
      case REFLECT_TOKEN_EXCLAMATION:
      case REFLECT_TOKEN_SLASH:
      case REFLECT_TOKEN_PERCENT:
      case REFLECT_TOKEN_LESS:
      case REFLECT_TOKEN_GREATER:
      case REFLECT_TOKEN_CARET:
      case REFLECT_TOKEN_PIPE:
      case REFLECT_TOKEN_QUESTION:
      case REFLECT_TOKEN_COLON:
      case REFLECT_TOKEN_SEMICOLON:
      case REFLECT_TOKEN_EQUAL:
      case REFLECT_TOKEN_HASH:
        if (!silent) {
          printf("    Assertion #%d: PASSED\n", test_case_number);
        }
        break;
      case REFLECT_TOKEN_EOF:
        assert(false && "Unreachable: This check if in the loop");
        break;
      default:
        assert(false && "Unreachable: Unknown token type");
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
      { .type = REFLECT_TOKEN_INTEGER, .integer = 1 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 2 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 3 },
      { .type = REFLECT_TOKEN_INTEGER, .integer = 100000 },
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
      { .type = REFLECT_TOKEN_EXCLAMATION, },
      { .type = REFLECT_TOKEN_SLASH, },
      { .type = REFLECT_TOKEN_PERCENT, },
      { .type = REFLECT_TOKEN_LESS, },
      { .type = REFLECT_TOKEN_GREATER, },
      { .type = REFLECT_TOKEN_CARET, },
      { .type = REFLECT_TOKEN_PIPE, },
      { .type = REFLECT_TOKEN_QUESTION, },
      { .type = REFLECT_TOKEN_COLON, },
      { .type = REFLECT_TOKEN_SEMICOLON, },
      { .type = REFLECT_TOKEN_EQUAL, },
      { .type = REFLECT_TOKEN_HASH, },
      { 0 }
    }
  );
  return 0;
}