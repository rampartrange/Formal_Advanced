#include "NumberExpression.h"

NumberExpression::NumberExpression(int value) : value(value) {}

int NumberExpression::eval() const {
    return value;
}