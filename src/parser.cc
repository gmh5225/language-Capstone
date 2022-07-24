/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#include "parser.h"

NumberLiteral::NumberLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string NumberLiteral::asJSON(void) {
    return "{\"*\":\"NumberLiteral\",\"literal\":\"" + literal + "\"}";
}

StringLiteral::StringLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string StringLiteral::asJSON(void) {
    return "{\"*\":\"StringLiteral\",\"literal\":\"" + safeLiterals(literal) +
           "\"}";
}

BooleanLiteral::BooleanLiteral(const std::string& literal) {
    this->literal = literal;
}

std::string BooleanLiteral::asJSON(void) {
    return "{\"*\":\"BooleanLiteral\",\"literal\":\"" + safeLiterals(literal) +
           "\"}";
}

VariableIdentifier::VariableIdentifier(const std::string& name, AST* child) {
    this->name = name;
    this->child = child;
}

std::string VariableIdentifier::asJSON(void) {
    return "{\"*\":\"VariableIdentifier\",\"identifier\":\"" + name +
           "\",\"child\":" + (child == nullptr ? "\"*\"" : child->asJSON()) + "}";
}

TypeIdentifier::TypeIdentifier(const std::string& name) {
    this->name = name;
}

std::string TypeIdentifier::asJSON(void) {
    return "{\"*\":\"TypeIdentifier\",\"identifier\":\"" + name + "\"}";
}

BinaryOperator::BinaryOperator(AST* left, AST* right, int op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

std::string BinaryOperator::asJSON(void) {
    return "{\"*\":\"BinaryOperation\",\"left\":" + left->asJSON() +
           ",\"operator\":\"" + Lexer::getTokenStr(op) +
           "\",\"right\":" + right->asJSON() + "}";
}

FunctionCall::FunctionCall(AST* callback, AST*, std::vector<AST*> args) {
    this->callback = callback;
    this->generic = generic;
    this->args = args;
}

std::string FunctionCall::asJSON(void) {
    std::string rep = "{\"*\":\"FunctionCall\",\"callback\":" 
                    + callback->asJSON() 
                    + ",\"generic\":" + "\"balls\""
                    //+ (generic != nullptr ? generic->asJSON() : "\"*\"") 
                    + ",\"args\":[";

    //LOG("callback: " << callback->asJSON() << ", generic null check: " << (generic == nullptr ? "yes":"no"));
    if (generic != nullptr) {
        LOG("generic: " << generic->asJSON());
    }
    for (AST* arg : args) rep += arg->asJSON() + ",";
    if (this->args.size() != 0) rep.pop_back();
    return rep + "]}";
}

VariableAssignment::VariableAssignment(AST* type, AST* name, AST* value) {
    this->type = type;
    this->name = name;
    this->value = value;
}

std::string VariableAssignment::asJSON(void) {
    return "{\"*\":\"VariableAssignment\",\"type\":" + type->asJSON() +
           ",\"name\":" + name->asJSON() + ",\"value\":" + value->asJSON() + "}";
}

VariableReAssignment::VariableReAssignment(int op, AST* name, AST* value) {
    this->op = op;
    this->name = name;
    this->value = value;
}

std::string VariableReAssignment::asJSON(void) {
    return "{\"*\":\"VariableReAssignment\",\"name\":" + name->asJSON() +
           ",\"value\":" + value->asJSON() + "}";
}

Block::Block(std::vector<AST*> statements) {
    this->statements = statements;
}

std::string Block::asJSON(void) {
    std::string rep = "{\"*\":\"Block\",\"statements\":[";
    for (AST* statement : this->statements) rep += statement->asJSON() + ",";
    if (this->statements.size() != 0) rep.pop_back();
    return rep + "]}";
}

IfElseStatement::IfElseStatement(AST* condition, AST* ifBlock, AST* elseBlock) {
    this->condition = condition;
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
}

std::string IfElseStatement::asJSON(void) {
    std::string rep =
            "{\"*\":\"IfElseStatement\",\"condition\":" + condition->asJSON() +
            ",\"ifBlock\":" + ifBlock->asJSON();
    if (elseBlock != nullptr) rep += ",\"elseBlock\":" + elseBlock->asJSON();
    return rep + "}";
}

WhileStatement::WhileStatement(AST* condition, AST* block) {
    this->condition = condition;
    this->block = block;
}

std::string WhileStatement::asJSON(void) {
    return "{\"*\":\"WhileStatement\",\"condition\":" + condition->asJSON() +
           ",\"block\":" + block->asJSON() + "}";
}

ParameterDeclaration::ParameterDeclaration(AST* type, AST* name) {
    this->type = type;
    this->name = name;
}

std::string ParameterDeclaration::asJSON(void) {
    return "{\"*\":\"ParameterDeclaration\",\"type\":" + type->asJSON() +
           ",\"name\":" + name->asJSON() + "}";
}

FunctionDeclaration::FunctionDeclaration(AST* name,
                                         AST* generic,
                                         std::vector<AST*> args, AST* block,
                                         AST* returns) {
    this->name = name;
    this->generic = generic;
    this->args = args;
    this->block = block;
    this->returns = returns;
}

std::string FunctionDeclaration::asJSON(void) {
    std::string rep = "{\"*\":\"FunctionDeclaration\",\"name\":" + name->asJSON() +
                      ",\"generic\":" + (generic == nullptr ? "\"*\"" : generic->asJSON()) + ",\"args\":[";
    for (AST* arg : args) rep += arg->asJSON() + ",";
    if (this->args.size() != 0) rep.pop_back();
    return rep + "],\"returns\":" + (returns == nullptr ? "\"*\"" : returns->asJSON()) +
           ",\"block\":" + block->asJSON() + "}";
}

Parser::Parser(Lexer* lexer) {
    this->lexer = lexer;
}

AST* Parser::variableIdentifier(void) {
    std::string name = lexer->tkStr;
    lexer->match(TOK_ID);
    auto v = new VariableIdentifier(name, nullptr);
    if (lexer->tk == '.') {
        lexer->match('.');
        v->child = variableIdentifier();
    }
    return v;
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
        AST* id = variableIdentifier();
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

AST* Parser::functionCall(AST* callback) {
    AST* generic = nullptr;
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') {
            if (lexer->currCh != '>')
                return new BinaryOperator(callback, shiftExpression(), 60);

            generic = new TypeIdentifier(lexer->tkStr);
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
    //LOG((generic == nullptr ? "Function call" : "Generic function call"));
    return new FunctionCall(callback, generic, args);
}

AST* Parser::functionCall(void) {
    functionCall(variableIdentifier());
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

    std::string typeStr = lexer->tkStr;
    AST* node = variableIdentifier();

    if (lexer->tkStr == "" && lexer->tk == 59) {
        throw new Exception("Must do something with identifier at " +
                            lexer->getPosition(lexer->tokenStart));
    }

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
        AST* v = new VariableReAssignment(op, node, logicalOrExpression());
        lexer->match(';');
        return v;
    } else if (lexer->tk == TOK_ID) {
        AST* type = new TypeIdentifier(typeStr);
        AST* name = variableIdentifier();
        lexer->match('=');
        AST* v = new VariableAssignment(type, name, logicalOrExpression());
        lexer->match(';');
        return v;
    }
}

AST* Parser::block(void) {
    lexer->match('{');
    std::vector<AST*> statements;
    while (lexer->tk != '}') { 
        auto s = statement();
        statements.push_back(s);
        //statements.push_back(statement()); 
    }
    lexer->match('}');
    return new Block(statements);
}

AST* Parser::parameterDeclaration(void) {
    AST* type = new TypeIdentifier(lexer->tkStr);
    lexer->match(TOK_ID);
    AST* name = new VariableIdentifier(lexer->tkStr, nullptr);
    lexer->match(TOK_ID);
    return new ParameterDeclaration(type, name);
}

AST* Parser::functionDeclaration(void) {
    AST* generic = nullptr;
    lexer->match(TOK_R_FUNC);
    AST* name = variableIdentifier();
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') {
            generic = new TypeIdentifier(lexer->tkStr);
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
    AST* returns = nullptr;
    if (lexer->tk == TOK_ID) {
        returns = new TypeIdentifier(lexer->tkStr);
        lexer->match(TOK_ID);
    }
    AST* body = block();
    return new FunctionDeclaration(name, generic, args, body, returns);
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
