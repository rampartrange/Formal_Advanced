//
// Created by Артем Андрианов on 25.12.2020.
//

#include "DeclarationList.h"

void DeclarationList::AddDeclaration(IdentList* declaration) {
    declarations.emplace_back(declaration);
}