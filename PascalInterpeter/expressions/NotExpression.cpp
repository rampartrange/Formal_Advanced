#include "NotExpression.h"

NotExpression::NotExpression(Expression* lhs) : lhs(lhs) {}

PascalObject NotExpression::eval() const {
    return PascalObject(!(lhs->eval()));
}