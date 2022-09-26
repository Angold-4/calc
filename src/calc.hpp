#include <cctype>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>

#define ul unsigned long

// Reports an error location and exit
static void verror_at(char *start, char *loc, const char *fmt, va_list ap) {
  // only one line
  int pos = loc - start;
  fprintf(stderr, "%s\n", start);
  fprintf(stderr, "%*s", pos, ""); // print pos spaces.
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

static void error_at(char *start, char *loc, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(start, loc, fmt, ap);
}

namespace calc {

/* Tokenizer */

typedef enum {
  TK_PUNCT, /* Punctuators 0 (+, -, *, /) */
  TK_NUM,   /* Numeric Literals 1 */
  TK_EOF,   /* End-of-file markers 2 */
} TokenKind;

/*
 * Token is a generic container 
 */

struct Token {
  TokenKind kind;
  Token* nxt;  /* A linked list */
  ul val;      /* If kind is TK_NUM, its value */
  char* loc;
  int len;     /* Length of this token */
};


class Tokenizer {
public:
  Tokenizer() {
    start = new_empty_token();
  };

  ~Tokenizer() {
    dealloc(start);
  }

  Token* Tokenize(char* raw);


protected:
  char* raw;     /* The raw char array that need to be tokenized */
  Token* start;  /* The begin of the token linked list */

  void dealloc(Token* token);
  Token* new_token(TokenKind kind, char* start, char* end);
  Token* new_empty_token();
  bool startwith(char *p, std::string q);
  int read_punct(char *p);




};

} // namespace calc
