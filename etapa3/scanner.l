%{
// Entrega 3
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

int                     { yylval.valor_lexico = lexical_value_create(TYPE, yytext); return TK_PR_INT; }
float                   { yylval.valor_lexico = lexical_value_create(TYPE, yytext); return TK_PR_FLOAT; }
bool                    { yylval.valor_lexico = lexical_value_create(TYPE, yytext); return TK_PR_BOOL; }
if                      { yylval.valor_lexico = lexical_value_create(CONTROL, yytext); return TK_PR_IF; }
else                    { yylval.valor_lexico = lexical_value_create(CONTROL, yytext); return TK_PR_ELSE; }
while                   { yylval.valor_lexico = lexical_value_create(CONTROL, yytext); return TK_PR_WHILE; }
return                  { yylval.valor_lexico = lexical_value_create(CONTROL, yytext); return TK_PR_RETURN; }

-                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
\+                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
!                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
\*                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
\/                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
%                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
\<                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
=                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext); return yytext[0]; }
>                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return yytext[0]; }
\{                      { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }
\}                      { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }
\(                      { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }
\)                      { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }
,                       { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }
;                       { yylval.valor_lexico = lexical_value_create(EXPRESSION, yytext);  return yytext[0]; }

&                       { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_AND; }
\|                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_OR; }

\<=                     { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_LE; }
==                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_EQ; }
>=                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_GE; }
!=                      { yylval.valor_lexico = lexical_value_create(OPERATOR, yytext);  return TK_OC_NE; }

false                   { yylval.valor_lexico = lexical_value_create(LITERAL, yytext);  return TK_LIT_FALSE; }
true                    { yylval.valor_lexico = lexical_value_create(LITERAL, yytext);  return TK_LIT_TRUE; }
(_|[a-z])[_a-z0-9]*	    { yylval.valor_lexico = lexical_value_create(IDENTIFIER, yytext);  return TK_IDENTIFICADOR; }
[0-9]+                  { yylval.valor_lexico = lexical_value_create(LITERAL, yytext);  return TK_LIT_INT; }
[0-9]*(\.[0-9])[0-9]*   { yylval.valor_lexico = lexical_value_create(LITERAL, yytext);  return TK_LIT_FLOAT; }

"\n"
[ \t]

\/\*                    { BEGIN(COMMENT); }
\/\/.*

<COMMENT>"\*\/"         { BEGIN(INITIAL); }
<COMMENT>"\n"
<COMMENT>.

.                       { return TK_ERRO; };

%%