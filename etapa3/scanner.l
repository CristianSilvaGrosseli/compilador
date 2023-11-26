%{
// Entrega 2
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include "asd.h"
#include "parser.tab.h"
%}

%option yylineno
%option noyywrap
%x COMMENT

%%

int                     { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_INT; }
float                   { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_FLOAT; }
bool                    { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_BOOL; }
if                      { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_IF; }
else                    { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_ELSE; }
while                   { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_WHILE; }
return                  { yylval.lexical_value = lexical_value_create(TYPE, yytext); return TK_PR_RETURN; }

-                       { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
\+                      { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
!                       { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
\*                      { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
\/                      { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
%                       { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
\<                      { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
=                       { yylval.lexical_value = lexical_value_create(TYPE, yytext); return yytext[0]; }
>                       { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
\{                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
\}                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
\(                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
\)                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
,                       { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }
;                       { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return yytext[0]; }

&                       { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_AND; }
\|                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_OR; }

\<=                     { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_LE; }
==                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_EQ; }
>=                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_GE; }
!=                      { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_OC_NE; }

false                   { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_LIT_FALSE; }
true                    { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_LIT_TRUE; }
(_|[a-z])[_a-z0-9]*	    { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_IDENTIFICADOR; }
[0-9]+                  { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_LIT_INT; }
[0-9]*(\.[0-9])[0-9]*   { yylval.lexical_value = lexical_value_create(TYPE, yytext);  return TK_LIT_FLOAT; }

"\n"
[ \t]

\/\*                    { BEGIN(COMMENT); }
\/\/.*

<COMMENT>"\*\/"         { BEGIN(INITIAL); }
<COMMENT>"\n"
<COMMENT>.

.                       { return TK_ERRO; };

%%