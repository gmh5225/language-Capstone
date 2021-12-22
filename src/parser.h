#ifndef BOUNCE_PARSER
#define BOUNCE_PARSER

#include "common.h"

#include "lexer.h"

// class Statement {
// }

// class Expression {
// }

class AST {
public:
    virtual std::string asString() = 0;
};

class BinaryOperator : public AST {
public:
    BinaryOperator(AST* left, AST* right, int op);

    AST* left;
    AST* right;
    int op;

    std::string asString();
};

class NumberLiteral : public AST {
public:
    NumberLiteral(std::string literal);
    std::string literal;

    std::string asString();
};

class VariableIdentifier : public AST {
public:
    VariableIdentifier(std::string name);
    std::string name;

    std::string asString();
};

class Parser {
private:
    Lexer* lexer;

    AST* factor();
    AST* term();
    AST* expression();

public:
    Parser(Lexer* lexer);
    ~Parser();

    AST* parse();
};


#endif