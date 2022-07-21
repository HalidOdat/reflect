// TODO: Handle negative test cases

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
        "    Assertion #%d: FAILED - type mismatch - expected: '%s', got '%s'\n",
        test_case_number,
        reflect_token_type_to_string(test_cases->type),
        reflect_token_type_to_string(token.type)
      );
      return;
    }

    bool passed = false;
    switch (token.type) {
      case REFLECT_TOKEN_EOF:
        assert(false && "Unreachable: This check is in the loop");
        break;
      case REFLECT_TOKEN_IDENTIFIER:
        if (strcmp(token.as.identifier, test_cases->as.identifier) != 0) {
          printf("    Assertion #%d: FAILED - Expected identifier \"%s\", got \"%s\"\n", test_case_number, test_cases->as.identifier, token.as.identifier);
        } else {
          passed = true;
        }
        break;
      case REFLECT_TOKEN_INTEGER:
        if (token.as.integer != test_cases->as.integer){
          printf("    Assertion #%d: FAILED - Expected integer %ld, got %ld\n", test_case_number, test_cases->as.integer, token.as.integer);
        } else if (token.modifier != test_cases->modifier) {
          printf("    Assertion #%d: FAILED - Modifier mismatch\n", test_case_number);
        } else {
          passed = true;
        }
        break;
      default:
        passed = true;
        break;
    }

    if (passed && !silent) {
      printf("    Assertion #%d: PASSED\n", test_case_number);
    }

    test_case_number++;
    test_cases++;
  }

  if (!silent) {
    printf("    All Test Assertions Passed!\n");
  }
}

void lexer_integer_tests();
void lexer_punctuator_tests();
void lexer_identifier_tests();

int main(int argc, const char* argv[]) {
  if (argc > 1 && strcmp(argv[1], "--verbose")) {
    silent = false;
  }

  printf("Lexer Tests:\n");
  lexer_integer_tests();
  lexer_punctuator_tests();
  lexer_identifier_tests();

  return 0;
}

void lexer_identifier_tests() {
  printf(" Identifier Tests:\n");
  lexer_test(
    "Simple Identifier Lexing",
    "x y z",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "x" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "y" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "z" },
      { 0 }
    }
  );

  lexer_test(
    "Alphanumerical Identifier Lexing",
    "get2 test1234020 a0b1c2d3",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "get2" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "test1234020" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "a0b1c2d3" },
      { 0 }
    }
  );

  lexer_test(
    "Underscore Identifier Lexing",
    "_ __attribute__ __linux__ _1_ _HELLO____1223___",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "_" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "__attribute__" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "__linux__" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "_1_" },
      { .type = REFLECT_TOKEN_IDENTIFIER, .as.identifier = "_HELLO____1223___" },
      { 0 }
    }
  );
}

void lexer_integer_tests() {
  printf(" Integer Tests:\n");
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
    "Simple Hexadecimal Integer Lexing",
    "0x0 0x1 0x100000 0xabcdef 0xABCDEF 0x0aB6ceFe 0X0aB6ceFe",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0x0,        .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0x1,        .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0x100000,   .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0xabcdef,   .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0xABCDEF,   .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0x0aB6ceFe, .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 0X0aB6ceFe, .modifier = REFLECT_MODIFIER_HEXADECIMAL },
      { 0 }
    }
  );

  lexer_test(
    "Simple Octal Integer Lexing",
    "00 01 01234567 000333300",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 00,        .modifier = REFLECT_MODIFIER_OCTAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 01,        .modifier = REFLECT_MODIFIER_OCTAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 01234567,  .modifier = REFLECT_MODIFIER_OCTAL },
      { .type = REFLECT_TOKEN_INTEGER, .as.integer = 000333300, .modifier = REFLECT_MODIFIER_OCTAL },
      { 0 }
    }
  );
}

void lexer_punctuator_tests() {
  printf(" Punctuator Tests:\n");
  lexer_test(
    "Single Character Punctuator Lexing",
    "[ ] ( ) { } . , & * + - ~ ! / % < > ^ | ? : ; = #",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_LBRACKET },
      { .type = REFLECT_TOKEN_RBRACKET },
      { .type = REFLECT_TOKEN_LPAREN },
      { .type = REFLECT_TOKEN_RPAREN },
      { .type = REFLECT_TOKEN_LBRACE },
      { .type = REFLECT_TOKEN_RBRACE },
      { .type = REFLECT_TOKEN_DOT },
      { .type = REFLECT_TOKEN_COMMA },
      { .type = REFLECT_TOKEN_AMPERSAND },
      { .type = REFLECT_TOKEN_STAR },
      { .type = REFLECT_TOKEN_PLUS },
      { .type = REFLECT_TOKEN_MINUS },
      { .type = REFLECT_TOKEN_TILDE },
      { .type = REFLECT_TOKEN_NOT },
      { .type = REFLECT_TOKEN_SLASH },
      { .type = REFLECT_TOKEN_PERCENT },
      { .type = REFLECT_TOKEN_LESS },
      { .type = REFLECT_TOKEN_GREATER },
      { .type = REFLECT_TOKEN_CARET },
      { .type = REFLECT_TOKEN_PIPE },
      { .type = REFLECT_TOKEN_QUESTION },
      { .type = REFLECT_TOKEN_COLON },
      { .type = REFLECT_TOKEN_SEMICOLON },
      { .type = REFLECT_TOKEN_ASSIGN },
      { .type = REFLECT_TOKEN_HASH },
      { 0 }
    }
  );

  lexer_test(
    "Double Character Punctuator Lexing",
    "-> ++ -- << >> <= >= == != && || *= /= %= += -= &= ^= ##",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_ARROW },
      { .type = REFLECT_TOKEN_INCREMENT },
      { .type = REFLECT_TOKEN_DECREMENT },
      { .type = REFLECT_TOKEN_LSHIFT },
      { .type = REFLECT_TOKEN_RSHIFT },
      { .type = REFLECT_TOKEN_LESS_EQUAL },
      { .type = REFLECT_TOKEN_GREATER_EQUAL },
      { .type = REFLECT_TOKEN_EQUALS },
      { .type = REFLECT_TOKEN_NOT_EQUALS },
      { .type = REFLECT_TOKEN_LOGICAL_AND },
      { .type = REFLECT_TOKEN_LOGICAL_OR },
      { .type = REFLECT_TOKEN_MUL_ASSIGN },
      { .type = REFLECT_TOKEN_DIV_ASSIGN },
      { .type = REFLECT_TOKEN_MOD_ASSIGN },
      { .type = REFLECT_TOKEN_ADD_ASSIGN },
      { .type = REFLECT_TOKEN_SUB_ASSIGN },
      { .type = REFLECT_TOKEN_AND_ASSIGN },
      { .type = REFLECT_TOKEN_XOR_ASSIGN },
      { .type = REFLECT_TOKEN_HASH_HASH },
      { 0 }
    }
  );

  lexer_test(
    "Triple Character Punctuator Lexing",
    "<<= >>= ...",
    (ReflectToken[]) {
      { .type = REFLECT_TOKEN_LSHIFT_ASSIGN },
      { .type = REFLECT_TOKEN_RSHIFT_ASSIGN },
      { .type = REFLECT_TOKEN_ELLIPSIS },
      { 0 }
    }
  );
}