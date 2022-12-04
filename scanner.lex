%{

#include <stdlib.h>
#include <string>
#include "parser.tab.hpp"
#include "hw3_output.hpp"

%}

%option yylineno
%option noyywrap

%%

[ \t\r\n]*                         { ; }
\/\/[^\r\n]*[\r|\n|\r\n]?          { ; }
void                               { return VOID; }
int                                { return INT; }
byte                               { return BYTE; }
b                                  { return B; }
bool                               { return BOOL; }
and                                { return AND; }
or                                 { return OR; }
not                                { return NOT; }
true                               { return TRUE; }
false                              { return FALSE; }
return                             { return RETURN; }
if                                 { return IF; }
else                               { return ELSE; }
while                              { return WHILE; }
break                              { return BREAK; }
continue                           { return CONTINUE; }
;                                  { return SC; }
,                                  { return COMMA; }
\(                                 { return LPAREN; }
\)                                 { return RPAREN; }
\{                                 { return LBRACE; }
\}                                 { return RBRACE; }
=                                  { return ASSIGN; }
==|!=                              { yylval.str = strdup(yytext); return EQOP; }
\<|>|<=|>=                         { yylval.str = strdup(yytext); return RELOP; }
\+|\-                              { yylval.str = strdup(yytext); return ADDOP; }
\*|\/                              { yylval.str = strdup(yytext); return MULOP; }
[a-zA-Z][a-zA-Z0-9]*               { yylval.str = strdup(yytext); return ID; }
0|[1-9][0-9]*                      { yylval.num = std::stoi(yytext); return NUM; }
\"([^\n\r\"\\]|\\[rnt"\\])+\"      { yylval.str = strdup(yytext); return STRING; }
.                                  { output::errorLex(yylineno); exit(0); }

%%
