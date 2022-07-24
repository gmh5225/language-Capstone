#ifndef CAPSTONE_AST
#define CAPSTONE_AST

#include "common.h"
#include "lexer.h"

class Node {
  public:
    virtual std::string toJSON() = 0;
};

class BinaryOperator : public Node {
  public:              
    Node* left;
    Node* right;
    int op;
     
    BinaryOperator(Node* left, Node* right, int op) : left(left), right(right), op(op) {}
    std::string toJSON(void);
};

class NumberLiteral : public Node {
  public:              
    std::string literal;
     
    NumberLiteral(const std::string& literal) : literal(literal) {}
    std::string toJSON(void);
};

#endif
