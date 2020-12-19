cmake_minimum_required(VERSION 3.15)
project(ParserExampleWithAst)

set (   CMAKE_CXX_STANDARD 11)

set(
        HEADERS
        driver.hh
        parser.hh
    )
find_package(BISON REQUIRED)
BISON_TARGET(
        MyParserWithAst
        parser.y
        ${ParserExampleWithAst_SOURCE_DIR}/parser.cpp
        DEFINES_FILE ${ParserExampleWithAst_SOURCE_DIR}/parser.hh
)
find_package(FLEX REQUIRED)
FLEX_TARGET(
        MyScannerWithAst
        scanner.l
        ${ParserExampleWithAst_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScannerWithAst MyParserWithAst)

add_executable(
        ParserExampleWithAst
        main.cpp
        driver.cpp
        ${BISON_MyParserWithAst_OUTPUTS}
        ${FLEX_MyScannerWithAst_OUTPUTS}
        expressions/NumberExpression.cpp
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
        assignments/Assignment.cpp
        assignments/AssignmentList.cpp
        Program.cpp
)

target_include_directories(
        ParserExampleWithAst PRIVATE ${ParserExampleWithAst_SOURCE_DIR}
)



