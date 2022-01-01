// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#ifndef BOUNCE_PARSER
#define BOUNCE_PARSER

#include "common.h"

#include "lexer.h"

class AST {
  public:
    virtual std::string asJSON(void) = 0;

    //virtual std::string codeGenC(void) = 0;
};

class BinaryOperator : public AST {
  public:
    BinaryOperator(AST* left, AST* right, int op);

    AST* left;
    AST* right;
    int op;

    std::string asJSON(void);
};

class FunctionCall : public AST {
  public:
    FunctionCall(AST* callback, AST* generic, std::vector<AST*> args);

    AST* callback;
    AST* generic;
    std::vector<AST*> args;

    std::string asJSON(void);
};

class NumberLiteral : public AST {
  public:
    NumberLiteral(const std::string& literal);
    std::string literal;

    std::string asJSON(void);
};

class StringLiteral : public AST {
  public:
    StringLiteral(const std::string& literal);
    std::string literal;

    std::string asJSON(void);
};

class BooleanLiteral : public AST {
  public:
    BooleanLiteral(const std::string& literal);
    std::string literal;

    std::string asJSON(void);
};

class VariableIdentifier : public AST {
  public:
    VariableIdentifier(const std::string& name, AST* child);
    AST* child;
    std::string name;

    std::string asJSON(void);
};

class TypeIdentifier : public AST {
  public:
    TypeIdentifier(const std::string& name);
    std::string name;

    std::string asJSON(void);
};

class VariableAssignment : public AST {
  public:
    VariableAssignment(AST* type, AST* name, AST* value);

    AST* type;
    AST* name;
    AST* value;

    std::string asJSON(void);
};

class VariableReAssignment : public AST {
  public:
    VariableReAssignment(int op, AST* name, AST* value);

    int op;
    AST* name;
    AST* value;

    std::string asJSON(void);
};

class Block : public AST {
  public:
    Block(std::vector<AST*> statements);

    std::vector<AST*> statements;

    std::string asJSON(void);
};

class IfElseStatement : public AST {
  public:
    IfElseStatement(AST* condition, AST* ifBlock, AST* elseBlock);

    AST* condition;
    AST* ifBlock;
    AST* elseBlock;

    std::string asJSON(void);
};

class WhileStatement : public AST {
  public:
    WhileStatement(AST* condition, AST* block);

    AST* condition;
    AST* block;

    std::string asJSON(void);
};

class ParameterDeclaration : public AST {
  public:
    ParameterDeclaration(AST* type, AST* name);

    AST* type;
    AST* name;

    std::string asJSON(void);
};

class FunctionDeclaration : public AST {
  public:
    FunctionDeclaration(AST* name, AST* generic, std::vector<AST*> args, AST* block, AST* returns);

    AST* name;
    AST* generic;
    std::vector<AST*> args;
    AST* block;
    AST* returns;

    std::string asJSON(void);
};

class Parser {
  private:
    Lexer* lexer;

    AST* variableIdentifier(void);
    AST* factor(void);
    AST* functionCall(void);
    AST* functionCall(AST* callback);

    AST* multiplicativeExpression(void);

    AST* additiveExpression(void);

    AST* shiftExpression(void);

    AST* relationalExpression(void);

    AST* equalityExpression(void);

    AST* bitwiseAndExpression(void);
    AST* bitwiseXorExpression(void);
    AST* bitwiseOrExpression(void);

    AST* logicalAndExpression(void);
    AST* logicalOrExpression(void);

    AST* statement(void);

    AST* block(void);

    AST* parameterDeclaration(void);
    AST* functionDeclaration(void);

    AST* program(void);

  public:
    Parser(Lexer* lexer);
    ~Parser(void);

    AST* parse(void);
};

#endif