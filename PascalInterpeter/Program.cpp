#include "Program.h"

Program::Program(
    AssignmentList* assignments,
    Expression* expression
): assignments(assignments), expression(expression) {}

std::vector<Assignment *> Program::GetAssigments() {
    return assignments->GetAssigments();
}