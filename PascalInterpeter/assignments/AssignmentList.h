#pragma once

#include "expressions/AssignmentExpression.h"

#include <vector>

class AssignmentList {
 public:
    void AddAssignment(AssignmentExpression* assignment);
    std::vector<AssignmentExpression*> GetAssigments();
 private:
    std::vector<AssignmentExpression*> assignments_;
};

