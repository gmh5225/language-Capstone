#ifndef CAPSTONE_PARSER
#define CAPSTONE_PARSER

#include "common.h"
#include "lexer.h"
#include "ast.h"

class Parser {
  private:
    Lexer* lexer;

  public:
    Parser(Lexer* lexer) : lexer(lexer) {
    }
    Node* parse(void);
};

#endif