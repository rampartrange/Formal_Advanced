#include "OrExpression.h"

OrExpression::OrExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

int OrExpression::eval() const {
    return ((lhs->eval()) || (rhs->eval()));
}