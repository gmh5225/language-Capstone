#ifndef CAPSTONE_AST
#define CAPSTONE_AST

// Generated by ./scripts/ast_gen.py

#include "common.h"
#include "lexer.h"

class Node {
  public:
    virtual std::string toJSON() = 0;
};

static std::string createList(const std::vector<Node*>&);
static std::string safeLiterals(const std::string& str);
static std::string nullSafeToString(Node*);


class UnaryOperator : public Node {
  public:              
    Node* element;
    int op;
     
    UnaryOperator(Node* element, int op) : element(element), op(op) {}
    std::string toJSON(void);
};

class BinaryOperator : public Node {
  public:              
    Node* left;
    Node* right;
    int op;
     
    BinaryOperator(Node* left, Node* right, int op) : left(left), right(right), op(op) {}
    std::string toJSON(void);
};

class FunctionCall : public Node {
  public:              
    Node* callback;
    Node* generic;
    std::vector<Node*> params;
     
    FunctionCall(Node* callback, Node* generic, std::vector<Node*> params) : callback(callback), generic(generic), params(params) {}
    std::string toJSON(void);
};

class NumberLiteral : public Node {
  public:              
    std::string literal;
     
    NumberLiteral(const std::string& literal) : literal(literal) {}
    std::string toJSON(void);
};

class StringLiteral : public Node {
  public:              
    std::string literal;
     
    StringLiteral(const std::string& literal) : literal(literal) {}
    std::string toJSON(void);
};

class BooleanLiteral : public Node {
  public:              
    std::string literal;
     
    BooleanLiteral(const std::string& literal) : literal(literal) {}
    std::string toJSON(void);
};

class NullLiteral : public Node {
  public:              
     
    NullLiteral() {}
    std::string toJSON(void);
};

class ArrayLiteral : public Node {
  public:              
    std::vector<Node*> literal;
     
    ArrayLiteral(std::vector<Node*> literal) : literal(literal) {}
    std::string toJSON(void);
};

class VariableIdentifier : public Node {
  public:              
    Node* child;
    std::string name;
     
    VariableIdentifier(Node* child, const std::string& name) : child(child), name(name) {}
    std::string toJSON(void);
};

class TypeIdentifier : public Node {
  public:              
    std::vector<Node*> children;
    std::string name;
    unsigned int list;
    unsigned int final;
     
    TypeIdentifier(std::vector<Node*> children, const std::string& name, unsigned int list, unsigned int final) : children(children), name(name), list(list), final(final) {}
    std::string toJSON(void);
};

class VariableDeclaration : public Node {
  public:              
    Node* type;
    Node* name;
    Node* value;
     
    VariableDeclaration(Node* type, Node* name, Node* value) : type(type), name(name), value(value) {}
    std::string toJSON(void);
};

class ExpressionStatement : public Node {
  public:              
    Node* expression;
     
    ExpressionStatement(Node* expression) : expression(expression) {}
    std::string toJSON(void);
};

class Block : public Node {
  public:              
    std::vector<Node*> statements;
     
    Block(std::vector<Node*> statements) : statements(statements) {}
    std::string toJSON(void);
};

class IfElseStatement : public Node {
  public:              
    Node* condition;
    Node* ifBlock;
    Node* elseBlock;
     
    IfElseStatement(Node* condition, Node* ifBlock, Node* elseBlock) : condition(condition), ifBlock(ifBlock), elseBlock(elseBlock) {}
    std::string toJSON(void);
};

class WhileStatement : public Node {
  public:              
    Node* condition;
    Node* block;
     
    WhileStatement(Node* condition, Node* block) : condition(condition), block(block) {}
    std::string toJSON(void);
};

class ForStatement : public Node {
  public:              
    Node* init;
    Node* condition;
    Node* post;
    Node* block;
     
    ForStatement(Node* init, Node* condition, Node* post, Node* block) : init(init), condition(condition), post(post), block(block) {}
    std::string toJSON(void);
};

class ParameterDeclaration : public Node {
  public:              
    Node* type;
    Node* name;
     
    ParameterDeclaration(Node* type, Node* name) : type(type), name(name) {}
    std::string toJSON(void);
};

class FunctionDeclaration : public Node {
  public:              
    Node* name;
    Node* generic;
    std::vector<Node*> params;
    std::vector<Node*> returns;
    Node* block;
     
    FunctionDeclaration(Node* name, Node* generic, std::vector<Node*> params, std::vector<Node*> returns, Node* block) : name(name), generic(generic), params(params), returns(returns), block(block) {}
    std::string toJSON(void);
};

class BreakStatement : public Node {
  public:              
     
    BreakStatement() {}
    std::string toJSON(void);
};

class ContinueStatement : public Node {
  public:              
     
    ContinueStatement() {}
    std::string toJSON(void);
};

class ReturnStatement : public Node {
  public:              
    std::vector<Node*> expressions;
     
    ReturnStatement(std::vector<Node*> expressions) : expressions(expressions) {}
    std::string toJSON(void);
};

class ImportStatement : public Node {
  public:              
    Node* package;
     
    ImportStatement(Node* package) : package(package) {}
    std::string toJSON(void);
};

class TernaryExpression : public Node {
  public:              
    Node* condition;
    Node* ifExpression;
    Node* elseExpression;
     
    TernaryExpression(Node* condition, Node* ifExpression, Node* elseExpression) : condition(condition), ifExpression(ifExpression), elseExpression(elseExpression) {}
    std::string toJSON(void);
};

class ClassDeclaration : public Node {
  public:              
    Node* name;
    Node* super;
    Node* body;
     
    ClassDeclaration(Node* name, Node* super, Node* body) : name(name), super(super), body(body) {}
    std::string toJSON(void);
};

class ClassVisibility : public Node {
  public:              
    Node* member;
    unsigned int visibility;
     
    ClassVisibility(Node* member, unsigned int visibility) : member(member), visibility(visibility) {}
    std::string toJSON(void);
};

#endif
