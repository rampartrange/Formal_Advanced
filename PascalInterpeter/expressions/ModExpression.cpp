#include "ModExpression.h"

ModExpression::ModExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

int ModExpression::eval() const {
    return lhs->eval() % rhs->eval();
}