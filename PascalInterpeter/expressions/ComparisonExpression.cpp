#include "ComparisonExpression.h"

ComparisonExpression::ComparisonExpression(Expression* lhs,
                                           Expression* rhs,
                                           const std::string& sign) :
                                           lhs(lhs),
                                           rhs(rhs),
                                           type(DefineComparisonType(sign)),
                                           value(ComputeValue()) {}

ComparisonType ComparisonExpression::DefineComparisonType(const std::string &sign) const{
    ComparisonType comparisonType;
    if (sign == "<") {
        comparisonType = ComparisonType::LT;
    } else if (sign == ">") {
        comparisonType = ComparisonType::GT;
    } else if (sign == "<=") {
        comparisonType = ComparisonType::LE;
    } else if (sign == ">=") {
        comparisonType = ComparisonType::GE;
    } else if (sign == "=") {
        comparisonType = ComparisonType::EQ;
    } else {
        comparisonType = ComparisonType::NE;
    }
    return comparisonType;
}

bool ComparisonExpression::ComputeValue() {
    bool result = false;
    switch (type) {
        case ComparisonType::LT : {
            result = lhs->eval() < rhs->eval();
            break;
        }
        case ComparisonType::GT : {
            result = lhs->eval() > rhs->eval();
            break;
        }
        case ComparisonType::LE : {
            result = lhs->eval() <= rhs->eval();
            break;
        }
        case ComparisonType::GE : {
            result = lhs->eval() >= rhs->eval();
            break;
        }
        case ComparisonType::EQ : {
            result = lhs->eval() == rhs->eval();
            break;
        }
        default : {
            result = lhs->eval() != rhs->eval();
            break;
        }
    }
    return result;
}


PascalObject ComparisonExpression::eval() const {
    return value;
}