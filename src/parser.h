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
    virtual std::string asString() = 0;
    virtual std::string asXML() = 0;
};

class BinaryOperator : public AST {
  public:
    BinaryOperator(AST* left, AST* right, int op);

    AST* left;
    AST* right;
    int op;

    std::string asString();
    std::string asXML();
};

class FunctionCall : public AST {
  public:
    FunctionCall(const std::string& callback, const std::string& generic, std::vector<AST*> args);

    std::string callback;
    std::string generic;
    std::vector<AST*> args;

    std::string asString();
    std::string asXML();
};

class NumberLiteral : public AST {
  public:
    NumberLiteral(const std::string& literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class StringLiteral : public AST {
  public:
    StringLiteral(const std::string& literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class BooleanLiteral : public AST {
  public:
    BooleanLiteral(const std::string& literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class VariableIdentifier : public AST {
  public:
    VariableIdentifier(const std::string& name);
    std::string name;

    std::string asString();
    std::string asXML();
};

class TypeIdentifier : public AST {
  public:
    TypeIdentifier(const std::string& name);
    std::string name;

    std::string asString();
    std::string asXML();
};

class VariableAssignment : public AST {
  public:
    VariableAssignment(const std::string& type, const std::string& name, AST* value);

    std::string type;
    std::string name;
    AST* value;

    std::string asString();
    std::string asXML();
};

class VariableReAssignment : public AST {
  public:
    VariableReAssignment(int op, const std::string& name, AST* value);

    int op;
    std::string name;
    AST* value;

    std::string asString();
    std::string asXML();
};

class Block : public AST {
  public:
    Block(std::vector<AST*> statements);

    std::vector<AST*> statements;

    std::string asString();
    std::string asXML();
};

class IfElseStatement : public AST {
  public:
    IfElseStatement(AST* condition, AST* ifBlock, AST* elseBlock);

    AST* condition;
    AST* ifBlock;
    AST* elseBlock;

    std::string asString();
    std::string asXML();
};

class WhileStatement : public AST {
  public:
    WhileStatement(AST* condition, AST* block);

    AST* condition;
    AST* block;

    std::string asString();
    std::string asXML();
};

class Parser {
  private:
    Lexer* lexer;

    AST* factor();
    AST* functionCall();
    AST* functionCall(VariableIdentifier* callback);

    AST* multiplicativeExpression();

    AST* additiveExpression();

    AST* shiftExpression();

    AST* relationalExpression();

    AST* equalityExpression();

    AST* bitwiseAndExpression();
    AST* bitwiseXorExpression();
    AST* bitwiseOrExpression();

    AST* logicalAndExpression();
    AST* logicalOrExpression();

    AST* statement();

    AST* block();

    AST* program();

  public:
    Parser(Lexer* lexer);
    ~Parser();

    AST* parse();
};

#endif