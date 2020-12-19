%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
    class Expression;
    class NumberExpression;
    class AddExpression;
    class SubstractExpression;
    class DivExpression;
    class IdentExpression;
    class UnaryMinusExpression;
    
    class ComparisonExpression;
    class AndExpression;
    class OrExpression;
    class XorExpression;

    class Assignment;
    class AssignmentList;

    class Program;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"

    #include "expressions/NumberExpression.h"
    #include "expressions/AddExpression.h"
    #include "expressions/MulExpression.h"
    #include "expressions/DivExpression.h"
    #include "expressions/SubstractExpression.h"
    #include "expressions/IdentExpression.h"
    #include "expressions/ModExpression.h"
    #include "expressions/UnaryMinusExpression.h"
    
    #include "expressions/ComparisonExpression.h"
    #include "expressions/AndExpression.h"
    #include "expressions/OrExpression.h"
    #include "expressions/XorExpression.h"

    #include "assignments/Assignment.h"
    #include "assignments/AssignmentList.h"
    #include "Program.h"

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    END 0 "end of file"
    
    ASSIGN ":="
    
    MINUS "-"
    PLUS "+"
    STAR "*"
    DIV  "/"
    MOD  "%"
    
    AND "&&"
    OR  "||"
    XOR "^"

    LPAREN "("
    RPAREN ")"i
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <Expression*> exp
%nterm <Assignment*> assignment
%nterm <AssignmentList*> assignments
%nterm <Program*> unit

%token <std::string> CMP
// %printer { yyo << $$; } <*>;

%nonassoc CMP
%right ":="
%left "+" "-" "||" "^"
%left "*" "/" "%" "&&"
%nonassoc UMINUS


%start unit
%%

unit: assignments exp { $$ = new Program($1, $2); driver.program = $$; };

assignments:
    %empty { $$ = new AssignmentList(); /* A -> eps */}
    | assignments assignment {
        $1->AddAssignment($2); $$ = $1;
    };


assignment:
    "identifier" ":=" exp {
        $$ = new Assignment($1, $3);
        driver.variables[$1] = $3->eval(); 
    };


exp:
      exp CMP exp { $$ = new ComparisonExpression($1, $3, $2); }
    | "number" {$$ = new NumberExpression($1); }
    | "identifier" {$$ = new IdentExpression($1, driver.variables[$1]); }
    | exp "+" exp { $$ = new AddExpression($1, $3); }
    | exp "-" exp { $$ = new SubstractExpression($1, $3); }
    | exp "*" exp { $$ = new MulExpression($1, $3); }
    | exp "/" exp { $$ = new DivExpression($1, $3); }
    | exp "%" exp { $$ = new ModExpression($1, $3); }
    | exp "&&" exp { $$ = new AndExpression($1, $3); }
    | exp "||" exp { $$ = new OrExpression($1, $3); }
    | exp "^" exp   { $$ = new XorExpression($1, $3); }
    | "(" exp ")" { $$ = $2; };
    | "-" exp %prec UMINUS { $$ = new UnaryMinusExpression($2); }

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
