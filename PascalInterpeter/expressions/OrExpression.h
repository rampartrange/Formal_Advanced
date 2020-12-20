#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class OrExpression: public Expression {
public:
    OrExpression(Expression* lhs, Expression* rhs);
    PascalObject eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};