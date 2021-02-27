#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class NotExpression: public Expression {
public:
    NotExpression(Expression* lhs);
    PascalObject eval() const override;
private:
    Expression* lhs;
};