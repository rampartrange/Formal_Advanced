#include "AddExpression.h"

AddExpression::AddExpression(Expression *lhs, Expression *rhs): lhs(lhs), rhs(rhs) {}

PascalObject AddExpression::eval() const {
    return lhs->eval() + rhs->eval();
}