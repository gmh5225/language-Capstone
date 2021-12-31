// Copyright (C) 2021 Justus Languell - All Rights Reserved.
//
// This file is part of Bounce which is released under the MIT license.
// See file LICENSE or go write <jus@gtsbr.org> for full license details.

#include "parser.h"

NumberLiteral::NumberLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string NumberLiteral::asString(void) {
    return "NumberLiteral〈" + literal + "〉";
}

std::string NumberLiteral::asXML(void) {
    return "<NumberLiteral>" + literal + "</NumberLiteral>";
}

std::string NumberLiteral::asJSON(void) {
    return "{\"*\":\"NumberLiteral\",\"literal\":\"" + literal + "\"}";
}

StringLiteral::StringLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string StringLiteral::asString(void) {
    return "StringLiteral〈" + safeLiterals(literal) + "〉";
}

std::string StringLiteral::asXML(void) {
    return "<StringLiteral>" + safeLiterals(literal) + "</StringLiteral>";
}

std::string StringLiteral::asJSON(void) {
    return "{\"*\":\"StringLiteral\",\"literal\":\"" + safeLiterals(literal) + "\"}";
}

BooleanLiteral::BooleanLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string BooleanLiteral::asString(void) {
    return "BooleanLiteral〈" + safeLiterals(literal) + "〉";
}

std::string BooleanLiteral::asXML(void) {
    return "<BooleanLiteral>" + safeLiterals(literal) + "</BooleanLiteral>";
}

std::string BooleanLiteral::asJSON(void) {
    return "{\"*\":\"BooleanLiteral\",\"literal\":\"" + safeLiterals(literal) + "\"}";
}

VariableIdentifier::VariableIdentifier(const std::string& name) {
    this->name = name;
}

std::string VariableIdentifier::asString(void) {
    return "VariableIdentifier〈" + name + "〉";
}

std::string VariableIdentifier::asXML(void) {
    return "<VariableIdentifier>" + name + "</VariableIdentifier>";
}

std::string VariableIdentifier::asJSON(void) {
    return "{\"*\":\"VariableIdentifier\",\"identifier\":\"" + name + "\"}";
}

TypeIdentifier::TypeIdentifier(const std::string& name) {
    this->name = name;
}

std::string TypeIdentifier::asString(void) {
    return "TypeIdentifier〈" + name + "〉";
}

std::string TypeIdentifier::asXML(void) {
    return "<TypeIdentifier>" + name + "</TypeIdentifier>";
}

std::string TypeIdentifier::asJSON(void) {
    return "{\"*\":\"TypeIdentifier\",\"identifier\":\"" + name + "\"}";
}

BinaryOperator::BinaryOperator(AST* left, AST* right, int op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

std::string BinaryOperator::asString(void) {
    return "BinaryOperation〈" + left->asString() + " " +
           Lexer::getTokenStr(op) + " " + right->asString() + "〉";
}

std::string BinaryOperator::asXML(void) {
    return "<BinaryOperation>" + left->asXML() + "<Operator>" +
           Lexer::getTokenStr(op) + "</Operator>" + right->asXML() +
           "</BinaryOperation>";
}

std::string BinaryOperator::asJSON(void) {
    return "{\"*\":\"BinaryOperation\",\"left\":" + left->asJSON() +
           ",\"operator\":\"" + Lexer::getTokenStr(op) + "\",\"right\":" +
           right->asJSON() + "}";
}

FunctionCall::FunctionCall(const std::string& callback, const std::string& generic, std::vector<AST*> args) {
    this->callback = callback;
    this->generic = generic;
    this->args = args;
}

// Not updated
std::string FunctionCall::asString(void) {
    std::string rep = "FunctionCall〈" + callback + " Args〈";
    for (AST* arg : args) rep += arg->asString() + " ";
    return rep + "〉〉";
}

std::string FunctionCall::asXML(void) {
    generic = (this->generic != "") ? "<Generic>" + this->generic + "</Generic>" : "";
    std::string rep =
            "<FunctionCall><Callback>" + callback + "</Callback>" + generic + "<Args>";
    for (AST* arg : args) rep += "<Arg>" + arg->asXML() + "</Arg>";
    return rep + "</Args></FunctionCall>";
}

std::string FunctionCall::asJSON(void) {
    generic = (this->generic != "") ? "\"generic\":\"" + this->generic + "\"" : "";
    std::string rep =
            "{\"*\":\"FunctionCall\",\"callback\":\"" + callback + "\",\"args\":[";
    for (AST* arg : args) rep += arg->asJSON() + ",";
    return rep + "]}";
}

VariableAssignment::VariableAssignment(const std::string& type, const std::string& name,
                                       AST* value) {
    this->type = type;
    this->name = name;
    this->value = value;
}

std::string VariableAssignment::asString(void) {
    return "VariableAssignment〈" + type + " " + name + " " +
           value->asString() + "〉";
}

std::string VariableAssignment::asXML(void) {
    return "<VariableAssignment><Type>" + type + "</Type><Name>" + name +
           "</Name><Operator>'='</Operator>" + value->asXML() +
           "</VariableAssignment>";
}

std::string VariableAssignment::asJSON(void) {
    return "{\"*\":\"VariableAssignment\",\"type\":\"" + type + "\",\"name\":\"" +
           name + "\",\"value\":" + value->asJSON() + "}";
}

VariableReAssignment::VariableReAssignment(int op, const std::string& name,
                                           AST* value) {
    this->op = op;
    this->name = name;
    this->value = value;
}

std::string VariableReAssignment::asString(void) {
    return "VariableReAssignment〈" + name + " " + Lexer::getTokenStr(op) +
           " " + value->asString() + "〉";
}

std::string VariableReAssignment::asXML(void) {
    return "<VariableReAssignment><Name>" + name + "</Name><Operator>" +
           Lexer::getTokenStr(op) + "</Operator>" + value->asXML() +
           "</VariableReAssignment>";
}

std::string VariableReAssignment::asJSON(void) {
    return "{\"*\":\"VariableReAssignment\",\"name\":\"" + name + "\",\"value\":" +
           value->asJSON() + "}";
}

Block::Block(std::vector<AST*> statements) {
    this->statements = statements;
}

std::string Block::asString(void) {
    std::string rep = "Block 〈\n";
    for (AST* statement : this->statements)
        rep += "  " + statement->asString() + " ;\n";
    return rep + "〉";
}

std::string Block::asXML(void) {
    std::string rep = "<Block>";
    for (AST* statement : this->statements) rep += statement->asXML();
    return rep + "</Block>";
}

std::string Block::asJSON(void) {
    std::string rep = "{\"*\":\"Block\",\"statements\":[";
    for (AST* statement : this->statements) rep += statement->asJSON() + ",";
    return rep + "]}";
}

IfElseStatement::IfElseStatement(AST* condition, AST* ifBlock, AST* elseBlock) {
    this->condition = condition;
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
}

std::string IfElseStatement::asString(void) {
    return "asString() method is not implemented for IfElseStatement";
}

std::string IfElseStatement::asXML(void) {
    std::string rep = "<IfElseStatement>";
    rep += "<Condition>" + condition->asXML() + "</Condition>";
    rep += "<IfBlock>" + ifBlock->asXML() + "</IfBlock>";
    if (elseBlock != nullptr) rep += "<ElseBlock>" + elseBlock->asXML() + "</ElseBlock>";
    return rep + "</IfElseStatement>";
}

std::string IfElseStatement::asJSON(void) {
    std::string rep = "{\"*\":\"IfElseStatement\",\"condition\":" +
                      condition->asJSON() + ",\"ifBlock\":" + ifBlock->asJSON();
    if (elseBlock != nullptr) rep += ",\"elseBlock\":" + elseBlock->asJSON();
    return rep + "}";
}

WhileStatement::WhileStatement(AST* condition, AST* block) {
    this->condition = condition;
    this->block = block;
}

std::string WhileStatement::asString(void) {
    return "asString() method is not implemented for WhileStatement";
}

std::string WhileStatement::asXML(void) {
    return "<WhileStatement><Condition>" + condition->asXML() + "</Condition>" + block->asXML() + "</WhileStatement>";
}

std::string WhileStatement::asJSON(void) {
    return "{\"*\":\"WhileStatement\",\"condition\":" + condition->asJSON() + ",\"block\":" + block->asJSON() + "}";
}

ParameterDeclaration::ParameterDeclaration(const std::string& type, const std::string& name) {
    this->type = type;
    this->name = name;
}

std::string ParameterDeclaration::asString() {
    return "ParameterDeclaration〈" + type + " " + name + "〉";
}

std::string ParameterDeclaration::asXML() {
    return "<ParameterDeclaration><Type>" + type + "</Type><Name>" + name + "</Name></ParameterDeclaration>";
}

std::string ParameterDeclaration::asJSON() {
    return "{\"*\":\"ParameterDeclaration\",\"type\":\"" + type + "\",\"name\":\"" + name + "\"}";
}

FunctionDeclaration::FunctionDeclaration(const std::string& name, const std::string& generic, std::vector<AST*> args, AST* block, const std::string& returns) {
    this->name = name;
    this->generic = generic;
    this->args = args;
    this->block = block;
    this->returns = returns;
}

std::string FunctionDeclaration::asString() {
    std::string rep = "FunctionDeclaration〈" + name + "(";
    for (AST* arg : args) rep += arg->asString() + ",";
    return rep + ")" + returns + "〉";
}

std::string FunctionDeclaration::asXML() {
    std::string rep = "<FunctionDeclaration><Name>" + name + "</Name><Generic>" + generic + "</Generic><Args>";
    for (AST* arg : args) rep += arg->asXML();
    return rep + "</Args><Returns>" + returns + "</Returns>" + block->asXML() + "</FunctionDeclaration>";
}

std::string FunctionDeclaration::asJSON() {
    std::string rep = "{\"*\":\"FunctionDeclaration\",\"name\":\"" + name + "\",\"generic\":\"" + generic + "\",\"args\":[";
    for (AST* arg : args) rep += arg->asJSON() + ",";
    return rep + "],\"returns\":\"" + returns + "\",\"block\":" + block->asJSON() + "}";
}

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
}

AST* Parser::factor(void) {
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
        if (lexer->tk == '(' || lexer->tk == '<') return functionCall(id);
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
    std::string generic = "";
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') {
            if (lexer->currCh != '>') 
                return new BinaryOperator(callback, shiftExpression(), 60);

            generic = lexer->tkStr;
            lexer->match(TOK_ID);
        }
        lexer->match('>');
    }

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
    return new FunctionCall(callback->name, generic, args);
}

AST* Parser::functionCall(void) {
    std::string callback = lexer->tkStr;
    lexer->match(TOK_ID);
    functionCall(new VariableIdentifier(callback));
}

// AST* Parser::unaryExpression(void) {}

AST* Parser::multiplicativeExpression(void) {
    AST* node = factor();
    while (lexer->tk == '*' || lexer->tk == '/' || lexer->tk == '%') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = factor();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::additiveExpression(void) {
    AST* node = multiplicativeExpression();
    while (lexer->tk == '+' || lexer->tk == '-') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = multiplicativeExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::shiftExpression(void) {
    AST* node = additiveExpression();
    while (lexer->tk == TOK_RSHIFT || lexer->tk == TOK_LSHIFT) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = additiveExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::relationalExpression(void) {
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

AST* Parser::equalityExpression(void) {
    AST* node = relationalExpression();
    while (lexer->tk == TOK_EQUAL || lexer->tk == TOK_NEQUAL) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = relationalExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseAndExpression(void) {
    AST* node = equalityExpression();
    while (lexer->tk == '&') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = equalityExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseXorExpression(void) {
    AST* node = bitwiseAndExpression();
    while (lexer->tk == '^') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseAndExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::bitwiseOrExpression(void) {
    AST* node = bitwiseXorExpression();
    while (lexer->tk == '|') {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseXorExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::logicalAndExpression(void) {
    AST* node = bitwiseOrExpression();
    while (lexer->tk == TOK_ANDAND) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = bitwiseOrExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::logicalOrExpression(void) {
    AST* node = logicalAndExpression();
    while (lexer->tk == TOK_OROR) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        AST* right = logicalAndExpression();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

AST* Parser::statement(void) {
    if (lexer->tk == TOK_R_IF) {
        lexer->match(TOK_R_IF);
        lexer->match('(');
        AST* cond = logicalOrExpression();
        lexer->match(')');
        AST* then;
        if (lexer->tk == '{') {
            then = block();
        } else {
            then = statement();
        }
        AST* else_ = nullptr;
        if (lexer->tk == TOK_R_ELSE) {
            lexer->match(TOK_R_ELSE);
            if (lexer->tk == '{') {
                else_ = block();
            } else {
                else_ = statement();
            }
        }
        return new IfElseStatement(cond, then, else_);
    }

    if (lexer->tk == TOK_R_WHILE) {
        lexer->match(TOK_R_WHILE);
        lexer->match('(');
        AST* cond = logicalOrExpression();
        lexer->match(')');
        return new WhileStatement(cond, block());
    }
    if (lexer->tk != TOK_ID) {
        throw new Exception("Expected some sort of identifier at " +
                            lexer->getPosition(lexer->tokenStart));
        return (AST*)NULL;
    }

    VariableIdentifier* node = new VariableIdentifier(lexer->tkStr);
    lexer->match(lexer->tk);

    if (lexer->tk == '(' || lexer->tk == '<') {
        auto f = functionCall(node);
        lexer->match(';');
        return f;
    }

    if (lexer->tk == '=' || lexer->tk == TOK_PLUSEQUAL ||
        lexer->tk == TOK_MINUSEQUAL || lexer->tk == TOK_TIMESEQUAL ||
        lexer->tk == TOK_DIVIDEEQUAL || lexer->tk == TOK_MODEQUAL ||
        lexer->tk == TOK_LSHIFTEQUAL || lexer->tk == TOK_RSHIFTEQUAL ||
        lexer->tk == TOK_ANDEQUAL || lexer->tk == TOK_OREQUAL ||
        lexer->tk == TOK_XOREQUAL) {
        int op = lexer->tk;
        lexer->match(lexer->tk);
        auto v = new VariableReAssignment(op, node->name, logicalOrExpression());
        lexer->match(';');
        return v;
    } else if (lexer->tk == TOK_ID) {
        std::string name = lexer->tkStr;
        lexer->match(lexer->tk);
        lexer->match('=');
        auto v = new VariableAssignment(node->name, name, logicalOrExpression());
        lexer->match(';');
        return v;
    }
}

AST* Parser::block(void) {
    lexer->match('{');
    std::vector<AST*> statements;
    while (lexer->tk != '}') {
        statements.push_back(statement());
    }
    lexer->match('}');
    return new Block(statements);
}

AST* Parser::parameterDeclaration(void) {
    auto type = lexer->tkStr;
    lexer->match(TOK_ID);
    auto name = lexer->tkStr;
    lexer->match(TOK_ID);
    return new ParameterDeclaration(type, name);
}

AST* Parser::functionDeclaration(void) {
    std::string generic = "";
    lexer->match(TOK_R_FUNC);
    std::string name = lexer->tkStr;
    lexer->match(TOK_ID);
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') {
            generic = lexer->tkStr;
            lexer->match(TOK_ID);
        }
        lexer->match('>');
    }
    lexer->match('(');
    std::vector<AST*> args;
    if (lexer->tk != ')') {
        args.push_back(parameterDeclaration());
        while (lexer->tk == ',') {
            lexer->match(',');
            args.push_back(parameterDeclaration());
        }
    }
    lexer->match(')');
     std::string returns = "u0";
    if (lexer->tk == TOK_ID) { 
        returns = lexer->tkStr;
        lexer->match(TOK_ID);
    }
    return new FunctionDeclaration(name, generic, args, block(), returns);
}

AST* Parser::program(void) {
    std::vector<AST*> statements;
    while (lexer->tk != TOK_EOF) {
        statements.push_back(functionDeclaration());
    }
    return new Block(statements);
}

AST* Parser::parse(void) {
    return program();
}
