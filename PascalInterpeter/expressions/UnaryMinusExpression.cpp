#include "UnaryMinusExpression.h"


UnaryMinusExpression::UnaryMinusExpression(Expression *lhs) : lhs(lhs) {}

int UnaryMinusExpression::eval() const{
    return -lhs->eval();
}