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

class FunctionCall : public AST {
public:
    FunctionCall(std::string callback, std::vector<AST*> args);

    std::string callback;
    std::vector<AST*> args;

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

class TypeIdentifier : public AST {
public:
    TypeIdentifier(std::string name);
    std::string name;

    std::string asString();
};

class VariableAssignment : public AST {
public:
    VariableAssignment(std::string type, std::string name, AST* value);

    std::string type;
    std::string name;
    AST* value;

    std::string asString();
};

class VariableReAssignment : public AST {
public:
    VariableReAssignment(int op, std::string name, AST* value);

    int op;
    std::string name;
    AST* value;

    std::string asString();
};

class Block : public AST {
public:
    Block(std::vector<AST*> statements);

    std::vector<AST*> statements;

    std::string asString();
};

class Parser {
private:
    Lexer* lexer;

    AST* factor();
    AST* functionCall();
    AST* functionCall(VariableIdentifier* callback);
    AST* term();
    AST* expression();
    AST* statement();
    AST* block();

public:
    Parser(Lexer* lexer);
    ~Parser();

    AST* parse();
};


#endif