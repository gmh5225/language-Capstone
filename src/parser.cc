// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#include "parser.h"

NumberLiteral::NumberLiteral(std::string literal) {
    this->literal = literal;
}

std::string NumberLiteral::asString() {
    return "NumberLiteral〈" + literal + "〉";
}

std::string NumberLiteral::asXML() {
    return "<NumberLiteral>" + literal + "</NumberLiteral>";
}

StringLiteral::StringLiteral(std::string literal) {
    this->literal = literal;
}

std::string StringLiteral::asString() {
    return "StringLiteral〈" + safeLiterals(literal) + "〉";
}

std::string StringLiteral::asXML() {
    return "<StringLiteral>" + safeLiterals(literal) + "</StringLiteral>";
}

BooleanLiteral::BooleanLiteral(std::string literal) {
    this->literal = literal;
}

std::string BooleanLiteral::asString() {
    return "BooleanLiteral〈" + safeLiterals(literal) + "〉";
}

std::string BooleanLiteral::asXML() {
    return "<BooleanLiteral>" + safeLiterals(literal) + "</BooleanLiteral>";
}

VariableIdentifier::VariableIdentifier(std::string name) {
    this->name = name;
}

std::string VariableIdentifier::asString() {
    return "VariableIdentifier〈" + name + "〉";
}

std::string VariableIdentifier::asXML() {
    return "<VariableIdentifier>" + name + "</VariableIdentifier>";
}

TypeIdentifier::TypeIdentifier(std::string name) {
    this->name = name;
}

std::string TypeIdentifier::asString() {
    return "TypeIdentifier〈" + name + "〉";
}

std::string TypeIdentifier::asXML() {
    return "<TypeIdentifier>" + name + "</TypeIdentifier>";
}

BinaryOperator::BinaryOperator(AST* left, AST* right, int op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

std::string BinaryOperator::asString() {
    return "BinaryOperation〈" + left->asString() + " " +
           Lexer::getTokenStr(op) + " " + right->asString() + "〉";
}

std::string BinaryOperator::asXML() {
    return "<BinaryOperation>" + left->asXML() + "<Operator>" +
           Lexer::getTokenStr(op) + "</Operator>" + right->asXML() +
           "</BinaryOperation>";
}

FunctionCall::FunctionCall(std::string callback, std::vector<AST*> args) {
    this->callback = callback;
    this->args = args;
}

std::string FunctionCall::asString() {
    std::string rep = "FunctionCall〈" + callback + " Args〈";
    for (AST* arg : args) rep += arg->asString() + " ";
    return rep + "〉〉";
}

std::string FunctionCall::asXML() {
    std::string rep =
            "<FunctionCall><Callback>" + callback + "</Callback><Args>";
    for (AST* arg : args) rep += "<Arg>" + arg->asXML() + "</Arg>";
    return rep + "</Args></FunctionCall>";
}

VariableAssignment::VariableAssignment(std::string type, std::string name,
                                       AST* value) {
    this->type = type;
    this->name = name;
    this->value = value;
}

std::string VariableAssignment::asString() {
    return "VariableAssignment〈" + type + " " + name + " " +
           value->asString() + "〉";
}

std::string VariableAssignment::asXML() {
    return "<VariableAssignment><Type>" + type + "</Type><Name>" + name +
           "</Name><Operator>'='</Operator>" + value->asXML() +
           "</VariableAssignment>";
}

VariableReAssignment::VariableReAssignment(int op, std::string name,
                                           AST* value) {
    this->op = op;
    this->name = name;
    this->value = value;
}

std::string VariableReAssignment::asString() {
    return "VariableReAssignment〈" + name + " " + Lexer::getTokenStr(op) +
           " " + value->asString() + "〉";
}

std::string VariableReAssignment::asXML() {
    return "<VariableReAssignment><Name>" + name + "</Name><Operator>" +
           Lexer::getTokenStr(op) + "</Operator>" + value->asXML() +
           "</VariableReAssignment>";
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

std::string Block::asXML() {
    std::string rep = "<Block>";
    for (AST* statement : this->statements) rep += statement->asXML();
    return rep + "</Block>";
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
    } else if (lexer->tk == TOK_STR) {
        StringLiteral* str = new StringLiteral(lexer->tkStr);
        lexer->match(TOK_STR);
        return str;
    } else if (lexer->tk == TOK_R_TRUE) {
        BooleanLiteral* boolean = new BooleanLiteral("1");
        lexer->match(TOK_R_TRUE);
        return boolean;
    } else if (lexer->tk == TOK_R_FALSE) {
        BooleanLiteral* boolean = new BooleanLiteral("0");
        lexer->match(TOK_R_FALSE);
        return boolean;
    } else if (lexer->tk == TOK_ID) {
        VariableIdentifier* id = new VariableIdentifier(lexer->tkStr);
        lexer->match(TOK_ID);
        if (lexer->tk == '(') return functionCall(id);
        return id;
    } else if (lexer->tk == '(') {
        lexer->match('(');
        AST* expr = logicalOrExpression();
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
        args.push_back(logicalOrExpression());
        while (lexer->tk == ',') {
            lexer->match(',');
            args.push_back(logicalOrExpression());
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
        args.push_back(logicalOrExpression());
        while (lexer->tk == ',') {
            lexer->match(',');
            args.push_back(logicalOrExpression());
        }
    }
    lexer->match(')');
    return new FunctionCall(callback, args);
}

// AST* Parser::unaryExpression() {}

AST* Parser::multiplicativeExpression() {
    AST* node = factor();
    while (lexer->tk == '*' || lexer->tk == '/' || lexer->tk == '%') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = factor();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::additiveExpression() {
    AST* node = multiplicativeExpression();
    while (lexer->tk == '+' || lexer->tk == '-') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = multiplicativeExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::shiftExpression() {
    AST* node = additiveExpression();
    while (lexer->tk == TOK_RSHIFT || lexer->tk == TOK_LSHIFT) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = additiveExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::relationalExpression() {
    AST* node = shiftExpression();
    while (lexer->tk == TOK_SPACESHIP || lexer->tk == '>' || lexer->tk == '<' ||
           lexer->tk == TOK_GEQUAL || lexer->tk == TOK_LEQUAL) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = shiftExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::equalityExpression() {
    AST* node = relationalExpression();
    while (lexer->tk == TOK_EQUAL || lexer->tk == TOK_NEQUAL) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = relationalExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseAndExpression() {
    AST* node = equalityExpression();
    while (lexer->tk == '&') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = equalityExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseXorExpression() {
    AST* node = bitwiseAndExpression();
    while (lexer->tk == '^') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseAndExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseOrExpression() {
    AST* node = bitwiseXorExpression();
    while (lexer->tk == '|') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseXorExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::logicalAndExpression() {
    AST* node = bitwiseOrExpression();
    while (lexer->tk == TOK_ANDAND) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseOrExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::logicalOrExpression() {
    AST* node = logicalAndExpression();
    while (lexer->tk == TOK_OROR) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = logicalAndExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::statement() {
    if (lexer->tk != TOK_ID) {
        throw new Exception("Expected some sort of identifier");
        return (AST*)NULL;
    }

    VariableIdentifier* node = new VariableIdentifier(lexer->tkStr);
    lexer->match(lexer->tk);

    if (lexer->tk == '(') return functionCall(node);

    if (lexer->tk == '=' || lexer->tk == TOK_PLUSEQUAL ||
        lexer->tk == TOK_MINUSEQUAL || lexer->tk == TOK_TIMESEQUAL ||
        lexer->tk == TOK_DIVIDEEQUAL || lexer->tk == TOK_MODEQUAL ||
        lexer->tk == TOK_LSHIFTEQUAL || lexer->tk == TOK_RSHIFTEQUAL ||
        lexer->tk == TOK_ANDEQUAL || lexer->tk == TOK_OREQUAL ||
        lexer->tk == TOK_XOREQUAL) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        return new VariableReAssignment(op, node->name, logicalOrExpression());
    } else if (lexer->tk == TOK_ID) {
        std::string name = lexer->tkStr;
        lexer->match(lexer->tk);
        lexer->match('=');
        return new VariableAssignment(node->name, name, logicalOrExpression());
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
