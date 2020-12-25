#pragma once

#include "../objects/PascalObject.h"
#include "IdentList.h"

class DeclarationList {
public:
    void AddDeclaration(IdentList* );
private:
    std::vector<IdentList*> declarations;
};