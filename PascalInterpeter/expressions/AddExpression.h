#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class AddExpression: public Expression {
 public:
    AddExpression(Expression* lhs, Expression* rhs);
    PascalObject eval() const override;
 private:
    Expression* lhs;
    Expression* rhs;
};