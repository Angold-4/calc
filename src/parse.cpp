#include "calc.hpp"

namespace calc {

Node* Parser::Parse(Token* tok) {
  token = tok;
  Node* ret = equation(&tok, tok);

  if (tok->kind != TK_EOF) {
    error_at(token->loc, tok->loc, "Extra token");
  }

  return ret;
};

Node* Parser::equation(Token **rest, Token* tok) {
  return add(rest, tok);
};

Node* Parser::add(Token **rest, Token *tok) {
  Node* node = mul(&tok, tok); /* recursive */
  char _plus[] = "+"; char _minus[] = "-";
  while (true) { /* "*" */
    if (equal(tok, _plus)) {
      node = new_binary(ND_ADD, node, mul(&tok, tok->nxt));
      continue;
    }
    if (equal(tok, _minus)) {
      node = new_binary(ND_SUB, node, mul(&tok, tok->nxt));
      continue;
    }
    *rest = tok;
    return node;
  }
};

Node* Parser::mul(Token **rest, Token *tok) {
  Node* node = unary(&tok, tok); // will update the tok pointer
  char _time[] = "*"; char _divide[] = "/";
  while (true) { /* "*" */
    if (equal(tok, _time)) {
      node = new_binary(ND_MUL, node, unary(&tok, tok->nxt));
      continue;
    }
    if (equal(tok, _divide)) {
      node = new_binary(ND_DIV, node, unary(&tok, tok->nxt));
      continue;
    }
    *rest = tok;
    return node;
  }
};

Node* Parser::unary(Token **rest, Token *tok) {
  char _plus[] = "+"; char _minus[] = "-";
  if (equal(tok, _plus)) {
    return unary(rest, tok->nxt);
  }
  if (equal(tok, _minus)) {
    return new_unary(ND_NEG, unary(rest, tok->nxt));
  }

  return primary(rest, tok);
};

Node *Parser::primary(Token **rest, Token *tok) {
  char _b[] = "("; char _d[] = ")";
  if (equal(tok, _b)) {
    Node* node = add(&tok, tok->nxt);
    *rest = skip(tok, _d);
    return node;
  }

  if (tok->kind == TK_NUM) {
    Node* node = new_num(tok->val);
    *rest = tok->nxt;
    return node;
  }

  /* Syntax error */
  
  char tmp[50];
  snprintf(tmp, sizeof(tmp), "syntax error");
  const char* ttmp = tmp;
  error_at(token->loc, tok->loc, ttmp);
  return new_node(ND_ERR);
};

void Parser::dealloc(Node* node) {
  if (node == nullptr) {
    return;
  }
  dealloc(node->lhs);
  dealloc(node->rhs);
  std::free(node);
};

Node* Parser::new_empty_node() {
  return (Node*) std::malloc(sizeof(Node));
};

Node* Parser::new_node(NodeKind kind) {
  Node* tmp = new_empty_node();
  tmp->kind = kind;
  return tmp;
};

Node* Parser::new_unary(NodeKind kind, Node *expr) {
  Node* tmp = new_empty_node();
  tmp->kind = kind;
  tmp->lhs = expr;
  return tmp;
};

Node* Parser::new_binary(NodeKind kind, Node *lhs, Node *rhs) {
  Node* tmp = new_empty_node();
  tmp->kind = kind;
  tmp->lhs = lhs;
  tmp->rhs = rhs;
  return tmp;
};

Node* Parser::new_num(int val) {
  Node* tmp = new_empty_node();
  tmp->kind = ND_NUM;
  tmp->val = val;
  return tmp;
};

bool Parser::equal(Token *tok, char *op) {
  return std::memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
};

/* Ensure that the current token is `s`. and then 'consume' it */
Token* Parser::skip(Token *tok, char *s) {
  if (!equal(tok, s)) {
    char tmp[50];
    snprintf(tmp, sizeof(tmp), "expected '%s'", s);
    const char* ttmp = tmp;
    error_at(token->loc, tok->loc, ttmp);
  }
  return tok->nxt;
};

}; // namespace calc
