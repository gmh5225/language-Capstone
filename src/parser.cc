#include "parser.h"

NumberLiteral::NumberLiteral(std::string literal) {
    this->literal = literal;
}

std::string NumberLiteral::asString() {
    return "NumberLiteral〈" + literal + "〉";
}

VariableIdentifier::VariableIdentifier(std::string name) {
    this->name = name;
}

std::string VariableIdentifier::asString() {
    return "VariableIdentifier〈" + name + "〉";
}

BinaryOperator::BinaryOperator(AST* left, AST* right, int op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

std::string BinaryOperator::asString() {
    return "BinaryOperation〈" + left->asString() + " " 
            + Lexer::getTokenStr(op) + " " 
            + right->asString() + "〉";
}

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
}

AST* Parser::factor() {
    if (lexer->tk == TOK_INT) {
        NumberLiteral* num = new NumberLiteral(lexer->tkStr);
        lexer->match(TOK_INT);
        return num;
    } else if (lexer->tk == TOK_FLOAT) {
        NumberLiteral* num = new NumberLiteral(lexer->tkStr);
        lexer->match(TOK_FLOAT);
        return num;
    } else if (lexer->tk == TOK_ID) {
        VariableIdentifier* id = new VariableIdentifier(lexer->tkStr);
        lexer->match(TOK_ID);
        return id;
    } else if (lexer->tk == '(') {
        lexer->match('(');
        AST* expr = expression();
        lexer->match(')');
        return expr;
    } else {
        throw new Exception("Expected a number or a parenthesized expression");
    }
}

AST* Parser::term() {
    AST* node = factor();
    while (lexer->tk == '*' || lexer->tk == '/') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = factor();
        node = new BinaryOperator(node, right, op); 
    }
    return node;
}

AST* Parser::expression() {
    AST* node = term();
    while (lexer->tk == '+' || lexer->tk == '-') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = term();
        node = new BinaryOperator(node, right, op); 
    }
    return node;
}

AST* Parser::parse() {
    AST* node = expression();
    if (lexer->tk != TOK_EOF)
        throw new Exception("Expected EOF");
    return node;
}