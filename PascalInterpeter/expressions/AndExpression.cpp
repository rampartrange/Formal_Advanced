#include "AndExpression.h"

AndExpression::AndExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

int AndExpression::eval() const {
    return lhs->eval() && rhs->eval();
}