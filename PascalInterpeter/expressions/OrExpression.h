#pragma once
#include "Expression.h"

class OrExpression: public Expression {
public:
    OrExpression(Expression* lhs, Expression* rhs);
    int eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};