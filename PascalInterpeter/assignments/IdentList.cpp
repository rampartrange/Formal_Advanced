#include "IdentList.h"

void IdentList::AddIdent(const std::string& name) {
    identList.push_back(name);
}

ObjectType IdentList::GetType() const {
    return type;
}

void IdentList::SetType(const std::string& objectType) {
    if (objectType == "integer") {
        type = ObjectType::INTEGER;
    } else if (objectType == "boolean") {
        type = ObjectType::BOOLEAN;
    } else if (objectType == "real") {
        type = ObjectType::REAL;
    } else if (objectType == "string") {
        type = ObjectType::STRING;
    }

}

const std::vector<std::string>& IdentList::GetIdentList() const {
    return identList;
}