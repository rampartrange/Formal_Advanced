#pragma once

#include <vector>
#include <string>
#include "../objects/PascalObject.h"

class IdentList {
public:
    void AddIdent(const std::string&);
    ObjectType GetType() const;
    void SetType(const std::string&);

    const std::vector<std::string>& GetIdentList() const;
private:
    std::vector<std::string> identList;
    ObjectType type;
};