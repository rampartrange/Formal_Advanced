#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"

class ObjectExpression: public Expression {
public:
    explicit ObjectExpression(int value);
    explicit ObjectExpression(double value);
    explicit ObjectExpression(std::string value);
    explicit ObjectExpression(bool value);

    PascalObject eval() const override;
private:
    PascalObject value;
};