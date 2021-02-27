#pragma once

#include "assignments/AssignmentList.h"
#include "expressions/Expression.h"
#include "assignments/DeclarationList.h"

class Program {
 public:
    Program(AssignmentList* assignments, Expression* expression, DeclarationList* declarations);

    std::vector<AssignmentExpression *> GetAssigments() ;

 private:
    AssignmentList* assignments;
    Expression* expression;
    DeclarationList* declarations;

};