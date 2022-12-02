%{

#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.hpp"
#include "hw3_output.hpp"

%}

%option yylineno
%option noyywrap

%%

[ \t\r\n]*                         ;
\/\/[^\r\n]*[\r|\n|\r\n]?          ;
void                               return VOID;
int                                return INT;
byte                               return BYTE;
b                                  return B;
bool                               return BOOL;
and                                return AND;
or                                 return OR;
not                                return NOT;
true                               return TRUE;
false                              return FALSE;
return                             return RETURN;
if                                 return IF;
else                               return ELSE;
while                              return WHILE;
break                              return BREAK;
continue                           return CONTINUE;
;                                  return SC;
,                                  return COMMA;
\(                                 return LPAREN;
\)                                 return RPAREN;
\{                                 return LBRACE;
\}                                 return RBRACE;
=                                  return ASSIGN;
==|!=                              return EQOP;
\<|>|<=|>=                         return RELOP;
\+|\-                              return ADDOP;
\*|\/                              return MULOP;
[a-zA-Z][a-zA-Z0-9]*               return ID;
0|[1-9][0-9]*                      return NUM;
\"([^\n\r\"\\]|\\[rnt"\\])+\"      return STRING;
.                                  { output::errorLex(yylineno); exit(0); }

%%
