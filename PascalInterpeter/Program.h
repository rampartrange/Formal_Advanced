#pragma once

#include "assignments/AssignmentList.h"
#include "expressions/Expression.h"

class Program {
 public:
    Program(AssignmentList* assignments, Expression* expression);

    std::vector<Assignment *> GetAssigments() ;

 private:
    AssignmentList* assignments;
    Expression* expression;

};