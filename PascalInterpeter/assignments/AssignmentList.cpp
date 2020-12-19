#include "AssignmentList.h"


void AssignmentList::AddAssignment(Assignment* assignment) {
    assignments_.push_back(assignment);
}

std::vector<Assignment*> AssignmentList::GetAssigments() {
    return assignments_;
}