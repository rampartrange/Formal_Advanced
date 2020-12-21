#pragma once
#include "expressions/Expression.h"
#include <string>
#include "../objects/PascalObject.h"

enum class ComparisonType {
    LT = 1,
    GT,
    LE,
    GE,
    EQ,
    NE
};


class ComparisonExpression: public Expression {
public:
    ComparisonExpression(Expression* lhs, Expression* rhs, const std::string& sign);
    PascalObject eval() const override;
private:
    ComparisonType DefineComparisonType(const std::string& sign) const;
    bool ComputeValue();

    Expression* rhs;
    Expression* lhs;
    ComparisonType type;

    PascalObject value;
};