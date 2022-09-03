#ifndef reflect__H_
#define reflect__H_

#include <stdint.h>
#include <stdbool.h>

// TODO: Temporary
#define REFLECT_MAX_INDENTIFIER_LENGTH 256
#define REFLECT_MAX_SUFFIX_LENGTH 32

typedef enum ReflectTokenType {
  REFLECT_TOKEN_EOF,
  REFLECT_TOKEN_IDENTIFIER,
  REFLECT_TOKEN_INTEGER,
  REFLECT_TOKEN_LBRACKET,
  REFLECT_TOKEN_RBRACKET,
  REFLECT_TOKEN_LPAREN,
  REFLECT_TOKEN_RPAREN,
  REFLECT_TOKEN_LBRACE,
  REFLECT_TOKEN_RBRACE,
  REFLECT_TOKEN_DOT,
  REFLECT_TOKEN_COMMA,
  REFLECT_TOKEN_ELLIPSIS,
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
  REFLECT_TOKEN_LSHIFT_ASSIGN,
  REFLECT_TOKEN_RSHIFT_ASSIGN,
  REFLECT_TOKEN_COUNT
} ReflectTokenType;

typedef enum ReflectModifier {
  REFLECT_MODIFIER_NONE,
  REFLECT_MODIFIER_OCTAL,
  REFLECT_MODIFIER_HEXADECIMAL,
  REFLECT_MODIFIER_COUNT,
} ReflectModifier;

typedef enum ReflectSuffix {
  REFLECT_SUFFIX_NONE,
  REFLECT_SUFFIX_L,
  REFLECT_SUFFIX_LL,
  REFLECT_SUFFIX_U,
  REFLECT_SUFFIX_UL,
  REFLECT_SUFFIX_ULL,
} ReflectSuffix;

typedef struct ReflectSourceLocation {
  uint32_t line;
  uint32_t column;
} ReflectSourceLocation;

typedef struct ReflectToken {
  ReflectTokenType      type;
  ReflectModifier       modifier;
  ReflectSourceLocation location;

  char suffix_string[REFLECT_MAX_SUFFIX_LENGTH + 1];

  union {
    uint64_t integer;
    char     identifier[REFLECT_MAX_INDENTIFIER_LENGTH];
  } as;
} ReflectToken;

typedef enum ReflectError {
  REFLECT_ERROR_NONE,
  REFLECT_ERROR_LEXER_BEGIN,
  REFLECT_ERROR_INVALID_CHARACTER       = REFLECT_ERROR_LEXER_BEGIN,
  REFLECT_ERROR_INVALID_INTEGER,
  REFLECT_ERROR_LEXER_END               = REFLECT_ERROR_INVALID_INTEGER,
  REFLECT_ERROR_COUNT,
} ReflectError;

#define REFLECT_LEXER_ERROR_STRING_MAX_LENGTH 512

typedef struct ReflectLexer {
  const char*           source;
  const char*           stream;
  ReflectError          error_code;
  char                  error_string[REFLECT_LEXER_ERROR_STRING_MAX_LENGTH];
  ReflectSourceLocation location;
} ReflectLexer;

extern void         reflect_lexer_init(ReflectLexer* lexer, const char* source);
extern bool         reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token);
extern ReflectError reflect_lexer_error_code_get(ReflectLexer* lexer);
extern const char*  reflect_lexer_error_string_get(ReflectLexer* lexer);

extern const char*  reflect_token_type_to_string(ReflectTokenType token_type);


// #-----------------------------------------------------------------------------------------#
// |                                  IMPLEMENTATION                                         |
// #-----------------------------------------------------------------------------------------#

#ifdef REFLECT_IMPLEMENTATION

#include <ctype.h>
#include <string.h>
#include <assert.h>

#define REFLECT_API

REFLECT_API const char* reflect_token_type_to_string(ReflectTokenType token_type) {
  switch (token_type) {
    case REFLECT_TOKEN_EOF:           return "<EOF>";
    case REFLECT_TOKEN_IDENTIFIER:    return "identifier";
    case REFLECT_TOKEN_INTEGER:       return "integer";
    case REFLECT_TOKEN_LBRACKET:      return "[";
    case REFLECT_TOKEN_RBRACKET:      return "]";
    case REFLECT_TOKEN_LPAREN:        return "(";
    case REFLECT_TOKEN_RPAREN:        return ")";
    case REFLECT_TOKEN_LBRACE:        return "{";
    case REFLECT_TOKEN_RBRACE:        return "}";
    case REFLECT_TOKEN_DOT:           return ".";
    case REFLECT_TOKEN_COMMA:         return ",";
    case REFLECT_TOKEN_ELLIPSIS:      return "...";
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
    case REFLECT_TOKEN_SUB_ASSIGN:    return "-=";
    case REFLECT_TOKEN_AND_ASSIGN:    return "&=";
    case REFLECT_TOKEN_OR_ASSIGN:     return "|=";
    case REFLECT_TOKEN_XOR_ASSIGN:    return "^=";
    case REFLECT_TOKEN_LSHIFT_ASSIGN: return "<<=";
    case REFLECT_TOKEN_RSHIFT_ASSIGN: return ">>=";
    default:                          return NULL;
  }
}

REFLECT_API void reflect_lexer_init(ReflectLexer* lexer, const char* source) {
  lexer->source          = source;
  lexer->stream          = source;
  lexer->location.line   = 1;
  lexer->location.column = 1;
  
  lexer->error_code      = REFLECT_ERROR_NONE;
  strcpy(lexer->error_string, "");
}

REFLECT_API ReflectError reflect_lexer_error_code_get(ReflectLexer* lexer) {
  return lexer->error_code;
}

REFLECT_API const char* reflect_lexer_error_string_get(ReflectLexer* lexer) {
  return lexer->error_string;
}

static char reflect__lexer_char_current(ReflectLexer* lexer) {
  return *lexer->stream;
}

static void reflect__lexer_char_advance(ReflectLexer* lexer) {
  ++lexer->location.column;
  ++lexer->stream;
}

static void reflect__lexer_char_back(ReflectLexer* lexer) {
  --lexer->location.column;
  --lexer->stream;
}

static bool reflect__lexer_char_next_if(ReflectLexer* lexer, const char c) {
  if (reflect__lexer_char_current(lexer) == c) {
    reflect__lexer_char_advance(lexer);
    return true;
  }
  return false;
}

static bool reflect__is_digit(const char c, uint8_t radix) {
  switch (radix) {
    case 2:  return c == '0' || c == '1';
    case 8:  return c >= '0' && c <= '7';
    case 10: return c >= '0' && c <= '9';
    case 16: return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
  }
  assert(false && "Unreachable");
}

static bool reflect__to_digit(const char c, const uint8_t radix, uint8_t* digit) {
  if (!reflect__is_digit(c, radix)) {
    return false;
  }

  static uint8_t digits[256] = {
    ['0'] = 0,
    ['1'] = 1,
    ['2'] = 2,
    ['3'] = 3, 
    ['4'] = 4, 
    ['5'] = 5, 
    ['6'] = 6, 
    ['7'] = 7, 
    ['8'] = 8, 
    ['9'] = 9, 
    ['a'] = 10, ['A'] = 10,
    ['b'] = 11, ['B'] = 11,
    ['c'] = 12, ['C'] = 12,
    ['d'] = 13, ['D'] = 13,
    ['e'] = 14, ['E'] = 14,
    ['f'] = 15, ['F'] = 15,
  };

  *digit = digits[(size_t)c];
  return true;
}

static const char* reflect__integer_radix_name(uint8_t radix) {
  switch (radix) {
    case 2:  return "binary";
    case 10: return "decimal";
    case 16: return "hexadecimal";
  }
  assert(false && "Unreachable");
}

static bool reflect__lexer_current_char_is_digit(ReflectLexer* lexer, uint8_t radix) {
  return reflect__is_digit(reflect__lexer_char_current(lexer), radix);
}

static bool reflect__lexer_current_char_to_digit(ReflectLexer* lexer, uint8_t radix, uint8_t* digit) {
  return reflect__to_digit(reflect__lexer_char_current(lexer), radix, digit);
}

static bool reflect__lexer_token_identifier_lex(ReflectLexer* lexer, ReflectToken* token) {
  token->type = REFLECT_TOKEN_IDENTIFIER;
  size_t count = 0;
  char c;
  while (isalnum(c = reflect__lexer_char_current(lexer)) || c == '_') {
    if (count + 2 == REFLECT_MAX_INDENTIFIER_LENGTH) {
      assert(false && "FIXME");
    }
    token->as.identifier[count++] = c;
    reflect__lexer_char_advance(lexer);
  }

  token->as.identifier[count] = '\0';
  return true;
}

static bool reflect__lexer_token_integer_lex(ReflectLexer* lexer, ReflectToken* token) {
  token->type     = REFLECT_TOKEN_INTEGER;

  uint64_t result = 0;
  uint8_t  radix  = 10;

  // Check for radix specification.
  if (reflect__lexer_char_next_if(lexer, '0')) {
    if (tolower(reflect__lexer_char_current(lexer)) == 'x') {
      reflect__lexer_char_advance(lexer);

      if (!reflect__lexer_current_char_is_digit(lexer, 16)) {
        reflect__lexer_char_back(lexer);
      } else {
        token->modifier = REFLECT_MODIFIER_HEXADECIMAL;
        radix = 16;
      }
    } else if (reflect__lexer_current_char_is_digit(lexer, 8)) {
      token->modifier = REFLECT_MODIFIER_OCTAL;
      radix = 8;
    }
  }

  uint8_t digit;
  while (reflect__lexer_current_char_to_digit(lexer, radix, &digit)) {
    result = result * (uint64_t)radix + (uint64_t)digit;
    reflect__lexer_char_advance(lexer);
  }

  // TODO: Handle suffix
  char c;
  if (isalpha(c = reflect__lexer_char_current(lexer)) || c == '_') {
    size_t count = 0;
    while (isalnum(c = reflect__lexer_char_current(lexer)) || c == '_') {
      if (count + 2 == REFLECT_MAX_SUFFIX_LENGTH) {
        assert(false && "TODO: handle bigger suffix");
      }
      token->suffix_string[count++] = c;
      reflect__lexer_char_advance(lexer);   
    }
    token->suffix_string[REFLECT_MAX_SUFFIX_LENGTH] = '\0';
  }

  token->as.integer = result;
  return true;
}

REFLECT_API bool reflect_lexer_token_next(ReflectLexer* lexer, ReflectToken* token) {

#define REFLECT__LEXER_CASE1(c, t)      \
  case (c):                             \
    token->type = (t);                  \
    reflect__lexer_char_advance(lexer); \
    return true

#define REFLECT__LEXER_CASE2(c1, t1, c2, t2)        \
  case (c1):                                        \
    token->type = (t1);                             \
    reflect__lexer_char_advance(lexer);             \
    if (reflect__lexer_char_next_if(lexer, (c2))) { \
      token->type = (t2);                           \
    }                                               \
    return true

#define REFLECT__LEXER_CASE3(c1, t1, c2, t2, c3, t3)       \
  case (c1):                                               \
    token->type = (t1);                                    \
    reflect__lexer_char_advance(lexer);                    \
    if (reflect__lexer_char_next_if(lexer, (c2))) {        \
      token->type = (t2);                                  \
    } else if (reflect__lexer_char_next_if(lexer, (c3))) { \
      token->type = (t3);                                  \
    }                                                      \
    return true


reflect__lexer_again:
  token->location = lexer->location;
  token->modifier = REFLECT_MODIFIER_NONE;
  switch (reflect__lexer_char_current(lexer)) {
    case '\0':
      token->type = REFLECT_TOKEN_EOF;
      return true;
    case '\n':
      lexer->location.line   += 1;
      lexer->location.column  = 0;

      // fallthrough
    case ' ':
      reflect__lexer_char_advance(lexer);
      goto reflect__lexer_again;

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': 
    case 'v': case 'w': case 'x': case 'y': case 'z':
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
    case '_':
      return reflect__lexer_token_identifier_lex(lexer, token);

    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return reflect__lexer_token_integer_lex(lexer, token);

    REFLECT__LEXER_CASE1('[', REFLECT_TOKEN_LBRACKET);
    REFLECT__LEXER_CASE1(']', REFLECT_TOKEN_RBRACKET);
    REFLECT__LEXER_CASE1('(', REFLECT_TOKEN_LPAREN);
    REFLECT__LEXER_CASE1(')', REFLECT_TOKEN_RPAREN);
    REFLECT__LEXER_CASE1('{', REFLECT_TOKEN_LBRACE);
    REFLECT__LEXER_CASE1('}', REFLECT_TOKEN_RBRACE);
    REFLECT__LEXER_CASE1(',', REFLECT_TOKEN_COMMA);
    REFLECT__LEXER_CASE1('~', REFLECT_TOKEN_TILDE);
    REFLECT__LEXER_CASE1('?', REFLECT_TOKEN_QUESTION);
    REFLECT__LEXER_CASE1(':', REFLECT_TOKEN_COLON);
    REFLECT__LEXER_CASE1(';', REFLECT_TOKEN_SEMICOLON);
    REFLECT__LEXER_CASE2('#', REFLECT_TOKEN_HASH,      '#', REFLECT_TOKEN_HASH_HASH);
    REFLECT__LEXER_CASE2('^', REFLECT_TOKEN_CARET,     '=', REFLECT_TOKEN_XOR_ASSIGN);
    REFLECT__LEXER_CASE2('=', REFLECT_TOKEN_ASSIGN,    '=', REFLECT_TOKEN_EQUALS);
    REFLECT__LEXER_CASE2('%', REFLECT_TOKEN_PERCENT,   '=', REFLECT_TOKEN_MOD_ASSIGN);
    REFLECT__LEXER_CASE2('!', REFLECT_TOKEN_NOT,       '=', REFLECT_TOKEN_NOT_EQUALS);
    REFLECT__LEXER_CASE2('*', REFLECT_TOKEN_STAR,      '=', REFLECT_TOKEN_MUL_ASSIGN);
    REFLECT__LEXER_CASE3('&', REFLECT_TOKEN_AMPERSAND, '=', REFLECT_TOKEN_AND_ASSIGN, '&', REFLECT_TOKEN_LOGICAL_AND);
    REFLECT__LEXER_CASE3('|', REFLECT_TOKEN_PIPE,      '=', REFLECT_TOKEN_OR_ASSIGN,  '|', REFLECT_TOKEN_LOGICAL_OR);
    REFLECT__LEXER_CASE3('+', REFLECT_TOKEN_PLUS,      '=', REFLECT_TOKEN_ADD_ASSIGN, '+', REFLECT_TOKEN_INCREMENT);
    case '.':
      token->type = REFLECT_TOKEN_DOT;
      reflect__lexer_char_advance(lexer);
      if (lexer->stream[0] == '.' && lexer->stream[1] == '.') {
        reflect__lexer_char_advance(lexer);
        reflect__lexer_char_advance(lexer);
        token->type = REFLECT_TOKEN_ELLIPSIS;
      }
      return true;
    case '-':
      token->type = REFLECT_TOKEN_MINUS;
      reflect__lexer_char_advance(lexer);
      if (reflect__lexer_char_next_if(lexer, '-')) {
        token->type = REFLECT_TOKEN_DECREMENT;
      } else if (reflect__lexer_char_next_if(lexer, '=')) {
        token->type = REFLECT_TOKEN_SUB_ASSIGN;
      } else if (reflect__lexer_char_next_if(lexer, '>')) {
        token->type = REFLECT_TOKEN_ARROW;
      }
      token->location = lexer->location;
      return true;
    case '/':
      token->type = REFLECT_TOKEN_SLASH;
      reflect__lexer_char_advance(lexer);
      if (reflect__lexer_char_next_if(lexer, '=')) {
        token->type = REFLECT_TOKEN_DIV_ASSIGN;
      }
      // TODO: Handle comments
      // else if (reflect__lexer_char_next_if(lexer, '/')) {
      //   token->type = REFLECT_TOKEN_SUB_ASSIGN;
      // }
      token->location = lexer->location;
      return true;
    case '<':
      token->type = REFLECT_TOKEN_LESS;
      reflect__lexer_char_advance(lexer);
      if (reflect__lexer_char_next_if(lexer, '=')) {
        token->type = REFLECT_TOKEN_LESS_EQUAL;
      } else if (reflect__lexer_char_next_if(lexer, '<')) {
        token->type = REFLECT_TOKEN_LSHIFT;
        if (reflect__lexer_char_next_if(lexer, '=')) {
          token->type = REFLECT_TOKEN_LSHIFT_ASSIGN;
        }
      }
      token->location = lexer->location;
      return true;
    case '>':
      token->type = REFLECT_TOKEN_GREATER;
      reflect__lexer_char_advance(lexer);
      if (reflect__lexer_char_next_if(lexer, '=')) {
        token->type = REFLECT_TOKEN_GREATER_EQUAL;
      } else if (reflect__lexer_char_next_if(lexer, '>')) {
        token->type = REFLECT_TOKEN_RSHIFT;
        if (reflect__lexer_char_next_if(lexer, '=')) {
          token->type = REFLECT_TOKEN_RSHIFT_ASSIGN;
        }
      }
      token->location = lexer->location;
      return true;
    default:
      lexer->error_code = REFLECT_ERROR_INVALID_CHARACTER;
      snprintf(
        lexer->error_string,
        REFLECT_LEXER_ERROR_STRING_MAX_LENGTH,
        "invalid character '%c'",
        reflect__lexer_char_current(lexer)
      );
      // Skip over invalid character
      reflect__lexer_char_advance(lexer);
      return false;
  }

  #undef REFLECT__LEXER_CASE1
  #undef REFLECT__LEXER_CASE2
  #undef REFLECT__LEXER_CASE3
}


#endif // REFLECT_IMPLEMENTATION
#endif // reflect__H_