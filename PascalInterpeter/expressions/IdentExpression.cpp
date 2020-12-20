#include "IdentExpression.h"

#include <utility>

IdentExpression::IdentExpression(std::string  ident, PascalObject value): ident(std::move(ident)), value(value) {}

PascalObject IdentExpression::eval() const {
    return value;
}