#pragma once
#include "Expression.h"
#include <iostream>
#include "../objects/PascalObject.h"

class MulExpression: public Expression {
 public:
    MulExpression(Expression* lhs, Expression* rhs);
    PascalObject eval() const override;
 private:
    Expression* lhs;
    Expression* rhs;
};