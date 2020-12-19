#pragma once
#include "Expression.h"

class ModExpression: public Expression {
public:
    ModExpression(Expression* lhs, Expression* rhs);
    int eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};