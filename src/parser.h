/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#ifndef CAPSTONE_PARSER
#define CAPSTONE_PARSER

#include "ast.h"
#include "common.h"
#include "lexer.h"

class Parser {
  public:
    Parser(Lexer* lexer) : lexer(lexer) {
    }
    Node* parse(void);

  private:
    Lexer* lexer;

    Node* parseFile(void);
    Node* parseGlobalScope(void);

    Node* parseImport(void);

    Node* parseEnumDecl(void);

    Node* parseClassDecl(void);
    Node* parseClassBody(void);
    Node* parseClassField(void);
    Node* parseClassMember(void);

    Node* parseFuncDecl(void);
    Node* parseVarIdent(void);
    Node* parseTypeIdent(void);
    Node* parseParamDecl(void);

    Node* parseBlockOrStatement(void);
    Node* parseBlock(void);
    Node* parseStatement(void);

    Node* parseIfElseStatement(void);
    Node* parseWhileStatement(void);
    Node* parseForStatement(void);

    Node* parseBreakStatement(void);
    Node* parseContinueStatement(void);
    Node* parseReturnStatement(void);

    Node* parseExpressionStatement(void);

    Node* parseVarDecl(void);

    Node* parseExpression(void);

    Node* parseTernaryExpression(void);

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

    Node* parseUnaryExpression(void);

    Node* parseElement(void);

    // What in the C++ fuck?
    Node* parseBinaryOperator(const int, Node* (Parser::*)(void));
    Node* parseBinaryOperator(std::vector<int>, Node* (Parser::*)(void));
};

#endif