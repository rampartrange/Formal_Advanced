#pragma once

#include "assignments/Assignment.h"

#include <vector>

class AssignmentList {
 public:
    void AddAssignment(Assignment* assignment);
    std::vector<Assignment*> GetAssigments();
 private:
    std::vector<Assignment*> assignments_;
};

