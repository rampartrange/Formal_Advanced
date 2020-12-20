#pragma once
#include "Expression.h"

class NotExpression: public Expression {
public:
    NotExpression(Expression* lhs);
    int eval() const override;
private:
    Expression* lhs;
    int value;
};