#pragma once
#include "Expression.h"
#include <string>

class IdentExpression: public Expression {
 public:
    IdentExpression(const std::string& ident, int value);
    int eval() const override;
 private:
    std::string ident;
    int value;
};