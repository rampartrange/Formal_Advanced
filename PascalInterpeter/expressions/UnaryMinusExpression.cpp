#include "UnaryMinusExpression.h"


UnaryMinusExpression::UnaryMinusExpression(Expression *lhs) : lhs(lhs) {}

PascalObject UnaryMinusExpression::eval() const{
    return -lhs->eval();
}