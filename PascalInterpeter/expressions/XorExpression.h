#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class XorExpression: public Expression {
public:
    XorExpression(Expression* lhs, Expression* rhs);
    PascalObject eval() const override;
private:
    Expression* lhs;
    Expression* rhs;
};