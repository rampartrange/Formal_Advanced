cmake_minimum_required(VERSION 3.15)
project(PascalInterpreter)

set (   CMAKE_CXX_STANDARD 11)

set(
        HEADERS
        driver.hh
        parser.hh
    )
find_package(BISON REQUIRED)
BISON_TARGET(
        MyParser
        parser.y
        ${PascalInterpreter_SOURCE_DIR}/parser.cpp
        DEFINES_FILE ${PascalInterpreter_SOURCE_DIR}/parser.hh
)
find_package(FLEX REQUIRED)
FLEX_TARGET(
        MyScanner
        scanner.l
        ${PascalInterpreter_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(
        PascalInterpreter
        main.cpp
        driver.cpp
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        expressions/ObjectExpression.cpp
        expressions/MulExpression.cpp
        expressions/AddExpression.cpp
        expressions/SubstractExpression.cpp
        expressions/UnaryMinusExpression.cpp
        expressions/DivExpression.cpp
        expressions/ModExpression.cpp
        expressions/IdentExpression.cpp
        expressions/ComparisonExpression.cpp
        expressions/AndExpression.cpp
        expressions/OrExpression.cpp
        expressions/XorExpression.cpp
        expressions/NotExpression.cpp
        expressions/AssignmentExpression.cpp
        assignments/AssignmentList.cpp
        objects/PascalObject.cpp
        Program.cpp
        assignments/DeclarationList.cpp
        assignments/IdentList.cpp)

target_include_directories(
        PascalInterpreter PRIVATE ${PascalInterpreter_SOURCE_DIR}
)



