#include "ast.h"

// Generated by ./scripts/ast_gen.py

std::string BinaryOperator::toJSON(void) {
    return "{\"_type\": \"BinaryOperator\",\"left\": " + left->toJSON() + ",\"right\": " + right->toJSON() + ",\"op\":" + Lexer::getTokenStr(op) + ",}";
}

std::string FunctionCall::toJSON(void) {
    return "{\"_type\": \"FunctionCall\",\"callback\": " + callback->toJSON() + ",\"generic\": " + generic->toJSON() + ",}";
}

std::string NumberLiteral::toJSON(void) {
    return "{\"_type\": \"NumberLiteral\",\"literal\":" + literal + ",}";
}

std::string StringLiteral::toJSON(void) {
    return "{\"_type\": \"StringLiteral\",\"literal\":" + literal + ",}";
}

std::string BooleanLiteral::toJSON(void) {
    return "{\"_type\": \"BooleanLiteral\",\"literal\":" + literal + ",}";
}

std::string VariableIdentifier::toJSON(void) {
    return "{\"_type\": \"VariableIdentifier\",\"child\": " + child->toJSON() + ",\"name\":" + name + ",}";
}

std::string TypeIdentifier::toJSON(void) {
    return "{\"_type\": \"TypeIdentifier\",\"child\": " + child->toJSON() + ",\"name\":" + name + ",}";
}

std::string VariableDeclaration::toJSON(void) {
    return "{\"_type\": \"VariableDeclaration\",\"type\": " + type->toJSON() + ",\"name\": " + name->toJSON() + ",\"value\": " + value->toJSON() + ",}";
}

std::string ExpressionStatement::toJSON(void) {
    return "{\"_type\": \"ExpressionStatement\",\"expression\": " + expression->toJSON() + ",}";
}

std::string Block::toJSON(void) {
    return "{\"_type\": \"Block\",}";
}

std::string IfElseStatement::toJSON(void) {
    return "{\"_type\": \"IfElseStatement\",\"condition\": " + condition->toJSON() + ",\"ifBlock\": " + ifBlock->toJSON() + ",\"elseBlock\": " + elseBlock->toJSON() + ",}";
}

std::string WhileStatement::toJSON(void) {
    return "{\"_type\": \"WhileStatement\",\"condition\": " + condition->toJSON() + ",\"block\": " + block->toJSON() + ",}";
}

std::string ParameterDeclaration::toJSON(void) {
    return "{\"_type\": \"ParameterDeclaration\",\"type\": " + type->toJSON() + ",\"name\": " + name->toJSON() + ",}";
}

std::string FunctionDeclaration::toJSON(void) {
    return "{\"_type\": \"FunctionDeclaration\",\"name\": " + name->toJSON() + ",\"generic\": " + generic->toJSON() + ",\"block\": " + block->toJSON() + ",}";
}