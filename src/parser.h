#ifndef CAPSTONE_PARSER
#define CAPSTONE_PARSER

#include "common.h"
#include "lexer.h"
#include "ast.h"

// Dont use this, just lols
#define PFN(name) Node* parser##name(void)

#define BIN_EXP(token, function) Node* node = function(); \
    while (lexer->tk == token) { \
        int op = lexer->tk; \
        lexer->match(lexer->tk); \
        Node* right = function(); \
        node = new BinaryOperator(node, right, op); \
    } \
    return node;

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

    Node* parseBlockOrStatement(void);
    Node* parseBlock(void);
    Node* parseStatement(void);

    Node* parseIfElseStatement(void);
    Node* parseWhileStatement(void);

    Node* parseNonControlStatement(void);
    Node* parseAssignStatement(void);

    Node* parseExpression(void);

    Node* parseAssignExpression(void);

    Node* parseLogicalOr(void);
    Node* parseLogicalAnd(void);

    Node* parseBitwiseOr(void);
    Node* parseBitwiseXor(void);
    Node* parseBitwiseAnd(void);

    Node* parseEquality(void);
    Node* parseRelational(void);
    Node* parseShift(void);

    Node* parseAdditive(void);
    Node* parseMultiplicative(void);

    Node* parseElement(void);

    // What in the C++ fuck?
    Node* parseBinaryOperator(const int, Node* (Parser::*)(void));
    Node* parseBinaryOperator(std::vector<int>, Node* (Parser::*)(void));
};

#endif