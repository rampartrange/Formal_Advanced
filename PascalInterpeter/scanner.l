%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "driver.hh"
    #include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type make_INTEGER(
    const std::string &s,
    const yy::parser::location_type& loc
  );
  yy::parser::symbol_type make_REAL(
    const std::string &s,
    const yy::parser::location_type& loc
  );
  yy::parser::symbol_type make_BOOL(
    const std::string &s,
    const yy::parser::location_type& loc
  ); 
  yy::parser::symbol_type make_STRING(
    const std::string &s,
    const yy::parser::location_type& loc
  ); 
%}

bool  true|false
id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]
real  {int}"."{int}([eE][+-]{int})?
string "\'"([^\n\'\\]*(\\[.\n])*)*"\'"
cmp   "<"|">"|"<="|">="|"="|"<>"
type  integer|boolean|real|string


%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
  // Code run each time yylex is called.
  loc.step();
%}

{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();

"begin"    return yy::parser::make_BEGIN  (loc);
"end"      return yy::parser::make_END    (loc);

"var"      return yy::parser::make_VAR    (loc);

"-"        return yy::parser::make_MINUS  (loc);       
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"div"      return yy::parser::make_DIV    (loc);
"mod"      return yy::parser::make_MOD    (loc);

"and"      return yy::parser::make_AND    (loc);
"or"       return yy::parser::make_OR     (loc);
"xor"      return yy::parser::make_XOR    (loc);
"not"      return yy::parser::make_NOT    (loc);

"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);

":="       return yy::parser::make_ASSIGN (loc);

","        return yy::parser::make_COMMA  (loc);
"."        return yy::parser::make_POINT (loc);
";"        return yy::parser::make_SEMICOLON (loc);
":"        return yy::parser::make_COLON  (loc);

{bool}     return make_BOOL(yytext, loc);
{type}     return yy::parser::make_TYPE(yytext, loc);
{cmp}      return yy::parser::make_CMP    (yytext, loc);
{id}       return yy::parser::make_IDENTIFIER(yytext, loc);

{int}      return make_INTEGER(yytext, loc);
{real}     return make_REAL(yytext, loc);
{string}   return make_STRING(yytext, loc);
.          {
                throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_EOF (loc);
%%

yy::parser::symbol_type make_INTEGER(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  long n = strtol(s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
  return yy::parser::make_INTEGER((int) n, loc);
}

yy::parser::symbol_type make_REAL(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  double n = stod(s);
  return yy::parser::make_REAL(n, loc);
}

yy::parser::symbol_type make_BOOL(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  bool n = (s == "true") ? true : false;
  return yy::parser::make_BOOL(n, loc);
}

yy::parser::symbol_type make_STRING(
  const std::string& s,
  const yy::parser::location_type& loc
) {
  int size = s.length();
  return yy::parser::make_STRING(s.substr(1, size - 2), loc);
}
