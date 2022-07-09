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
  REFLECT_TOKEN_NOT,
  REFLECT_TOKEN_SLASH,
  REFLECT_TOKEN_PERCENT,
  REFLECT_TOKEN_LESS,
  REFLECT_TOKEN_GREATER,
  REFLECT_TOKEN_CARET,
  REFLECT_TOKEN_PIPE,
  REFLECT_TOKEN_QUESTION,
  REFLECT_TOKEN_COLON,
  REFLECT_TOKEN_SEMICOLON,
  REFLECT_TOKEN_ASSIGN,
  REFLECT_TOKEN_HASH,
  REFLECT_TOKEN_HASH_HASH,
  REFLECT_TOKEN_ARROW,
  REFLECT_TOKEN_INCREMENT,
  REFLECT_TOKEN_DECREMENT,
  REFLECT_TOKEN_LSHIFT,
  REFLECT_TOKEN_RSHIFT,
  REFLECT_TOKEN_LESS_EQUAL,
  REFLECT_TOKEN_GREATER_EQUAL,
  REFLECT_TOKEN_EQUALS,
  REFLECT_TOKEN_NOT_EQUALS,
  REFLECT_TOKEN_LOGICAL_AND,
  REFLECT_TOKEN_LOGICAL_OR,
  REFLECT_TOKEN_MUL_ASSIGN,
  REFLECT_TOKEN_DIV_ASSIGN,
  REFLECT_TOKEN_MOD_ASSIGN,
  REFLECT_TOKEN_ADD_ASSIGN,
  REFLECT_TOKEN_SUB_ASSIGN,
  REFLECT_TOKEN_AND_ASSIGN,
  REFLECT_TOKEN_XOR_ASSIGN,
  REFLECT_TOKEN_OR_ASSIGN,
  REFLECT_TOKEN_COUNT
} ReflectTokenType;

typedef struct ReflectToken {
  ReflectTokenType type;
  
  union {
    uint64_t integer;
  } as;
} ReflectToken;

typedef enum ReflectLexerError {
  REFLECT_LEXER_ERROR_NONE,
  REFLECT_LEXER_ERROR_INVALID_CHARACTER,
} ReflectLexerError;

#define REFLECT_LEXER_ERROR_MAX_LENGTH 64

typedef struct ReflectLexer {
  const char*       source;
  const char*       stream;
  ReflectLexerError error_code;
  char              error_string[REFLECT_LEXER_ERROR_MAX_LENGTH];
} ReflectLexer;

extern void              reflect_lexer_init(ReflectLexer* lexer, const char* source);
extern bool              reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token);
extern ReflectLexerError reflect_lexer_error_code_get(ReflectLexer* lexer);
extern const char*       reflect_lexer_error_string_get(ReflectLexer* lexer);

extern const char*       reflect_token_type_to_string(ReflectTokenType token_type);




// #-----------------------------------------------------------------------------------------#
// |                                  IMPLEMENTATION                                         |
// #-----------------------------------------------------------------------------------------#

#ifdef REFLECT_IMPLEMENTATION

#include <ctype.h>
#include <string.h>

#define REFLECT_API

REFLECT_API const char* reflect_token_type_to_string(ReflectTokenType token_type) {
  switch (token_type) {
    case REFLECT_TOKEN_EOF:           return "<EOF>";
    case REFLECT_TOKEN_INTEGER:       return "integer";
    case REFLECT_TOKEN_LBRACKET:      return "[";
    case REFLECT_TOKEN_RBRACKET:      return "]";
    case REFLECT_TOKEN_LPAREN:        return "(";
    case REFLECT_TOKEN_RPAREN:        return ")";
    case REFLECT_TOKEN_LBRACE:        return "{";
    case REFLECT_TOKEN_RBRACE:        return "}";
    case REFLECT_TOKEN_DOT:           return ".";
    case REFLECT_TOKEN_AMPERSAND:     return "&";
    case REFLECT_TOKEN_STAR:          return "*";
    case REFLECT_TOKEN_PLUS:          return "+";
    case REFLECT_TOKEN_MINUS:         return "-";
    case REFLECT_TOKEN_TILDE:         return "~";
    case REFLECT_TOKEN_NOT:           return "!";
    case REFLECT_TOKEN_SLASH:         return "/";
    case REFLECT_TOKEN_PERCENT:       return "%";
    case REFLECT_TOKEN_LESS:          return "<";
    case REFLECT_TOKEN_GREATER:       return ">";
    case REFLECT_TOKEN_CARET:         return "^";
    case REFLECT_TOKEN_PIPE:          return "|";
    case REFLECT_TOKEN_QUESTION:      return "?";
    case REFLECT_TOKEN_COLON:         return ":";
    case REFLECT_TOKEN_SEMICOLON:     return ";";
    case REFLECT_TOKEN_ASSIGN:        return "=";
    case REFLECT_TOKEN_HASH:          return "#";
    case REFLECT_TOKEN_HASH_HASH:     return "##";
    case REFLECT_TOKEN_ARROW:         return "->";
    case REFLECT_TOKEN_INCREMENT:     return "++";
    case REFLECT_TOKEN_DECREMENT:     return "--";
    case REFLECT_TOKEN_LSHIFT:        return ">>";
    case REFLECT_TOKEN_RSHIFT:        return "<<";
    case REFLECT_TOKEN_LESS_EQUAL:    return "<=";
    case REFLECT_TOKEN_GREATER_EQUAL: return ">=";
    case REFLECT_TOKEN_EQUALS:        return "==";
    case REFLECT_TOKEN_NOT_EQUALS:    return "!=";
    case REFLECT_TOKEN_LOGICAL_AND:   return "&&";
    case REFLECT_TOKEN_LOGICAL_OR:    return "||";
    case REFLECT_TOKEN_MUL_ASSIGN:    return "*=";
    case REFLECT_TOKEN_DIV_ASSIGN:    return "/=";
    case REFLECT_TOKEN_MOD_ASSIGN:    return "%=";
    case REFLECT_TOKEN_ADD_ASSIGN:    return "+=";
    case REFLECT_TOKEN_SUB_ASSIGN:    return "+=";
    case REFLECT_TOKEN_AND_ASSIGN:    return "&=";
    case REFLECT_TOKEN_OR_ASSIGN:     return "|=";
    case REFLECT_TOKEN_XOR_ASSIGN:    return "^=";
    default:                          return NULL;
  }
}

REFLECT_API void reflect_lexer_init(ReflectLexer* lexer, const char* source) {
  lexer->source          = source;
  lexer->stream          = source;
  lexer->error_code      = REFLECT_LEXER_ERROR_NONE;
  strcpy(lexer->error_string, "");
}

static char _reflect_lexer_char_current(ReflectLexer* lexer) {
  return *lexer->stream;
}

static void _reflect_lexer_char_advance(ReflectLexer* lexer) {
  ++lexer->stream;
}

static bool _reflect_lexer_integer_lex(ReflectLexer* lexer, uint64_t* out_result) {
  uint64_t result = 0;
  while (isdigit(_reflect_lexer_char_current(lexer))) {
    result = result * 10 + (uint64_t)(_reflect_lexer_char_current(lexer) - '0');
    _reflect_lexer_char_advance(lexer);
  }
  *out_result = result;

  return true;
}

REFLECT_API bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token) {

#define _REFLECT_LEXER_CASE1(c, t)      \
  case (c):                             \
    token->type = (t);                  \
    _reflect_lexer_char_advance(lexer); \
    return true

#define _REFLECT_LEXER_CASE2(c1, t1, c2, t2)          \
  case (c1):                                          \
    token->type = (t1);                               \
    _reflect_lexer_char_advance(lexer);               \
    if (_reflect_lexer_char_current(lexer) == (c2)) { \
      token->type = (t2);                             \
      _reflect_lexer_char_advance(lexer);             \
    }                                                 \
    return true

#define _REFLECT_LEXER_CASE3(c1, t1, c2, t2, c3, t3)          \
  case (c1):                                                  \
    token->type = (t1);                                       \
    _reflect_lexer_char_advance(lexer);                       \
    if (_reflect_lexer_char_current(lexer) == (c2)) {         \
      token->type = (t2);                                     \
      _reflect_lexer_char_advance(lexer);                     \
    } else if (_reflect_lexer_char_current(lexer) == (c3)) {  \
      token->type = (t3);                                     \
      _reflect_lexer_char_advance(lexer);                     \
    }                                                         \
    return true


_reflect_lexer_again:
  switch (_reflect_lexer_char_current(lexer)) {
    case '\0':
      token->type = REFLECT_TOKEN_EOF;
      printf("EOF reached");
      return true;
    case ' ':
      _reflect_lexer_char_advance(lexer);
      goto _reflect_lexer_again;
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      token->type = REFLECT_TOKEN_INTEGER;
      return _reflect_lexer_integer_lex(lexer, &token->as.integer);

    _REFLECT_LEXER_CASE1('[', REFLECT_TOKEN_LBRACKET);
    _REFLECT_LEXER_CASE1(']', REFLECT_TOKEN_RBRACKET);
    _REFLECT_LEXER_CASE1('(', REFLECT_TOKEN_LPAREN);
    _REFLECT_LEXER_CASE1(')', REFLECT_TOKEN_RPAREN);
    _REFLECT_LEXER_CASE1('{', REFLECT_TOKEN_LBRACE);
    _REFLECT_LEXER_CASE1('}', REFLECT_TOKEN_RBRACE);
    _REFLECT_LEXER_CASE1('.', REFLECT_TOKEN_DOT);
    _REFLECT_LEXER_CASE1('~', REFLECT_TOKEN_TILDE);
    _REFLECT_LEXER_CASE1('?', REFLECT_TOKEN_QUESTION);
    _REFLECT_LEXER_CASE1(':', REFLECT_TOKEN_COLON);
    _REFLECT_LEXER_CASE1(';', REFLECT_TOKEN_SEMICOLON);

    _REFLECT_LEXER_CASE2('#', REFLECT_TOKEN_HASH,      '#', REFLECT_TOKEN_HASH_HASH);
    _REFLECT_LEXER_CASE2('^', REFLECT_TOKEN_CARET,     '=', REFLECT_TOKEN_XOR_ASSIGN);
    _REFLECT_LEXER_CASE2('=', REFLECT_TOKEN_ASSIGN,    '=', REFLECT_TOKEN_EQUALS);
    _REFLECT_LEXER_CASE2('%', REFLECT_TOKEN_PERCENT,   '=', REFLECT_TOKEN_MOD_ASSIGN);
    _REFLECT_LEXER_CASE2('!', REFLECT_TOKEN_NOT,       '=', REFLECT_TOKEN_NOT_EQUALS);

    _REFLECT_LEXER_CASE2('&', REFLECT_TOKEN_AMPERSAND, '=', REFLECT_TOKEN_AND_ASSIGN);
    _REFLECT_LEXER_CASE2('*', REFLECT_TOKEN_STAR,      '=', REFLECT_TOKEN_MUL_ASSIGN);

    _REFLECT_LEXER_CASE2('|', REFLECT_TOKEN_PIPE,      '=', REFLECT_TOKEN_OR_ASSIGN);
    _REFLECT_LEXER_CASE2('+', REFLECT_TOKEN_PLUS,      '=', REFLECT_TOKEN_ADD_ASSIGN);
    _REFLECT_LEXER_CASE2('-', REFLECT_TOKEN_MINUS,     '=', REFLECT_TOKEN_SUB_ASSIGN);
    _REFLECT_LEXER_CASE2('/', REFLECT_TOKEN_SLASH,     '=', REFLECT_TOKEN_DIV_ASSIGN);
    _REFLECT_LEXER_CASE2('<', REFLECT_TOKEN_LESS,      '=', REFLECT_TOKEN_LESS_EQUAL);
    _REFLECT_LEXER_CASE2('>', REFLECT_TOKEN_GREATER,   '=', REFLECT_TOKEN_GREATER_EQUAL);

    default:
      lexer->error_code = REFLECT_LEXER_ERROR_INVALID_CHARACTER;
      snprintf(
        lexer->error_string,
        REFLECT_LEXER_ERROR_MAX_LENGTH,
        "invalid character '%c'",
        _reflect_lexer_char_current(lexer)
      );
      return false;
  }

  #undef _REFLECT_LEXER_CASE1
  #undef _REFLECT_LEXER_CASE2
  #undef _REFLECT_LEXER_CASE3
}

REFLECT_API ReflectLexerError reflect_lexer_error_code_get(ReflectLexer* lexer) {
  return lexer->error_code;
}

REFLECT_API const char* reflect_lexer_error_string_get(ReflectLexer* lexer) {
  return lexer->error_string;
}


#endif // REFLECT_IMPLEMENTATION
#endif // _REFLECT_H_