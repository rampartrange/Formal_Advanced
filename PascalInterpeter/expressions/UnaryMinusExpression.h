#pragma once
#include "Expression.h"

class UnaryMinusExpression: public Expression {
public:
    UnaryMinusExpression(Expression* lhs);
    int eval() const override;
private:
    Expression* lhs;
    int value;
};