#include <cctype>
#include <stdlib.h>
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <string>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <cstdarg>

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


// Reports an error and exit.
static void error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
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
        int val;      /* If kind is TK_NUM, its value */
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



    /* Parser */
    typedef enum {
        ND_ADD, // +
        ND_SUB, // -
        ND_MUL, // *
        ND_DIV, // /
        ND_NEG, // unary -
        ND_NUM, // integer
        ND_ERR,
    } NodeKind;

    // AST node type
    typedef struct Node Node;
    struct Node {
        NodeKind kind; // Node kind
        Node *lhs;     // Left-hand side
        Node *rhs;     // Right-hand side
        int val;       // Used if kind == ND_NUM
    };

    class Parser {
    public:
        Parser() {
            start = new_empty_node();
        }

        ~Parser() {
            dealloc(start);
        }

        Node* Parse(Token* tok);

    protected:
        Token* token;   /* Input tokens */
        Node* start;    /* Output nodes */

        /* equation = add
         */
        Node* equation(Token **rest, Token *tok);

        /* add = mul ("+" mul | "-" mul)*
         */
        Node* add(Token **rest, Token *tok);

        /* mul = unary ("*" unary | "/" unary)* 
         */
        Node* mul(Token **rest, Token *tok);

        /* unary = ("+" | "-") unary
         *       | primary    .
         */
        Node* unary(Token **rest, Token *tok);

        /* primary = "(" add ")"
         *         | num
         */
        Node* primary(Token **rest, Token *tok);


        /* helper functions */
        void dealloc(Node* node);
        Node* new_empty_node();
        Node* new_node(NodeKind kind);
        Node* new_unary(NodeKind kind, Node *expr);
        Node* new_binary(NodeKind kind, Node *lhs, Node *rhs);
        Node* new_num(int val);

        bool equal(Token *tok, char *op);
        Token* skip(Token *tok, char *s);
    };


    /* Code Generator */

    class Generator {
    public:
        Generator() {
            depth = 0;
        }

        void GenCode(Node *node);

        void GenASM(Node* node);

    protected:
        Node *start;
        int depth;

        void push();
        void pop(const char* reg);

        void prologue_mac();
        void prologue_linux();
    };

} // namespace calc
