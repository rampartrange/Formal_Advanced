#pragma once

#include <string>
#include <iostream>

enum class ObjectType {
    INTEGER,
    REAL,
    STRING,
    BOOLEAN,
    UNDEFINED
};

std::ostream& operator<<(std::ostream& out, const ObjectType& obj);

enum class BooleanConstant : int {
    False = 0,
    True = 1
};

class PascalObject {
public:
    PascalObject();

    explicit PascalObject(int);
    explicit PascalObject(double);
    explicit PascalObject(std::string);
    explicit PascalObject(bool);
    explicit PascalObject(ObjectType);

    PascalObject(const PascalObject&) = default;

    PascalObject& operator=(const PascalObject&) = default;

    ObjectType GetType() const;

    int GetIntValue() const;
    double GetDoubleValue() const;
    std::string GetStringValue() const;
    bool GetBoolValue() const;

    friend PascalObject operator-(const PascalObject& lhs);
    friend PascalObject operator+(const PascalObject& lhs, const PascalObject& rhs);
    friend PascalObject operator-(const PascalObject& lhs, const PascalObject& rhs);
    friend PascalObject operator*(const PascalObject& lhs, const PascalObject& rhs);
    friend PascalObject operator/(const PascalObject& lhs, const PascalObject& rhs);
    friend PascalObject operator%(const PascalObject& lhs, const PascalObject& rhs);

    friend bool operator< (const PascalObject& lhs, const PascalObject& rhs);
    friend bool operator> (const PascalObject& lhs, const PascalObject& rhs);
    friend bool operator<=(const PascalObject& lhs, const PascalObject& rhs);
    friend bool operator>=(const PascalObject& lhs, const PascalObject& rhs);
    friend bool operator==(const PascalObject& lhs, const PascalObject& rhs);
    friend bool operator!=(const PascalObject& lhs, const PascalObject& rhs);

    friend std::ostream& operator<<(std::ostream& out, const PascalObject& obj);
    friend std::istream& operator>>(std::istream& in, PascalObject& obj);

    explicit operator bool() const;
private:

    int intValue;
    double doubleValue;
    std::string stringValue;
    bool boolValue;

    ObjectType type;
};

bool AreTypesCorrect(const PascalObject& lhs, const PascalObject& rhs);
