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
    virtual std::string asString(void) = 0;
    virtual std::string asXML(void) = 0;
    virtual std::string asJSON(void) = 0;
};

class BinaryOperator : public AST {
  public:
    BinaryOperator(AST* left, AST* right, int op);

    AST* left;
    AST* right;
    int op;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class FunctionCall : public AST {
  public:
    FunctionCall(const std::string& callback, const std::string& generic, std::vector<AST*> args);

    std::string callback;
    std::string generic;
    std::vector<AST*> args;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class NumberLiteral : public AST {
  public:
    NumberLiteral(const std::string& literal);
    std::string literal;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class StringLiteral : public AST {
  public:
    StringLiteral(const std::string& literal);
    std::string literal;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class BooleanLiteral : public AST {
  public:
    BooleanLiteral(const std::string& literal);
    std::string literal;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class VariableIdentifier : public AST {
  public:
    VariableIdentifier(const std::string& name);
    std::string name;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class TypeIdentifier : public AST {
  public:
    TypeIdentifier(const std::string& name);
    std::string name;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class VariableAssignment : public AST {
  public:
    VariableAssignment(const std::string& type, const std::string& name, AST* value);

    std::string type;
    std::string name;
    AST* value;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class VariableReAssignment : public AST {
  public:
    VariableReAssignment(int op, const std::string& name, AST* value);

    int op;
    std::string name;
    AST* value;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class Block : public AST {
  public:
    Block(std::vector<AST*> statements);

    std::vector<AST*> statements;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class IfElseStatement : public AST {
  public:
    IfElseStatement(AST* condition, AST* ifBlock, AST* elseBlock);

    AST* condition;
    AST* ifBlock;
    AST* elseBlock;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class WhileStatement : public AST {
  public:
    WhileStatement(AST* condition, AST* block);

    AST* condition;
    AST* block;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class ParameterDeclaration : public AST {
  public:
    ParameterDeclaration(const std::string& type, const std::string& name);

    std::string type;
    std::string name;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class FunctionDeclaration : public AST {
  public:
    FunctionDeclaration(const std::string& name, const std::string& generic, std::vector<AST*> args, AST* block, const std::string& returns);

    std::string name;
    std::string generic;
    std::vector<AST*> args;
    AST* block;
    std::string returns;

    std::string asString(void);
    std::string asXML(void);
    std::string asJSON(void);
};

class Parser {
  private:
    Lexer* lexer;

    AST* factor(void);
    AST* functionCall(void);
    AST* functionCall(VariableIdentifier* callback);

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