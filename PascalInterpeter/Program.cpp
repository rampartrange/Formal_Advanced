#include "Program.h"

Program::Program(
    AssignmentList* assignments,
    Expression* expression,
    DeclarationList* declarations
): assignments(assignments), expression(expression), declarations(declarations){}

std::vector<AssignmentExpression *> Program::GetAssigments() {
    return assignments->GetAssigments();
}