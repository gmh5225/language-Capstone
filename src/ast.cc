#include "ast.h"

// Generated by ./scripts/ast_gen.py

static std::string createList(const std::vector<Node*>& v) {
    std::string result = "[";
    for (int i = 0; i < v.size(); i++) {
        result += nullSafeToString(v[i]);
        if (i < v.size() - 1) result += ", ";
    }
    return result + "]";
}

static std::string safeLiterals(const std::string& str) {
    std::string result = "";
    for (char const& c : str) {
        switch (c) {
        case '\n': result += "\\n"; break;
        case '\r': result += "\\r"; break;
        case '\t': result += "\\t"; break;
        case '\a': result += "\\a"; break;
        case '\b': result += "\\b"; break;
        case '\f': result += "\\f"; break;
        case '\v': result += "\\v"; break;
        case '\\': result += "\\\\"; break;
        case '\'': result += "\\'"; break;
        case '\"': result += "\\\""; break;
        default: result += c;
        }
    }
    return result;
}

static std::string nullSafeToString(Node* node) {
    return node == nullptr ? "null" : node->toJSON();
}

std::string UnaryOperator::toJSON(void) {
    return "{\"_type\": \"UnaryOperator\",\"element\": " + nullSafeToString(element) + ",\"op\": \"" + Lexer::getTokenStr(op) + "\"}";
}

std::string BinaryOperator::toJSON(void) {
    return "{\"_type\": \"BinaryOperator\",\"left\": " + nullSafeToString(left) + ",\"right\": " + nullSafeToString(right) + ",\"op\": \"" + Lexer::getTokenStr(op) + "\"}";
}

std::string FunctionCall::toJSON(void) {
    return "{\"_type\": \"FunctionCall\",\"callback\": " + nullSafeToString(callback) + ",\"generic\": " + nullSafeToString(generic) + ",\"params\": " + createList(params) + "}";
}

std::string NumberLiteral::toJSON(void) {
    return "{\"_type\": \"NumberLiteral\",\"literal\": \"" + safeLiterals(literal) + "\"}";
}

std::string StringLiteral::toJSON(void) {
    return "{\"_type\": \"StringLiteral\",\"literal\": \"" + safeLiterals(literal) + "\"}";
}

std::string BooleanLiteral::toJSON(void) {
    return "{\"_type\": \"BooleanLiteral\",\"literal\": \"" + safeLiterals(literal) + "\"}";
}

std::string NullLiteral::toJSON(void) {
    return "{\"_type\": \"NullLiteral\"}";
}

std::string ArrayLiteral::toJSON(void) {
    return "{\"_type\": \"ArrayLiteral\",\"literal\": " + createList(literal) + "}";
}

std::string VariableIdentifier::toJSON(void) {
    return "{\"_type\": \"VariableIdentifier\",\"child\": " + nullSafeToString(child) + ",\"name\": \"" + safeLiterals(name) + "\"}";
}

std::string TypeIdentifier::toJSON(void) {
    return "{\"_type\": \"TypeIdentifier\",\"children\": " + createList(children) + ",\"name\": \"" + safeLiterals(name) + "\",\"list\": \"" + std::to_string(list) + "\",\"final\": \"" + std::to_string(final) + "\"}";
}

std::string VariableDeclaration::toJSON(void) {
    return "{\"_type\": \"VariableDeclaration\",\"type\": " + nullSafeToString(type) + ",\"name\": " + nullSafeToString(name) + ",\"value\": " + nullSafeToString(value) + "}";
}

std::string ExpressionStatement::toJSON(void) {
    return "{\"_type\": \"ExpressionStatement\",\"expression\": " + nullSafeToString(expression) + "}";
}

std::string Block::toJSON(void) {
    return "{\"_type\": \"Block\",\"statements\": " + createList(statements) + "}";
}

std::string IfElseStatement::toJSON(void) {
    return "{\"_type\": \"IfElseStatement\",\"condition\": " + nullSafeToString(condition) + ",\"ifBlock\": " + nullSafeToString(ifBlock) + ",\"elseBlock\": " + nullSafeToString(elseBlock) + "}";
}

std::string WhileStatement::toJSON(void) {
    return "{\"_type\": \"WhileStatement\",\"condition\": " + nullSafeToString(condition) + ",\"block\": " + nullSafeToString(block) + "}";
}

std::string ForStatement::toJSON(void) {
    return "{\"_type\": \"ForStatement\",\"init\": " + nullSafeToString(init) + ",\"condition\": " + nullSafeToString(condition) + ",\"post\": " + nullSafeToString(post) + ",\"block\": " + nullSafeToString(block) + "}";
}

std::string ParameterDeclaration::toJSON(void) {
    return "{\"_type\": \"ParameterDeclaration\",\"type\": " + nullSafeToString(type) + ",\"name\": " + nullSafeToString(name) + "}";
}

std::string FunctionDeclaration::toJSON(void) {
    return "{\"_type\": \"FunctionDeclaration\",\"name\": " + nullSafeToString(name) + ",\"generic\": " + nullSafeToString(generic) + ",\"params\": " + createList(params) + ",\"returns\": " + createList(returns) + ",\"block\": " + nullSafeToString(block) + "}";
}

std::string BreakStatement::toJSON(void) {
    return "{\"_type\": \"BreakStatement\"}";
}

std::string ContinueStatement::toJSON(void) {
    return "{\"_type\": \"ContinueStatement\"}";
}

std::string ReturnStatement::toJSON(void) {
    return "{\"_type\": \"ReturnStatement\",\"expressions\": " + createList(expressions) + "}";
}

std::string ImportStatement::toJSON(void) {
    return "{\"_type\": \"ImportStatement\",\"package\": " + nullSafeToString(package) + "}";
}