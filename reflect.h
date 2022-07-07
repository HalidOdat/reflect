#ifndef _REFLECT_H_
#define _REFLECT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum ReflectTokenType {
  REFLECT_TOKEN_EOF,
  REFLECT_TOKEN_INTEGER,
  REFLECT_TOKEN_LBRACKET,
  REFLECT_TOKEN_RBRACKET,
  REFLECT_TOKEN_LPAREN,
  REFLECT_TOKEN_RPAREN,
  REFLECT_TOKEN_LBRACE,
  REFLECT_TOKEN_RBRACE,
  REFLECT_TOKEN_DOT,
  REFLECT_TOKEN_AMPERSAND,
  REFLECT_TOKEN_STAR,
  REFLECT_TOKEN_PLUS,
  REFLECT_TOKEN_MINUS,
  REFLECT_TOKEN_TILDE,
  REFLECT_TOKEN_EXCLAMATION,
  REFLECT_TOKEN_SLASH,
  REFLECT_TOKEN_PERCENT,
  REFLECT_TOKEN_LESS,
  REFLECT_TOKEN_GREATER,
  REFLECT_TOKEN_CARET,
  REFLECT_TOKEN_PIPE,
  REFLECT_TOKEN_QUESTION,
  REFLECT_TOKEN_COLON,
  REFLECT_TOKEN_SEMICOLON,
  REFLECT_TOKEN_EQUAL,
  REFLECT_TOKEN_HASH,
} ReflectTokenType;

typedef struct ReflectToken {
  ReflectTokenType type;
  
  union {
    uint64_t integer;
  };
} ReflectToken;

typedef struct ReflectLexer {
  const char*  source;
  const char*  stream;
  // ReflectToken token;
} ReflectLexer;

extern void reflect_lexer_init(ReflectLexer* lexer, const char* source);
// extern void reflect_lexer_token_current(ReflectLexer* lexer, ReflectToken* token);
extern bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token);

#ifdef REFLECT_IMPLEMENTATION

#include <ctype.h>

#define REFLECT_API

REFLECT_API void reflect_lexer_init(ReflectLexer* lexer, const char* source) {
  lexer->source = source;
  lexer->stream = source;
}

// REFLECT_API void reflect_lexer_token_current(ReflectLexer* lexer, ReflectToken* out_token) {
//   // TODO: Check if valid token
//   *out_token = lexer->token;
// }

static char _reflect_lexer_char_current(ReflectLexer* lexer) {
  return *lexer->stream;
}

static char _reflect_lexer_char_advance(ReflectLexer* lexer) {
  ++lexer->stream;
}

static char _reflect_lexer_char_next(ReflectLexer* lexer) {
  char c = *lexer->stream;
  lexer->stream++;
  return c;
}

static bool _reflect_lexer_integer_lex(ReflectLexer* lexer, uint64_t* out_result) {
  uint64_t result = 0;
  char c;
  while (isdigit(c = _reflect_lexer_char_next(lexer))) {
    result = result * 10 + (c - '0'); 
  }
  *out_result = result;

  return true;
}


REFLECT_API bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token) {

#define _REFLECT_LEXER_CASE1(c, t)   \
  case (c):                          \
    token->type = (t);               \
    _reflect_lexer_char_advance(lexer); \
    return true

_reflect_lexer_again:
  switch (_reflect_lexer_char_current(lexer)) {
    case '\0':
      token->type = REFLECT_TOKEN_EOF;
      return true;
    case ' ':
      _reflect_lexer_char_advance(lexer);
      goto _reflect_lexer_again;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      token->type = REFLECT_TOKEN_INTEGER;
      return _reflect_lexer_integer_lex(lexer, &token->integer);

    _REFLECT_LEXER_CASE1('[', REFLECT_TOKEN_LBRACKET);
    _REFLECT_LEXER_CASE1(']', REFLECT_TOKEN_RBRACKET);
    _REFLECT_LEXER_CASE1('(', REFLECT_TOKEN_LPAREN);
    _REFLECT_LEXER_CASE1(')', REFLECT_TOKEN_RPAREN);
    _REFLECT_LEXER_CASE1('{', REFLECT_TOKEN_LBRACE);
    _REFLECT_LEXER_CASE1('}', REFLECT_TOKEN_RBRACE);
    _REFLECT_LEXER_CASE1('.', REFLECT_TOKEN_DOT);
    _REFLECT_LEXER_CASE1('&', REFLECT_TOKEN_AMPERSAND);
    _REFLECT_LEXER_CASE1('*', REFLECT_TOKEN_STAR);
    _REFLECT_LEXER_CASE1('+', REFLECT_TOKEN_PLUS);
    _REFLECT_LEXER_CASE1('-', REFLECT_TOKEN_MINUS);
    _REFLECT_LEXER_CASE1('~', REFLECT_TOKEN_TILDE);
    _REFLECT_LEXER_CASE1('!', REFLECT_TOKEN_EXCLAMATION);
    _REFLECT_LEXER_CASE1('/', REFLECT_TOKEN_SLASH);
    _REFLECT_LEXER_CASE1('%', REFLECT_TOKEN_PERCENT);
    _REFLECT_LEXER_CASE1('<', REFLECT_TOKEN_LESS);
    _REFLECT_LEXER_CASE1('>', REFLECT_TOKEN_GREATER);
    _REFLECT_LEXER_CASE1('^', REFLECT_TOKEN_CARET);
    _REFLECT_LEXER_CASE1('|', REFLECT_TOKEN_PIPE);
    _REFLECT_LEXER_CASE1('?', REFLECT_TOKEN_QUESTION);
    _REFLECT_LEXER_CASE1(':', REFLECT_TOKEN_COLON);
    _REFLECT_LEXER_CASE1(';', REFLECT_TOKEN_SEMICOLON);
    _REFLECT_LEXER_CASE1('=', REFLECT_TOKEN_EQUAL);
    _REFLECT_LEXER_CASE1('#', REFLECT_TOKEN_HASH);
    default:
      return false;
  }

  #undef _REFLECT_LEXER_CASE1
}

#endif

#endif // _REFLECT_H_