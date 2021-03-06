#include "DivExpression.h"

DivExpression::DivExpression(
    Expression *e1, Expression *e2
): first(e1), second(e2) {}

PascalObject DivExpression::eval() const {
    return first->eval() / second->eval();
}