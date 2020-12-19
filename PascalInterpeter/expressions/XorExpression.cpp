#include "XorExpression.h"

XorExpression::XorExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

int XorExpression::eval() const {
    return ((lhs->eval()) ^ (rhs->eval()));
}