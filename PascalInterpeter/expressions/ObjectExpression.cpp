#include "ObjectExpression.h"

#include <utility>

ObjectExpression::ObjectExpression(int value) : value(PascalObject(value)) {}

ObjectExpression::ObjectExpression(double value) : value(PascalObject(value)) {}

ObjectExpression::ObjectExpression(std::string value) : value(PascalObject(std::move(value))) {}

ObjectExpression::ObjectExpression(bool value) : value(PascalObject(value)) {}

PascalObject ObjectExpression::eval() const {
    return value;
}