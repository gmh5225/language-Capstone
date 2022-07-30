/**
 * Copyright (C) 2021-2022 Justus Languell
 * This file is part of Capstone, which is licensed under the MIT license.
 * For more details, see ./license.txt or write <jus@gtsbr.org>.
 */
#include "parser.h"

Node* Parser::parse(void) {
    return parseFile();
}

Node* Parser::parseFile(void) {
    std::vector<Node*> nodes;
    while (lexer->tk != TOK_EOF) nodes.push_back(parseGlobalScope());
    return new Block(nodes);
}

Node* Parser::parseGlobalScope(void) {
    if (lexer->tk == TOK_R_FUNC)
        return parseFuncDecl();
    else if (lexer->tk == TOK_R_IMPORT)
        return parseImport();
    else if (lexer->tk == TOK_R_ENUM)
        return parseEnumDecl();
    else if (lexer->tk == TOK_R_CLASS)
        return parseClassDecl();
    else
        return parseBlockOrStatement();
}

Node* Parser::parseEnumDecl(void) {
    lexer->match(TOK_R_ENUM);
    Node* name = parseTypeIdent();
    std::vector<Node*> nodes;
    
    lexer->match('{');
    if (lexer->tk != '{') {
        nodes.push_back(parseVarIdent());
        while (lexer->tk == ',') {
            lexer->match(',');
            nodes.push_back(parseVarIdent());
        }
    }
    lexer->match('}');
    
    return new EnumDeclaration(name, nodes);
}

Node* Parser::parseClassDecl(void) {
    lexer->match(TOK_R_CLASS);
    Node* name = parseTypeIdent();
    Node* super = nullptr;
    if (lexer->tk == ':') {
        lexer->match(':');
        super = parseTypeIdent();
    }
    Node* body = parseClassBody();
    return new ClassDeclaration(name, super, body);
}

Node* Parser::parseClassBody(void) {
    lexer->match('{');
    std::vector<Node*> fields;
    while (lexer->tk != '}') fields.push_back(parseClassField());
    lexer->match('}');
    return new Block(fields);
}

Node* Parser::parseClassField(void) {
    unsigned int staticness = 0;
    if (lexer->tk == TOK_R_PRIVATE) {
        lexer->match(TOK_R_PRIVATE);
        if (lexer->tk == TOK_R_STATIC) {
            lexer->match(TOK_R_STATIC);
            staticness = 1;
        }
        return new ClassField(parseClassMember(), 0, staticness);
    } else if (lexer->tk == TOK_R_PROTECTED) {
        lexer->match(TOK_R_PROTECTED);
        if (lexer->tk == TOK_R_STATIC) {
            lexer->match(TOK_R_STATIC);
            staticness = 1;
        }
        return new ClassField(parseClassMember(), 1, staticness);
    } else if (lexer->tk == TOK_R_PUBLIC) {
        lexer->match(TOK_R_PUBLIC);
        if (lexer->tk == TOK_R_STATIC) {
            lexer->match(TOK_R_STATIC);
            staticness = 1;
        }
        return new ClassField(parseClassMember(), 2, staticness);
    } else {
         if (lexer->tk == TOK_R_STATIC) {
            lexer->match(TOK_R_STATIC);
            staticness = 1;
        }
        return new ClassField(parseClassMember(), 0, staticness);
    }
}



Node* Parser::parseClassMember(void) {
    if (lexer->tk == TOK_R_FUNC)
        return parseFuncDecl();
    else
        return parseExpressionStatement();
}

Node* Parser::parseImport(void) {
    lexer->match(TOK_R_IMPORT);
    Node* package = parseVarIdent();
    lexer->match(';');
    return new ImportStatement(package);
}

Node* Parser::parseFuncDecl(void) {
    Node* generic = NULL;
    lexer->match(TOK_R_FUNC);

    Node* name = parseVarIdent();
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') generic = parseTypeIdent();
    }
    lexer->match('(');
    std::vector<Node*> params;
    if (lexer->tk != ')') {
        params.push_back(parseParamDecl());
        while (lexer->tk == ',') {
            lexer->match(',');
            params.push_back(parseParamDecl());
        }
    }
    lexer->match(')');
    std::vector<Node*> returns;
    if (lexer->tk == TOK_ID)
        returns.push_back(parseTypeIdent());
    else if (lexer->tk == '(') {
        lexer->match('(');
        returns.push_back(parseTypeIdent());
        while (lexer->tk == ',') {
            lexer->match(',');
            returns.push_back(parseTypeIdent());
        }
        lexer->match(')');
    }
    Node* body = parseBlock();
    return new FunctionDeclaration(name, generic, params, returns, body);
}

Node* Parser::parseVarIdent(void) {
    const std::string name = lexer->tkStr;
    lexer->match(TOK_ID);
    auto var = new VariableIdentifier(NULL, name);
    if (lexer->tk == '.') {
        lexer->match('.');
        var->child = parseVarIdent();
    }
    return var;
}

Node* Parser::parseTypeIdent(void) {
    const bool isConst = lexer->tk == TOK_R_CONST;
    if (isConst) lexer->match(TOK_R_CONST);
    const std::string name = lexer->tkStr;
    lexer->match(TOK_ID);
    std::vector<Node*> children;
    if (lexer->tk == '<') {
        lexer->match('<');
        children.push_back(parseTypeIdent());
        while (lexer->tk == ',') {
            lexer->match(',');
            children.push_back(parseTypeIdent());
        }

        if (lexer->tk == TOK_RSHIFT)
            lexer->match(TOK_RSHIFT);
        else if (lexer->tk == '>')
            lexer->match('>');
        else
            ;
        // throw new Exception("Got " + Lexer::getTokenStr(lexer->tk) + "
        // expected " +
        //                 Lexer::getTokenStr('>') + " at " +
        //                 lexer->getPosition(lexer->tokenStart));
    }

    auto type = new TypeIdentifier(children, name, 0, isConst ? 1 : 0);
    while (lexer->tk == '[') {
        lexer->match('[');
        lexer->match(']');
        type->list++;
    }
    return type;
}

Node* Parser::parseParamDecl(void) {
    Node* var = parseVarIdent();
    lexer->match(':');
    return new ParameterDeclaration(var, parseTypeIdent());
}

Node* Parser::parseBlockOrStatement(void) {
    return lexer->tk == '{' ? parseBlock() : parseStatement();
}

Node* Parser::parseBlock(void) {
    lexer->match('{');
    std::vector<Node*> statements;
    while (lexer->tk != '}') statements.push_back(parseStatement());
    lexer->match('}');
    return new Block(statements);
}

Node* Parser::parseStatement(void) {
    if (lexer->tk == TOK_R_IF)
        return parseIfElseStatement();
    else if (lexer->tk == TOK_R_WHILE)
        return parseWhileStatement();
    else if (lexer->tk == TOK_R_FOR)
        return parseForStatement();
    else if (lexer->tk == TOK_R_BREAK)
        return parseBreakStatement();
    else if (lexer->tk == TOK_R_CONTINUE)
        return parseContinueStatement();
    else if (lexer->tk == TOK_R_RETURN)
        return parseReturnStatement();
    else
        return parseExpressionStatement();
}

Node* Parser::parseIfElseStatement(void) {
    lexer->match(TOK_R_IF);
    lexer->match('(');
    Node* condition = parseExpression();
    lexer->match(')');
    Node* ifStatement = parseBlockOrStatement();
    Node* elseStatement = nullptr;
    if (lexer->tk == TOK_R_ELSE) {
        lexer->match(TOK_R_ELSE);
        elseStatement = parseBlockOrStatement();
    }
    return new IfElseStatement(condition, ifStatement, elseStatement);
}

Node* Parser::parseWhileStatement(void) {
    lexer->match(TOK_R_WHILE);
    lexer->match('(');
    Node* condition = parseExpression();
    lexer->match(')');
    Node* block = parseBlockOrStatement();
    return new WhileStatement(condition, block);
}

Node* Parser::parseForStatement(void) {
    lexer->match(TOK_R_FOR);
    lexer->match('(');
    Node* init = lexer->tk != ';' ? parseExpressionStatement() : NULL;
    Node* condition = parseExpression();
    lexer->match(';');
    Node* post = lexer->tk != ';' ? parseExpression() : NULL;
    lexer->match(')');
    Node* block = parseBlockOrStatement();
    return new ForStatement(init, condition, post, block);
}

Node* Parser::parseBreakStatement(void) {
    lexer->match(TOK_R_BREAK);
    lexer->match(';');
    return new BreakStatement();
}

Node* Parser::parseContinueStatement(void) {
    lexer->match(TOK_R_CONTINUE);
    lexer->match(';');
    return new ContinueStatement();
}

Node* Parser::parseReturnStatement(void) {
    lexer->match(TOK_R_RETURN);
    std::vector<Node*> nodes;
    if (lexer->tk != ';') {
        nodes.push_back(parseExpression());
        while (lexer->tk == ',') {
            lexer->match(',');
            nodes.push_back(parseExpression());
        }
    }
    lexer->match(';');
    return new ReturnStatement(nodes);
}

Node* Parser::parseExpressionStatement(void) {
    LOG(1);
    if (lexer->tk == ';') {
        lexer->match(';');
        return NULL;
    }
    LOG(2);
    if (lexer->tk == TOK_R_VAR || lexer->tk == TOK_R_CONST) 
        return parseVarDecl();
    else
        return parseExpression();
}

Node* Parser::parseVarDecl(void) {
    LOG(3);
    const bool isConst = lexer->tk == TOK_R_CONST;
    lexer->match(isConst ? TOK_R_CONST : TOK_R_VAR);
    const unsigned int nConst = isConst ? 1 : 0;
    Node* name = parseVarIdent();
    Node* type = nullptr;
    if (lexer->tk == ':') {
        lexer->match(':');
        type = parseTypeIdent();
    }
    if (lexer->tk == ';') {
        lexer->match(';');
        return new VariableDeclaration(nConst, type, name, NULL);
    } else {
        lexer->match('=');
        auto value = parseExpression();
        lexer->match(';');
        return new VariableDeclaration(nConst, type, name, value);
    }
}



Node* Parser::parseExpression(void) {
    return parseTernaryExpression();
}

Node* Parser::parseTernaryExpression(void) {
    Node* condition = parseAssignExpression();
    if (lexer->tk != '?') return condition;
    lexer->match('?');
    Node* ifTrue = parseExpression();
    lexer->match(':');
    Node* ifFalse = parseExpression();
    return new TernaryExpression(condition, ifTrue, ifFalse);
}

Node* Parser::parseAssignExpression(void) {
    return parseBinaryOperator({'=', TOK_PLUSEQUAL, TOK_MINUSEQUAL,
                                TOK_TIMESEQUAL, TOK_DIVIDEEQUAL, TOK_MODEQUAL,
                                TOK_LSHIFTEQUAL, TOK_RSHIFTEQUAL, TOK_ANDEQUAL,
                                TOK_OREQUAL, TOK_XOREQUAL},
                               &Parser::parseLogicalOr);
}

Node* Parser::parseLogicalOr(void) {
    return parseBinaryOperator(TOK_OROR, &Parser::parseLogicalAnd);
}

Node* Parser::parseLogicalAnd(void) {
    return parseBinaryOperator(TOK_ANDAND, &Parser::parseEquality);
}

Node* Parser::parseBitwiseOr(void) {
    return parseBinaryOperator('|', &Parser::parseBitwiseXor);
}

Node* Parser::parseBitwiseXor(void) {
    return parseBinaryOperator('^', &Parser::parseBitwiseAnd);
}

Node* Parser::parseBitwiseAnd(void) {
    return parseBinaryOperator('&', &Parser::parseEquality);
}

Node* Parser::parseEquality(void) {
    return parseBinaryOperator({TOK_EQUAL, TOK_NEQUAL},
                               &Parser::parseRelational);
}

Node* Parser::parseRelational(void) {
    return parseBinaryOperator(
            {TOK_SPACESHIP, '>', '<', TOK_GEQUAL, TOK_LEQUAL},
            &Parser::parseShift);
}

Node* Parser::parseShift(void) {
    return parseBinaryOperator({TOK_RSHIFT, TOK_LSHIFT},
                               &Parser::parseAdditive);
}

Node* Parser::parseAdditive(void) {
    return parseBinaryOperator({'+', '-'}, &Parser::parseMultiplicative);
}

Node* Parser::parseMultiplicative(void) {
    return parseBinaryOperator({'*', '/', '%'}, &Parser::parseUnaryExpression);
}

Node* Parser::parseUnaryExpression(void) {
    if (lexer->tk == '!' || lexer->tk == '$') {
        const int op = lexer->tk;
        lexer->match(lexer->tk);
        return new UnaryOperator(parseElement(), op);
    } else
        return parseElement();
}

Node* Parser::parseElement(void) {
    if (lexer->tk == TOK_INT || lexer->tk == TOK_FLOAT) {
        auto num = new NumberLiteral(lexer->tkStr);
        lexer->match(lexer->tk);
        return num;
    } else if (lexer->tk == TOK_STR) {
        auto str = new StringLiteral(lexer->tkStr);
        lexer->match(lexer->tk);
        return str;
    } else if (lexer->tk == TOK_R_TRUE || lexer->tk == TOK_R_FALSE) {
        auto boolean = new BooleanLiteral(lexer->tk == TOK_R_TRUE ? "1" : "0");
        lexer->match(lexer->tk);
        return boolean;
    } else if (lexer->tk == TOK_R_NULL) {
        auto null = new NullLiteral();
        lexer->match(lexer->tk);
        return null;
    } else if (lexer->tk == '[') {
        lexer->match('[');
        std::vector<Node*> elements;
        if (lexer->tk != ']') {
            elements.push_back(parseExpression());
            while (lexer->tk == ',') {
                lexer->match(',');
                elements.push_back(parseExpression());
            }
        }
        lexer->match(']');
        return new ArrayLiteral(elements);
    } else if (lexer->tk == '<') {
        lexer->match('<');
        auto type = parseTypeIdent();
        lexer->match('>');
        auto name = parseVarIdent();
        lexer->match('(');
        std::vector<Node*> params;
        if (lexer->tk != ')') {
            params.push_back(parseExpression());
            while (lexer->tk == ',') {
                lexer->match(',');
                params.push_back(parseExpression());
            }
        }
        lexer->match(')');
        return new FunctionCall(name, type, params);
    } else if (lexer->tk == TOK_ID) {
        auto name = parseVarIdent();
        if (lexer->tk != '(')
            return name;
        else {
            lexer->match('(');
            std::vector<Node*> params;
            if (lexer->tk != ')') {
                params.push_back(parseExpression());
                while (lexer->tk == ',') {
                    lexer->match(',');
                    params.push_back(parseExpression());
                }
            }
            lexer->match(')');
            return new FunctionCall(name, NULL, params);
        }
    }
}

Node* Parser::parseBinaryOperator(const int token,
                                  Node* (Parser::*callback)(void)) {
    Node* node = (*this.*callback)();
    while (lexer->tk == token) {
        const int op = lexer->tk;
        lexer->match(lexer->tk);
        Node* right = (*this.*callback)();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

Node* Parser::parseBinaryOperator(std::vector<int> tokens,
                                  Node* (Parser::*callback)(void)) {
    Node* node = (*this.*callback)();
    while (std::count(tokens.begin(), tokens.end(), lexer->tk) > 0) {
        const int op = lexer->tk;
        lexer->match(lexer->tk);
        Node* right = (*this.*callback)();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}
