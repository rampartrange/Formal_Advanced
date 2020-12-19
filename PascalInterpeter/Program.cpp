#include "Program.h"

Program::Program(
    AssignmentList* assignments,
    Expression* expression
): assignments_(assignments), expression_(expression) {}

std::vector<Assignment *> Program::GetAssigments() {
    return assignments_->GetAssigments();
}