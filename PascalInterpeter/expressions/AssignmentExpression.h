#pragma once

#include "expressions/Expression.h"

#include <string>

class AssignmentExpression {
 public:
    AssignmentExpression(std::string  variable, Expression* expression);
 private:
    std::string variable_;
    Expression* expression_;
};