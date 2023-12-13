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
int counter=0;
%}

%union
{
	lexical_value_t* valor_lexico;
	asd_tree_t* nodo;
}

%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_BOOL
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_WHILE
%token<valor_lexico> TK_PR_RETURN
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT
%token<valor_lexico> TK_LIT_FALSE
%token<valor_lexico> TK_LIT_TRUE
%token<valor_lexico> TK_ERRO

%token<valor_lexico> '='
%token<valor_lexico> '<'
%token<valor_lexico> '>'
%token<valor_lexico> '+'
%token<valor_lexico> '*'
%token<valor_lexico> '/'
%token<valor_lexico> '%'
%token<valor_lexico> '!'
%token<valor_lexico> '-'

%type<valor_lexico> type

%type<nodo> program
%type<nodo> list
%type<nodo> element
%type<nodo> function_definition
%type<nodo> global_declaration
%type<nodo> parameter_list
%type<nodo> variable_declaration
%type<nodo> local_declaration
%type<nodo> identifier_list
%type<nodo> command_list
%type<nodo> simple_command
%type<nodo> command_block
%type<nodo> assignment
%type<nodo> function_call
%type<nodo> return_command
%type<nodo> conditional_if
%type<nodo> conditional_else
%type<nodo> iteration
%type<nodo> expression_list
%type<nodo> expression
%type<nodo> precedence_6
%type<nodo> precedence_5
%type<nodo> precedence_4
%type<nodo> precedence_3
%type<nodo> precedence_2
%type<nodo> precedence_1
%type<nodo> literal 

%define parse.error verbose

%start program;

%%

program: %empty { $$ = NULL; arvore = NULL;} 
    | list  {  $$ = $1; arvore = $$; }
    ;


list: element list  { if ($1 != NULL  && $2 !=NULL){
                         $$ = $1; 
                         asd_add_child($$, $2);
                         } 
                        else if ($1 != NULL ) 
                            { $$ = $1; } 
                          else if ($2 != NULL)
                            { $$ = $2; }
                            else{ $$ = NULL;} 
                        }
    | element { $$ = $1; }
    ;

element: function_definition { $$ = $1; }
    | global_declaration { $$ = NULL; }
    ;

function_definition: '(' parameter_list ')' TK_OC_GE type '!' TK_IDENTIFICADOR command_block { $$ = asd_new($7, 0); asd_add_child($$,$8); }
    ;

parameter_list: type TK_IDENTIFICADOR { $$ = asd_new($2, 0); }
    | parameter_list ',' type TK_IDENTIFICADOR { if ($1 != NULL) { $$ = $1; asd_add_child($$, asd_new($4,0)); } else { $$ = asd_new($4,0); } }
    | %empty {$$ = NULL; }
    ;

global_declaration: variable_declaration ';' { $$ = $1; }
    ;

local_declaration: variable_declaration { $$ = $1; }
    ;

variable_declaration: type identifier_list { $$ = $2; }
    ;

type: TK_PR_INT { $$ = $1; }
    | TK_PR_FLOAT { $$ = $1; }
    | TK_PR_BOOL { $$ = $1; }
    ;

identifier_list: TK_IDENTIFICADOR  { $$ = asd_new($1, 0); }
    | identifier_list ',' TK_IDENTIFICADOR { if ($1 != NULL) { $$ = $1; asd_add_child($$, asd_new($3, 0)); } else { $$ = asd_new($3, 0); } }
    ;

command_block: '{' command_list '}' { $$ = $2; }
    ;

command_list: simple_command command_list { if ($1 != NULL) { $$ = $1; asd_add_child($$, $2); } else { $$ = $2; } }
    | %empty { $$ = NULL; }
    ;

simple_command: local_declaration ';' { $$ = $1; }
    | assignment ';' { $$ = $1; }
    | function_call ';' { $$ = $1; }
    | return_command ';' { $$ = $1; }
    | conditional_if conditional_else ';' { $$ = $1; asd_add_child($$, $2); }
    | iteration ';' { $$ = $1; }
    | command_block ';' { $$ = $1; }
    ;

assignment: TK_IDENTIFICADOR '=' expression { $$ = asd_new($2, 0); asd_add_child($$, asd_new($1, 0)); asd_add_child($$, $3); }
    ;

function_call: TK_IDENTIFICADOR '(' expression_list ')' { $$ = asd_new($1, ARVORE_CALL); asd_add_child($$, $3); }
    | TK_IDENTIFICADOR '(' ')'  { $$ = asd_new($1, ARVORE_CALL); }
    ;

return_command: TK_PR_RETURN expression { $$ = asd_new($1, 0); asd_add_child($$, $2); }
    ;

conditional_if: TK_PR_IF '(' expression ')' command_block { $$ = asd_new($1, 0); asd_add_child($$,$3); asd_add_child($$,$5); }
    ;

conditional_else: TK_PR_ELSE command_block { $$ = asd_new($1, 0); asd_add_child($$,$2); }
    | %empty { $$ = NULL; }
    ;

iteration: TK_PR_WHILE '(' expression ')' command_block { $$ = asd_new($1, 0); asd_add_child($$,$3); asd_add_child($$, $5); }
    ;

expression_list: expression { $$ = $1 ; }
    | expression_list ',' expression {$$ = $1; asd_add_child($$, $3);}
    ;

expression: precedence_6 {$$=$1;}
    | expression TK_OC_OR precedence_6 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_6: precedence_5                {$$=$1;}
    | precedence_6 TK_OC_AND precedence_5 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_5: precedence_4               {$$=$1;}
    | precedence_5 TK_OC_EQ precedence_4 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_5 TK_OC_NE precedence_4 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_4: precedence_3          {$$=$1;}
    | precedence_4 '<' precedence_3 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_4 '>' precedence_3 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_4 TK_OC_LE precedence_3 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_4 TK_OC_GE precedence_3 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_3: precedence_2          {$$=$1;}
    | precedence_3 '+' precedence_2 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_3 '-' precedence_2 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_2: precedence_1 {$$=$1;}
    | precedence_2 '*' precedence_1 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_2 '/' precedence_1 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    | precedence_2 '%' precedence_1 { $$ = asd_new($2, 0); asd_add_child($$, $1); asd_add_child($$, $3); };
    ;

precedence_1: '(' expression ')' {$$=$2;}
    | '!' precedence_1  { $$ = asd_new($1, 0); asd_add_child($$, $2); };
    | '-' precedence_1  { $$ = asd_new($1, 0); asd_add_child($$, $2); };
    | TK_IDENTIFICADOR  { $$ = asd_new($1, 0); };
    | literal           { $$ = $1; };
    | function_call     { $$ = $1; };
    ;

literal: TK_LIT_INT {$$ = asd_new($1, 0);}
    | TK_LIT_FLOAT {$$ = asd_new($1, 0);}
    | TK_LIT_FALSE {$$ = asd_new($1, 0);}
    | TK_LIT_TRUE {$$ = asd_new($1, 0);}
    ;

%%

int yyerror (char const *mensagem)
{
    int line = get_line_number();
    fprintf(stderr, "Erro de sintaxe na linha %d. \n", line);
    return 1;
}
