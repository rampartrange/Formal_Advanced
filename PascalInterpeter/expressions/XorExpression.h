#pragma once
#include "Expression.h"

class XorExpression: public Expression {
public:
    XorExpression(Expression* lhs, Expression* rhs);
    int eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};