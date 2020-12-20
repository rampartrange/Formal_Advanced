#include "OrExpression.h"

OrExpression::OrExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

PascalObject OrExpression::eval() const {
    return PascalObject((lhs->eval()) || (rhs->eval()));
}