%{

#include <stdlib.h>
#include <string>
#include "parser.tab.hpp"
#include "hw3_output.hpp"
#include "syntax_token.hpp"

%}

%option yylineno
%option noyywrap

%%

[ \t\r\n]*                         { ; }
\/\/[^\r\n]*[\r|\n|\r\n]?          { ; }
void                               { yylval.token = new syntax_token(VOID, yylineno, yytext); return VOID; }
int                                { yylval.token = new syntax_token(INT, yylineno, yytext); return INT; }
byte                               { yylval.token = new syntax_token(BYTE, yylineno, yytext); return BYTE; }
b                                  { yylval.token = new syntax_token(B, yylineno, yytext); return B; }
bool                               { yylval.token = new syntax_token(BOOL, yylineno, yytext); return BOOL; }
and                                { yylval.token = new syntax_token(AND, yylineno, yytext); return AND; }
or                                 { yylval.token = new syntax_token(OR, yylineno, yytext); return OR; }
not                                { yylval.token = new syntax_token(NOT, yylineno, yytext); return NOT; }
true                               { yylval.token = new syntax_token(TRUE, yylineno, yytext); return TRUE; }
false                              { yylval.token = new syntax_token(FALSE, yylineno, yytext); return FALSE; }
return                             { yylval.token = new syntax_token(RETURN, yylineno, yytext); return RETURN; }
if                                 { yylval.token = new syntax_token(IF, yylineno, yytext); return IF; }
else                               { yylval.token = new syntax_token(ELSE, yylineno, yytext); return ELSE; }
while                              { yylval.token = new syntax_token(WHILE, yylineno, yytext); return WHILE; }
break                              { yylval.token = new syntax_token(BREAK, yylineno, yytext); return BREAK; }
continue                           { yylval.token = new syntax_token(CONTINUE, yylineno, yytext); return CONTINUE; }
;                                  { return SC; }
,                                  { return COMMA; }
\(                                 { return LPAREN; }
\)                                 { return RPAREN; }
\{                                 { return LBRACE; }
\}                                 { return RBRACE; }
=                                  { yylval.token = new syntax_token(ASSIGN, yylineno, yytext); return ASSIGN; }
==|!=                              { yylval.token = new syntax_token(EQOP, yylineno, yytext); return EQOP; }
\<|>|<=|>=                         { yylval.token = new syntax_token(RELOP, yylineno, yytext); return RELOP; }
\+|\-                              { yylval.token = new syntax_token(ADDOP, yylineno, yytext); return ADDOP; }
\*|\/                              { yylval.token = new syntax_token(MULOP, yylineno, yytext); return MULOP; }
[a-zA-Z][a-zA-Z0-9]*               { yylval.token = new syntax_token(ID, yylineno, yytext); return ID; }
\"([^\n\r\"\\]|\\[rnt"\\])+\"      { yylval.token = new syntax_token(STRING, yylineno, yytext); return STRING; }
0|[1-9][0-9]*                      { yylval.num_token = new numeric_syntax_token(NUM, yylineno, yytext, std::stoi(yytext)); return NUM; }
.                                  { output::errorLex(yylineno); }

%%
