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
    class ObjectExpression;
    class AddExpression;
    class SubstractExpression;
    class DivExpression;
    class IdentExpression;
    class UnaryMinusExpression;
    
    class ComparisonExpression;
    class AndExpression;
    class OrExpression;
    class XorExpression;
    class NotExpression;

    class PascalObject;

    class AssignmentExpression;
    class AssignmentList;
    class DeclarationList;
    class IdentList;

    class Program;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"

    #include "expressions/ObjectExpression.h"
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
    #include "expressions/NotExpression.h"

    #include "objects/PascalObject.h"

    #include "expressions/AssignmentExpression.h"
    #include "assignments/AssignmentList.h"
    #include "assignments/DeclarationList.h"
    #include "assignments/IdentList.h"

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
    EOF 0       "end of file"
    
    BEGIN       "begin"
    END         "end"

    VAR         "var"

    ASSIGN      ":="
    
    MINUS       "-"
    PLUS        "+"
    STAR        "*"
    DIV         "/"
    MOD         "%"
    
    AND         "&&"
    OR          "||"
    XOR         "^"
    NOT         "!"

    LPAREN      "("
    RPAREN      ")"

    SEMICOLON   ";"
    POINT       "."
    COMMA       ","i
    COLON       ":"
;

%token <std::string> 
    IDENTIFIER "identifier"
    TYPE       "type"

%token <int> INTEGER "integer"
%token <double> REAL "real"
%token <std::string> STRING "string"
%token <bool> BOOL "bool"

%nterm <Expression*> exp
%nterm <AssignmentExpression*> assignment

%nterm <DeclarationList*> DeclBlock
%nterm <AssignmentList*> MainBlock

%nterm <AssignmentList*> assignments
%nterm <DeclarationList*> declarations
%nterm <IdentList*>  IdentListSeq
%nterm <IdentList*>  IdentList

%nterm <Program*> unit

%token <std::string> CMP
// %printer { yyo << $$; } <*>;

%nonassoc CMP
%right ":="
%left "+" "-" "||" "^"
%left "*" "/" "%" "&&"
%nonassoc UMINUS
%nonassoc "!"


%%
%start unit;

unit : DeclBlock MainBlock  EOF {$$ = new Program($2, NULL, $1); driver.program = $$; };

DeclBlock:
    %empty { $$ = NULL; }
    | "var" declarations { $$ = $2; };

declarations:
    %empty { $$  = new DeclarationList(); }
    | declarations IdentList ":" "type" ";" {
        $2->SetType($4);
        for (const auto& name : $2->GetIdentList()) {
            std::cout << name << ": " << $2->GetType() << std::endl;
        }
        $1->AddDeclaration($2);
        $$ = $1;
    };

IdentList: 
    | "identifier" IdentListSeq {
        $2->AddIdent($1);
        $$ = $2;
    };

IdentListSeq:
    %empty { $$ = new IdentList(); }
    | IdentListSeq "," "identifier" {
        $1->AddIdent($3);
        $$ = $1;
    };


MainBlock: "begin" assignments "end" "."  { $$ = $2; };

assignments:
    %empty { $$ = new AssignmentList(); /* A -> eps */}
    | assignments assignment {
        $1->AddAssignment($2); $$ = $1;
    };


assignment:
    "identifier" ":=" exp ";"{
        $$ = new AssignmentExpression($1, $3);
        driver.variables[$1] = $3->eval(); 
    };


exp:
    exp CMP exp { $$ = new ComparisonExpression($1, $3, $2); }
    | "integer" { $$ = new ObjectExpression($1); }
    | "real"    { $$ = new ObjectExpression($1); }
    | "string"  { $$ = new ObjectExpression($1); }
    | "bool"    { $$ = new ObjectExpression($1); }
    | "identifier" {$$ = new IdentExpression($1, driver.variables[$1]); }
    | exp "+" exp { $$ = new AddExpression($1, $3); }
    | exp "-" exp { $$ = new SubstractExpression($1, $3); }
    | exp "*" exp { $$ = new MulExpression($1, $3); }
    | exp "/" exp { $$ = new DivExpression($1, $3); }
    | exp "%" exp { $$ = new ModExpression($1, $3); }
    | exp "&&" exp { $$ = new AndExpression($1, $3); }
    | exp "||" exp { $$ = new OrExpression($1, $3); }
    | exp "^" exp   { $$ = new XorExpression($1, $3); }
    | "!" exp       { $$ = new NotExpression($2); }
    | "(" exp ")" { $$ = $2; };
    | "-" exp %prec UMINUS { $$ = new UnaryMinusExpression($2); };

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
