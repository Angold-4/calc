#include "calc.hpp"

namespace calc {
    Token* Tokenizer::Tokenize(char *r) {
        raw = r;
        Token head = {};
        Token* curr = &head;

        while (*r) {
            /* Skip white space */
            if (std::isspace(*r)) {
                r++;
                continue;
            }

            /* Numeric value */
            if (std::isdigit(*r)) {
                curr = curr->nxt = new_token(TK_NUM, r, r);
                char *p = r; /* previous */
                curr->val = std::strtoul(r, &r, 10);
                curr->len = r - p;
                continue;
            }
            int punct_len = read_punct(r);
            if (punct_len) {
                curr = curr->nxt = new_token(TK_PUNCT, r, r+punct_len);
                r += punct_len;
                continue;
            }

            /* If we reach here, must be an error */
            std::string tmpmsg = "Invalid token";
            error_at(raw, r, tmpmsg.c_str());
        }

        curr = curr->nxt = new_token(TK_EOF, r, r);

        start = head.nxt;
        return start;
    };


    void Tokenizer::dealloc(Token *token) {
        Token* tmp = nullptr;
        while (token) {
            tmp = token->nxt;
            std::free(token);
            token = tmp;
        }
    }

    Token* Tokenizer::new_token(TokenKind kind, char* start, char* end) {
        Token *tok = (Token*) std::malloc(sizeof(Token));
        tok->kind = kind;
        tok->loc = start;
        tok->len = end - start;
        return tok;
    };


    Token* Tokenizer::new_empty_token() {
        return (Token*) std::malloc(sizeof(Token));
    };

    // Check whether p start with *q (q better be a const char*)
    bool Tokenizer::startwith(char *p, std::string q) {
        return std::strncmp(p, q.c_str(), q.size()) == 0;
    }

    int Tokenizer::read_punct(char *p) {
        if (startwith(p, ">=") || startwith(p, "<=") ||
            startwith(p, "==") || startwith(p, "!="))
            return 2;
        return std::ispunct(*p) ? 1 : 0;
    };

} // namespace calc
