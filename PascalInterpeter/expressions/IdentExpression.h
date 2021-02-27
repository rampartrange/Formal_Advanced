#pragma once
#include "Expression.h"
#include "../objects/PascalObject.h"
#include <string>

class IdentExpression: public Expression {
 public:
    IdentExpression(std::string  ident, PascalObject value);
    PascalObject eval() const override;
 private:
    std::string ident;
    PascalObject value;
};