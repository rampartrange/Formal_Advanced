#include "OrExpression.h"

OrExpression::OrExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

PascalObject OrExpression::eval() const {
    return PascalObject(static_cast<bool>((lhs->eval()) || (rhs->eval())));
}