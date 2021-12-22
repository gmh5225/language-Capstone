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

TypeIdentifier::TypeIdentifier(std::string name) {
    this->name = name;
}

std::string TypeIdentifier::asString() {
    return "TypeIdentifier〈" + name + "〉";
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

FunctionCall::FunctionCall(std::string callback, std::vector<AST*> args) {
    this->callback = callback;
    this->args = args;
}

std::string FunctionCall::asString() {
    std::string rep = "FunctionCall〈" + callback + " Args〈";
    for (AST* arg : args)
        rep += arg->asString() + " ";
    return rep + "〉〉";
}

VariableAssignment::VariableAssignment(std::string type, std::string name, AST* value) {
    this->type = type;
    this->name = name;
    this->value = value;
}

std::string VariableAssignment::asString() {
    return "VariableAssignment〈" + type + " " + name + " " + value->asString() + "〉";
}

VariableReAssignment::VariableReAssignment(int op, std::string name, AST* value) {
    this->op = op;
    this->name = name;
    this->value = value;
}

std::string VariableReAssignment::asString() {
    return "VariableReAssignment〈" + name + " " + Lexer::getTokenStr(op) 
            + " " + value->asString() + "〉";
}

Block::Block(std::vector<AST*> statements) {
    this->statements = statements;
}

std::string Block::asString() {
    std::string rep = "Block 〈\n";
    for (AST* statement : this->statements) 
        rep += "  " + statement->asString() + " ;\n";
    return rep + "〉";
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
        if (lexer->tk == '(')
            return functionCall(id);
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

AST* Parser::functionCall(VariableIdentifier* callback) {
    std::vector<AST*> args;
    lexer->match('(');
    if (lexer->tk != ')') {
        args.push_back(expression());
        while (lexer->tk == ',') {
            lexer->match(',');
            args.push_back(expression());
        }
    }
    lexer->match(')');
    return new FunctionCall(callback->name, args);
}

AST* Parser::functionCall() {
    std::string callback = lexer->tkStr;
    lexer->match(TOK_ID);
    lexer->match('(');
    std::vector<AST*> args;
    if (lexer->tk != ')') {
        args.push_back(expression());
        while (lexer->tk == ',') {
            lexer->match(',');
            args.push_back(expression());
        }
    }
    lexer->match(')');
    return new FunctionCall(callback, args);
}

AST* Parser::term() {
    AST* node = factor();
    while (lexer->tk == '*' || lexer->tk == '/' || lexer->tk == '%') {
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

AST* Parser::statement() {
    if (lexer->tk != TOK_ID)
        throw new Exception("Expected some sort of identifier");

    VariableIdentifier* node = new VariableIdentifier(lexer->tkStr);
    lexer->match(lexer->tk);

    if (lexer->tk == '(')
        return functionCall(node);

    if (lexer->tk == '=' 
        || lexer->tk == TOK_PLUSEQUAL 
        || lexer->tk == TOK_MINUSEQUAL
        || lexer->tk == TOK_TIMESEQUAL
        || lexer->tk == TOK_DIVIDEEQUAL
        || lexer->tk == TOK_MODEQUAL
    ) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        return new VariableReAssignment(op, node->name, expression()); 
    } else if (lexer->tk == TOK_ID) {
        std::string name = lexer->tkStr; 
        lexer->match(lexer->tk);
        lexer->match('=');
        return new VariableAssignment(node->name, name, expression());
    }
}

AST* Parser::block() {
    std::vector<AST*> statements;
    while (lexer->tk != TOK_EOF) {
        statements.push_back(statement());
        lexer->match(';');
    }
    return new Block(statements);
}

AST* Parser::parse() {
    return block();
}
