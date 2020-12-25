#include "AssignmentList.h"


void AssignmentList::AddAssignment(AssignmentExpression* assignment) {
    assignments_.push_back(assignment);
}

std::vector<AssignmentExpression*> AssignmentList::GetAssigments() {
    return assignments_;
}