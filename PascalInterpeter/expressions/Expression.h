#pragma once
#include "../objects/PascalObject.h"

class Expression {
public:
    virtual PascalObject eval() const = 0;
};