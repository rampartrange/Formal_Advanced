#pragma once
#include "expressions/Expression.h"
#include <string>

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
    int eval() const override;
private:
    ComparisonType DefineComparisonType(const std::string& sign) const;
    int ComputeValue();

    Expression* rhs;
    Expression* lhs;
    ComparisonType type;

    int value;
};