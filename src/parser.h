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
    FunctionCall(std::string callback, std::vector<AST*> args);

    std::string callback;
    std::vector<AST*> args;

    std::string asString();
    std::string asXML();

};

class NumberLiteral : public AST {
public:
    NumberLiteral(std::string literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class StringLiteral : public AST {
public:
    StringLiteral(std::string literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class BooleanLiteral : public AST {
public:
    BooleanLiteral(std::string literal);
    std::string literal;

    std::string asString();
    std::string asXML();
};

class VariableIdentifier : public AST {
public:
    VariableIdentifier(std::string name);
    std::string name;

    std::string asString();
    std::string asXML();
};

class TypeIdentifier : public AST {
public:
    TypeIdentifier(std::string name);
    std::string name;

    std::string asString();
    std::string asXML();
};

class VariableAssignment : public AST {
public:
    VariableAssignment(std::string type, std::string name, AST* value);

    std::string type;
    std::string name;
    AST* value;

    std::string asString();
    std::string asXML();
};

class VariableReAssignment : public AST {
public:
    VariableReAssignment(int op, std::string name, AST* value);

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

public:
    Parser(Lexer* lexer);
    ~Parser();

    AST* parse();
};


#endif