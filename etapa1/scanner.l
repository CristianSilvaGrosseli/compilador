%{
// Entrega 1
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include "tokens.h"
#include "helpers.h"
%}

%option yylineno
%option noyywrap
%x COMMENT

%%

int                     { return TK_PR_INT; }
float                   { return TK_PR_FLOAT; }
bool                    { return TK_PR_BOOL; }
if                      { return TK_PR_IF; }
else                    { return TK_PR_ELSE; }
while                   { return TK_PR_WHILE; }
return                  { return TK_PR_RETURN; }

-                       { return yytext[0]; }
\+                       { return yytext[0]; }
!                       { return yytext[0]; }
\*                      { return yytext[0]; }
\/                      { return yytext[0]; }
%                       { return yytext[0]; }
\<                      { return yytext[0]; }
=                       { return yytext[0]; }
>                       { return yytext[0]; }
\{                      { return yytext[0]; }
\}                      { return yytext[0]; }
\(                      { return yytext[0]; }
\)                      { return yytext[0]; }
,                       { return yytext[0]; }
;                       { return yytext[0]; }

&                       { return TK_OC_AND; }
\|                      { return TK_OC_OR; }

\<=                     { return TK_OC_LE; }
==                      { return TK_OC_EQ; }
>=                      { return TK_OC_GE; }
!=                      { return TK_OC_NE; }

false                   { return TK_LIT_FALSE; }
true                    { return TK_LIT_TRUE; }
(_|[a-z])[_a-z0-9]*	    { return TK_IDENTIFICADOR; }
[0-9]+                  { return TK_LIT_INT; }
[0-9]*(\.[0-9])[0-9]*   { return TK_LIT_FLOAT; }

"\n"
[ \t]

\/\*                    { BEGIN(COMMENT); }
\/\/.*

<COMMENT>"\*\/"         { BEGIN(INITIAL); }
<COMMENT>"\n"
<COMMENT>.

.                       { return TK_ERRO; };

%%