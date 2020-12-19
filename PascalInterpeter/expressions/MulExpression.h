#pragma once
#include "Expression.h"
#include <iostream>

class MulExpression: public Expression {
 public:
    MulExpression(Expression* lhs, Expression* rhs);
    int eval() const override;
 private:
    Expression* lhs;
    Expression* rhs;
};