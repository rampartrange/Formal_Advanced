#include "MulExpression.h"

MulExpression::MulExpression(
    Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

int MulExpression::eval() const {
    return lhs->eval() * rhs->eval();
}