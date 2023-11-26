%{
// Entrega 3
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include <stdlib.h>
#include "asd.h"

int yylex(void);
int yyerror (char const *mensagem);
extern void* arvore;
int get_line_number();
%}

%union
{
	lexical_value_t* lexical_value;
	asd_tree_t* nodo;
}

%token<lexical_value_t> TK_PR_INT
%token<lexical_value_t> TK_PR_FLOAT
%token<lexical_value_t> TK_PR_BOOL
%token<lexical_value_t> TK_PR_IF
%token<lexical_value_t> TK_PR_ELSE
%token<lexical_value_t> TK_PR_WHILE
%token<lexical_value_t> TK_PR_RETURN
%token<lexical_value_t> TK_OC_LE
%token<lexical_value_t> TK_OC_GE
%token<lexical_value_t> TK_OC_EQ
%token<lexical_value_t> TK_OC_NE
%token<lexical_value_t> TK_OC_AND
%token<lexical_value_t> TK_OC_OR
%token<lexical_value_t> TK_IDENTIFICADOR
%token<lexical_value_t> TK_LIT_INT
%token<lexical_value_t> TK_LIT_FLOAT
%token<lexical_value_t> TK_LIT_FALSE
%token<lexical_value_t> TK_LIT_TRUE
%token<lexical_value_t> TK_ERRO

%define parse.error verbose

%start program;

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

global_declaration: variable_declaration ';'
    ;

local_declaration: variable_declaration
    ;

variable_declaration: type identifier_list
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

conditional_else: TK_PR_ELSE command_block
    |
    ;

iteration: TK_PR_WHILE '(' expression ')' command_block
    ;

expression_list: expression
    | expression_list ',' expression
    ;

expression: precedence_6
    | expression TK_OC_OR precedence_6
    ;

precedence_6: precedence_5
    | precedence_6 TK_OC_AND precedence_5
    ;

precedence_5: precedence_4
    | precedence_5 TK_OC_EQ precedence_4
    | precedence_5 TK_OC_NE precedence_4
    ;

precedence_4: precedence_3
    | precedence_4 '<' precedence_3
    | precedence_4 '>' precedence_3
    | precedence_4 TK_OC_LE precedence_3
    | precedence_4 TK_OC_GE precedence_3
    ;

precedence_3: precedence_2
    | precedence_3 '+' precedence_2
    | precedence_3 '-' precedence_2
    ;

precedence_2: precedence_1
    | precedence_2 '*' precedence_1
    | precedence_2 '/' precedence_1
    | precedence_2 '%' precedence_1
    ;

precedence_1: '(' expression ')'
    | '!' precedence_1
    | '-' precedence_1
    | TK_IDENTIFICADOR
    | literal
    | function_call
    ;

literal: TK_LIT_INT
    | TK_LIT_FLOAT
    | TK_LIT_FALSE
    | TK_LIT_TRUE
    ;

%%

int yyerror (char const *mensagem)
{
    int line = get_line_number();
    fprintf(stderr, "Erro de sintaxe na linha %d. \n", line);
    return 1;
}