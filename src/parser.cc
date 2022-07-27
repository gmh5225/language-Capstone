#include "parser.h"

Node* Parser::parse(void) {
    return parseFile();
}

Node* Parser::parseFile(void) {
    std::vector<Node*> nodes;
    while (lexer->tk != TOK_EOF)
        nodes.push_back(parseFuncDecl());
    return new Block(nodes);
}

Node* Parser::parseFuncDecl(void) {
    Node* generic = NULL;
    lexer->match(TOK_R_FUNC);

    Node* name = parseVarIdent();
    if (lexer->tk == '<') {
        lexer->match('<');
        if (lexer->tk != '>') 
            generic = parseTypeIdent();
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
        else ;
            // throw new Exception("Got " + Lexer::getTokenStr(lexer->tk) + " expected " +
            //                 Lexer::getTokenStr('>') + " at " +
            //                 lexer->getPosition(lexer->tokenStart));
    }

    auto type = new TypeIdentifier(children, name, 0);
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
    while (lexer->tk != '}')
        statements.push_back(parseStatement());
    lexer->match('}');
    std::cout << statements.size() << std::endl;
    return new Block(statements);
}

Node* Parser::parseStatement(void) {
    if (lexer->tk == TOK_R_IF)
        return parseIfElseStatement();
    else if (lexer->tk == TOK_R_WHILE)
        return parseWhileStatement();
    else
        return parseExpressionStatement();
}

Node* Parser::parseIfElseStatement(void) {
    lexer->match(TOK_R_IF);
    lexer->match('(');
    Node* condition = parseExpression();
    lexer->match(')');
    Node* ifStatement = parseBlockOrStatement();
    Node* elseStatement;
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

// var: type
Node* Parser::parseExpressionStatement(void) {
    Node* node = parseExpression();
    if (lexer->tk != ':') {
        auto es = new ExpressionStatement(node);
        lexer->match(';');
        return es;
    } else {
        lexer->match(':');
        auto type = parseTypeIdent();
        if (lexer->tk == ';') {
            lexer->match(';');
            return new VariableDeclaration(type, node, NULL);
        } else {
            lexer->match('=');
            auto value = parseExpression();
            lexer->match(';');
            return new VariableDeclaration(type, node, value);
        }
    } 
}

Node* Parser::parseExpression(void) {
    return parseAssignExpression();
}

Node* Parser::parseDeclExpression(void) {
    return parseBinaryOperator(':', &Parser::parseAssignExpression);
}

Node* Parser::parseAssignExpression(void) {
    return parseBinaryOperator({'=', TOK_PLUSEQUAL, TOK_MINUSEQUAL, TOK_TIMESEQUAL, TOK_DIVIDEEQUAL, TOK_MODEQUAL, TOK_LSHIFTEQUAL, TOK_RSHIFTEQUAL, TOK_ANDEQUAL, TOK_OREQUAL, TOK_XOREQUAL}, &Parser::parseLogicalOr);
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
    return parseBinaryOperator({TOK_EQUAL, TOK_NEQUAL}, &Parser::parseRelational);
}

Node* Parser::parseRelational(void) {
    return parseBinaryOperator({TOK_SPACESHIP, '>', '<', TOK_GEQUAL, TOK_LEQUAL}, &Parser::parseShift);
}

Node* Parser::parseShift(void) {
    return parseBinaryOperator({TOK_RSHIFT, TOK_LSHIFT}, &Parser::parseAdditive);
}

Node* Parser::parseAdditive(void) {
    return parseBinaryOperator({'+', '-'}, &Parser::parseMultiplicative);
}

Node* Parser::parseMultiplicative(void) {
    return parseBinaryOperator({'*', '/', '%'}, &Parser::parseElement);
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
        auto null = new NullLiteral("null");
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

Node* Parser::parseBinaryOperator(const int token, Node* (Parser::*callback)(void)) {
    Node* node = (*this.*callback)();
    while (lexer->tk == token) {
        const int op = lexer->tk;
        lexer->match(lexer->tk);
        Node* right = (*this.*callback)();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

Node* Parser::parseBinaryOperator(std::vector<int> tokens, Node* (Parser::*callback)(void)) {
    Node* node = (*this.*callback)();
    while (std::count(tokens.begin(), tokens.end(), lexer->tk) > 0) {
        const int op = lexer->tk;
        lexer->match(lexer->tk);
        Node* right = (*this.*callback)();
        node = new BinaryOperator(node, right, op);
    }
    return node;
}

