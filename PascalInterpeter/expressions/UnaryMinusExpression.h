#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class UnaryMinusExpression: public Expression {
public:
    UnaryMinusExpression(Expression* lhs);
    PascalObject eval() const override;
private:
    Expression* lhs;
    int value;
};