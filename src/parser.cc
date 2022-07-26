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
    auto type = new TypeIdentifier(NULL, name);
    if (lexer->tk == '<') {
        lexer->match('<');
        type->child = parseTypeIdent();
        lexer->match('>');
    }
    return type;
}

Node* Parser::parseParamDecl(void) {
    return new ParameterDeclaration(parseTypeIdent(), 
            parseVarIdent());
}

Node* Parser::parseBlock(void) {
    lexer->match('{');
    std::vector<Node*> statements;
    while (lexer->tk != '}')
        statements.push_back(parseStatement());
    lexer->match('}');
    return new Block(statements);
}

Node* Parser::parseStatement(void) {
    if (lexer->tk == TOK_R_IF) {
        lexer->match(TOK_R_IF);
        lexer->match('(');
        Node* condition = parseLogical();
    }
}