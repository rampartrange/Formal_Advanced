#include "NotExpression.h"

NotExpression::NotExpression(Expression* lhs) : lhs(lhs) {}

int NotExpression::eval() const {
    return !(lhs->eval());
}