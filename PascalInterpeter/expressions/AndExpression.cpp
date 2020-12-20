#include "AndExpression.h"

AndExpression::AndExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

PascalObject AndExpression::eval() const {
    return PascalObject(lhs->eval() && rhs->eval());
}