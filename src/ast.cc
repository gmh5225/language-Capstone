#include "ast.h"

std::string BinaryOperator::toJSON(void) {
    return "{\"_type\": \"BinaryOperator\",\"left\": " + left->toJSON() + ",\"right\": " + right->toJSON() + ",\"op\":" + Lexer::getTokenStr(op) + ",}";
}
std::string NumberLiteral::toJSON(void) {
    return "{\"_type\": \"NumberLiteral\",\"literal\":" + literal + ",}";
}