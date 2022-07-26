#ifndef CAPSTONE_PARSER
#define CAPSTONE_PARSER

#include "common.h"
#include "lexer.h"
#include "ast.h"

// Dont use this, just lols
#define PFN(name) Node* parser##name(void)

class Parser {
  public:
    Parser(Lexer* lexer) : lexer(lexer) {
    }
    Node* parse(void);

  private:
    Lexer* lexer;

    Node* parseFile(void);
    Node* parseFuncDecl(void);
    Node* parseVarIdent(void);
    Node* parseTypeIdent(void);
    Node* parseParamDecl(void);
    Node* parseBlock(void);
    Node* parseStatement(void);

    Node* parseLogical(void);
    Node* parseLogicalOr(void);
    Node* parseLogicalAnd(void);
};

#endif