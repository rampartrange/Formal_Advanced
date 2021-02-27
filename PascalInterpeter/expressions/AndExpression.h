#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class AndExpression: public Expression {
public:
    AndExpression(Expression* lhs, Expression* rhs);
    PascalObject eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};