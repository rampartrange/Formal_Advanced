#pragma once
#include "Expression.h"

class AndExpression: public Expression {
public:
    AndExpression(Expression* lhs, Expression* rhs);
    int eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};