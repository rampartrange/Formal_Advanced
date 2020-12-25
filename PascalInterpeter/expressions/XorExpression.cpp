#include "XorExpression.h"

XorExpression::XorExpression(
        Expression *lhs, Expression *rhs
): lhs(lhs), rhs(rhs) {}

PascalObject XorExpression::eval() const {
    return PascalObject(static_cast<bool>(static_cast<bool>(lhs->eval()) ^ static_cast<bool>(rhs->eval())));
}