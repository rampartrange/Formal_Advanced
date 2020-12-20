#include "ModExpression.h"

ModExpression::ModExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

PascalObject ModExpression::eval() const {
    return lhs->eval() % rhs->eval();
}