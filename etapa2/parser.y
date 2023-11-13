%{
// Entrega 2
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
int yylex(void);
void yyerror (char const *mensagem);
%}

%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_BOOL
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_LIT_FALSE
%token TK_LIT_TRUE
%token TK_ERRO

%%

program: list
    |
    ;

list:  list element | element;

element: function_definition
    | global_declaration
    ;

function_definition: '(' parameter_list ')' TK_OC_GE type'!' TK_IDENTIFICADOR command_block
    ;

parameter_list: type TK_IDENTIFICADOR
    | parameter_list ',' type TK_IDENTIFICADOR
    |
    ;

global_declaration: variable_declaration
    ;

local_declaration: variable_declaration
    ;

variable_declaration: type identifier_list ';'
    ;

type: TK_PR_INT
    | TK_PR_FLOAT
    | TK_PR_BOOL
    ;

identifier_list: TK_IDENTIFICADOR
    | identifier_list ',' TK_IDENTIFICADOR
    ;

command_block: '{' command_list '}'
    ;

command_list: simple_command command_list
    |
    ;

simple_command: local_declaration ';'
    | assignment ';'
    | function_call ';'
    | return_command ';'
    | conditional_if conditional_else ';'
    | iteration ';'
    | command_block ';'
    ;

assignment: TK_IDENTIFICADOR '=' expression
    ;

function_call: TK_IDENTIFICADOR '(' expression_list ')'
    | TK_IDENTIFICADOR '(' ')'
    ;

return_command: TK_PR_RETURN expression
    ;

conditional_if: TK_PR_IF '(' expression ')' command_block
    ;

conditional_else: TK_PR_ELSE expression
    |
    ;

iteration: TK_PR_WHILE '(' expression ')' command_block
    ;

%%

void yyerror (char const *mensagem)
{
    int line = get_line_number();
    fprintf(stderr, "Erro de sintaxe na linha %d. \n", line);
    exit(3);
}