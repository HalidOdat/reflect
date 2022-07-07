#ifndef _REFLECT_H_
#define _REFLECT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum ReflectTokenType {
  REFLECT_TOKEN_EOF,
  REFLECT_TOKEN_INTEGER,
} ReflectTokenType;

typedef struct ReflectToken {
  ReflectTokenType type;
  
  union {
    int integer;
  };
} ReflectToken;

typedef struct ReflectLexer {
  const char*  source;
  const char*  stream;
  ReflectToken token;
} ReflectLexer;

extern void reflect_lexer_init(ReflectLexer* lexer, const char* source);
extern void reflect_lexer_token_current(ReflectLexer* lexer, ReflectToken* token);
extern bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token);

#ifdef REFLECT_IMPLEMENTATION

#include <ctype.h>

#define REFLECT_API

REFLECT_API void reflect_lexer_init(ReflectLexer* lexer, const char* source) {
  lexer->source = source;
  lexer->stream = source;
}

REFLECT_API void reflect_lexer_token_current(ReflectLexer* lexer, ReflectToken* out_token) {
  // TODO: Check if valid token
  *out_token = lexer->token;
}

static char _reflect_lexer_char_current(ReflectLexer* lexer) {
  return *lexer->stream;
}

static char _reflect_lexer_char_next(ReflectLexer* lexer) {
  char c = *lexer->stream;
  lexer->stream++;
  return c;
}

static bool _reflect_lexer_next_integer(ReflectLexer* lexer, int* out_result) {
  int result = 0;
  char c;
  while (isdigit(c = _reflect_lexer_char_next(lexer))) {
    result = result * 10 + (c - '0'); 
  }
  *out_result = result;
}

REFLECT_API bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token) {
_reflect_lexer_again:
  switch (_reflect_lexer_char_current(lexer)) {
    case '\0':
      lexer->token.type = REFLECT_TOKEN_EOF;
      return true;
    case ' ':
      _reflect_lexer_char_current(lexer);
      goto _reflect_lexer_again;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      lexer->token.type    = REFLECT_TOKEN_INTEGER;
      return _reflect_lexer_next_integer(lexer, &lexer->token.integer);
    default:
      return false;
  }
}

#endif

#endif // _REFLECT_H_