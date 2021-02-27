//
// Created by Артем Андрианов on 20.12.2020.
//

#include "PascalObject.h"

#include <utility>

PascalObject::PascalObject() : intValue(0),
                               doubleValue(0.0),
                               stringValue(""),
                               boolValue(false),
                               type(ObjectType::UNDEFINED) {};

PascalObject::PascalObject(int value) : intValue(value),
                                        doubleValue(0.0),
                                        stringValue(""),
                                        boolValue(false),
                                        type(ObjectType::INTEGER) {};

PascalObject::PascalObject(double value) : intValue(0),
                                           doubleValue(value),
                                           stringValue(""),
                                           boolValue(false),
                                           type(ObjectType::REAL) {};

PascalObject::PascalObject(std::string value) : intValue(0),
                                                doubleValue(0.0),
                                                stringValue(std::move(value)),
                                                boolValue(false),
                                                type(ObjectType::STRING) {};

PascalObject::PascalObject(bool value) : intValue(0),
                                         doubleValue(0.0),
                                         stringValue(""),
                                         boolValue(value),
                                         type(ObjectType::BOOLEAN) {};

PascalObject::PascalObject(ObjectType value) : intValue(0),
                                               doubleValue(0.0),
                                               stringValue(""),
                                               boolValue(false),
                                               type(value) {};

std::ostream& operator<<(std::ostream& out, const ObjectType& obj) {
    switch (obj) {
        case ObjectType::INTEGER: {
            out << "INTEGER";
            break;
        }
        case ObjectType::REAL: {
            out << "REAL";
            break;
        }
        case ObjectType::STRING: {
            out << "STRING";
            break;
        }
        case ObjectType::BOOLEAN: {
            out << "BOOLEAN";
            break;
        }
        default: {
            out << "UNDEFINED";
            break;
        }
    }
    return out;
}

bool AreTypesCorrect(const PascalObject& lhs, const PascalObject& rhs) {
    return lhs.GetType() == rhs.GetType();
}

int PascalObject::GetIntValue() const {
    return intValue;
}

double PascalObject::GetDoubleValue() const {
    return doubleValue;
}

std::string PascalObject::GetStringValue() const {
    return stringValue;
}

bool PascalObject::GetBoolValue() const {
    return boolValue;
}

ObjectType PascalObject::GetType() const {
    return type;
}


PascalObject operator-(const PascalObject& lhs) {
    ObjectType type = lhs.GetType();
    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(-lhs.GetIntValue());
        }
        case ObjectType::REAL : {
            return PascalObject(-lhs.GetDoubleValue());
        }
        default:
            return PascalObject();
    }
}

PascalObject operator+(const PascalObject& lhs, const PascalObject& rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return PascalObject();
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(lhs.GetIntValue() + rhs.GetIntValue());
        }
        case ObjectType::REAL : {
            return PascalObject(lhs.GetDoubleValue() + rhs.GetDoubleValue());
        }
        case ObjectType::STRING : {
            return PascalObject(lhs.GetStringValue().append(rhs.GetStringValue()));
        }
        default:
            return PascalObject();
    }

}
PascalObject operator-(const PascalObject &lhs, const PascalObject &rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return PascalObject();
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(lhs.GetIntValue() - rhs.GetIntValue());
        }
        case ObjectType::REAL : {
            return PascalObject(lhs.GetDoubleValue() - rhs.GetDoubleValue());
        }
        default:
            return PascalObject();
    }
}

PascalObject operator*(const PascalObject& lhs, const PascalObject& rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return PascalObject();
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(lhs.GetIntValue() * rhs.GetIntValue());
        }
        case ObjectType::REAL : {
            return PascalObject(lhs.GetDoubleValue() * rhs.GetDoubleValue());
        }
        default:
            return PascalObject();
    }
}

PascalObject operator/(const PascalObject& lhs, const PascalObject& rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return PascalObject();
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(lhs.GetIntValue() / rhs.GetIntValue());
        }
        case ObjectType::REAL : {
            return PascalObject(lhs.GetDoubleValue() / rhs.GetDoubleValue());
        }
        default:
            return PascalObject();
    }
}

PascalObject operator%(const PascalObject& lhs, const PascalObject& rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return PascalObject();
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return PascalObject(lhs.GetIntValue() % rhs.GetIntValue());
        }
        default:
            return PascalObject();
    }
}

bool operator< (const PascalObject& lhs, const PascalObject& rhs) {
    ObjectType type = lhs.GetType();
    if (!AreTypesCorrect(lhs, rhs)) {
        return false;
    }

    switch (type) {
        case ObjectType::INTEGER : {
            return lhs.GetIntValue() < rhs.GetIntValue();
        }
        case ObjectType::REAL : {
            return lhs.GetDoubleValue() < rhs.GetDoubleValue();
        }
        case ObjectType::STRING : {
            return lhs.GetStringValue() < rhs.GetStringValue();
        }
        default:
            return false;
    }
}
bool operator> (const PascalObject& lhs, const PascalObject& rhs) {
    return rhs < lhs;
}

bool operator<=(const PascalObject& lhs, const PascalObject& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const PascalObject& lhs, const PascalObject& rhs) {
    return !(lhs < rhs);
}

bool operator==(const PascalObject& lhs, const PascalObject& rhs) {
    return (lhs <= rhs) && (lhs >= rhs);
}

bool operator!=(const PascalObject& lhs, const PascalObject& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& out, const PascalObject& obj) {
    ObjectType type = obj.GetType();

    switch (type) {
        case ObjectType::INTEGER : {
            out << obj.GetIntValue();
            break;
        }
        case ObjectType::REAL : {
            out << obj.GetDoubleValue();
            break;
        }
        case ObjectType::STRING : {
            out << obj.GetStringValue();
            break;
        }
        case ObjectType::BOOLEAN : {
            out << (obj.GetBoolValue() ? "TRUE" : "FALSE");
            break;
        }
        default:
            out << "Incorrect type was in input";
            break;
    }
    return out;
}

void StrToLowerCase(std::string& input) {
    for (auto& symbol : input) {
        symbol = std::tolower(symbol);
    }
}

std::istream& operator>>(std::istream& in, PascalObject& obj) {
    std::string input;
    in >> input;
    StrToLowerCase(input);

    if (input == "false" || input == "true") {
        obj = PascalObject((input == "false") ? false : true);
        return in;
    }

    try {
        int intValue = std::stoi(input);
        double doubleValue = std::stod(input);

        if (doubleValue == static_cast<double>(intValue)) {
            obj = PascalObject(intValue);
        } else {
            obj = PascalObject(doubleValue);
        }
    } catch (const std::invalid_argument& error) {
        obj = PascalObject(input);
    }
    return in;
}

PascalObject::operator bool() const {
    bool result;
    switch (type) {
        case ObjectType::INTEGER : {
            result = GetIntValue() != 0;
            break;
        }
        case ObjectType::REAL : {
            result = GetDoubleValue() != 0.;
            break;
        }
        case ObjectType::STRING : {
            result = !GetStringValue().empty();
            break;
        }
        case ObjectType::BOOLEAN : {
            result = GetBoolValue();
            break;
        }
        default:
            result = false;
            break;
    }
    return result;
}