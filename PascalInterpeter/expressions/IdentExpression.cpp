#include "IdentExpression.h"

IdentExpression::IdentExpression(const std::string& ident, int value): ident(ident), value(value) {}

int IdentExpression::eval() const {
    return value;
}