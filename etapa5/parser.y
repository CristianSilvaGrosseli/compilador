%{
// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include <stdlib.h>
#include "tad_ast.h"
#include "table.h"

int yylex(void);
int yyerror (char const *mensagem);
extern void* arvore;
TableList* global_table_list = NULL;
Table* global_table = NULL;
int get_line_number();
int counter=0;
int current_type = -1;
%}

%union
{
    lexical_value_t* valor_lexico;
	node_ast_tree_t* nodo;
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
%type<nodo> parameter_tuple
%type<nodo> local_declaration
%type<nodo> local_identifier_list
%type<nodo> global_identifier_list
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

%start program_begin;

%%

program_begin: program

program: %empty { $$ = NULL; arvore = NULL; }
    | list  { $$ = $1; arvore = $$; }
    ;


list: element list  { if ($1 != NULL  && $2 !=NULL){
                         $$ = $1;
                         ast_add_child($$, $2);
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
    | global_declaration { $$ = $1; }
    ;

function_definition: '(' parameter_list ')' TK_OC_GE type '!' TK_IDENTIFICADOR command_block
{
    $7->token_nature = TOKEN_NATURE_FUNCTION;
    insert_symbol_to_global_scope(&global_table_list, $7);
    $$ = ast_new($7, 0); ast_add_child($$,$8);
}
    ;

function_definition: '(' ')' TK_OC_GE type '!' TK_IDENTIFICADOR command_block
{
    $6->token_type = current_type;
    $6->token_nature = TOKEN_NATURE_FUNCTION;
    insert_symbol_to_global_scope(&global_table_list, $6);
    $$ = ast_new($6, 0); ast_add_child($$,$7);
}
    ;

parameter_tuple: type TK_IDENTIFICADOR { $$ = ast_new($2, 0); insert_symbol_to_current_scope(&global_table_list, $2); };

parameter_list: parameter_tuple { $$=$1; }
    | parameter_list ',' parameter_tuple { $$ = $1; ast_add_child($$, $3); };


global_declaration: type global_identifier_list ';' { printf("global declaration"); $$ = NULL; }
    ;

local_declaration: type local_identifier_list { printf("local declaration"); $$ = NULL; }
    ;

type: TK_PR_INT { current_type = TOKEN_TYPE_INT; $$ = $1; }
    | TK_PR_FLOAT { current_type = TOKEN_TYPE_FLOAT; $$ = $1; }
    | TK_PR_BOOL { current_type = TOKEN_TYPE_BOOL;  $$ = $1; }
    ;

global_identifier_list: TK_IDENTIFICADOR
{
    printf("identifier list 1");
    if ($1 != NULL)
    {
        $1->token_type = current_type;
        $1->token_nature = TOKEN_NATURE_VARIABLE;
        insert_symbol_to_global_scope(&global_table_list, $1);
    }
    $$ = NULL;
}
    | global_identifier_list ',' TK_IDENTIFICADOR
{
    if ($3 != NULL)
    {
        printf("identifier list 2");
        $3->token_type = current_type;
        $3->token_nature = TOKEN_NATURE_VARIABLE;
        insert_symbol_to_global_scope(&global_table_list, $3);
    }
    $$ = NULL;
}
    ;

local_identifier_list: TK_IDENTIFICADOR
{
    printf("identifier list 1");
    if ($1 != NULL)
    {
        $1->token_type = current_type;
        $1->token_nature = TOKEN_NATURE_VARIABLE;
        insert_symbol_to_current_scope(&global_table_list, $1);
    }
    $$ = NULL;
}
    | local_identifier_list ',' TK_IDENTIFICADOR
{
    if ($3 != NULL)
    {
        printf("identifier list 2");
        $3->token_type = current_type;
        $3->token_nature = TOKEN_NATURE_VARIABLE;
        insert_symbol_to_current_scope(&global_table_list, $3);
    }
    $$ = NULL;
}
    ;

command_block: '{' '}' { /*pop_scope(&global_table_list);*/ $$ = NULL; }
    | '{' command_list '}' { /*pop_scope(&global_table_list);*/ $$ = $2; }
    ;

command_list: simple_command ';' command_list {
        if($1 == NULL) {
            $$ = $3;
        }
        else
        {
            $$ = $1;
            ast_add_child($$, $3);
        }
    }
    | simple_command ';' {$$ = $1;}
    ;

simple_command: local_declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | function_call { $$ = $1; }
    | return_command { $$ = $1; }
    | conditional_if conditional_else { $$ = $1; ast_add_child($$, $2); }
    | iteration { $$ = $1; }
    | command_block { $$ = $1; }
    ;

assignment: TK_IDENTIFICADOR '=' expression
{
    $$ = ast_new($2, 0);
    ast_add_child($$, ast_new($1, 0));
    ast_add_child($$, $3);
    $1->token_type = infer_type($$);
    check_err_undeclared(&global_table_list, $1);
    check_err_function(&global_table_list, $1);
}
    ;

function_call: TK_IDENTIFICADOR '(' expression_list ')'
{
    check_err_undeclared(&global_table_list, $1);
    check_err_variable(&global_table_list, $1);
    $$ = ast_new($1, ARVORE_CALL); ast_add_child($$, $3);
}
    | TK_IDENTIFICADOR '(' ')'
{
    check_err_undeclared(&global_table_list, $1);
    check_err_variable(&global_table_list, $1);
    $$ = ast_new($1, ARVORE_CALL);
}
    ;

return_command: TK_PR_RETURN expression
{
    $1->token_type = infer_type($2);
    $$ = ast_new($1, 0); ast_add_child($$, $2);
}
    ;

conditional_if: TK_PR_IF '(' expression ')' command_block
{
    $1->token_type = infer_type($3);
    $$ = ast_new($1, 0); ast_add_child($$,$3); ast_add_child($$,$5);
}
    ;

conditional_else: TK_PR_ELSE command_block
{
    $1->token_type = infer_type($2);
    $$ = $2;
}
    | %empty { $$ = NULL; }
    ;

iteration: TK_PR_WHILE '(' expression ')' command_block
{
    $1->token_type = infer_type($3);
    $$ = ast_new($1, 0); ast_add_child($$,$3); ast_add_child($$, $5);
}
    ;

expression_list: expression  ',' expression_list { if ($1 != NULL) { $$ = $1; ast_add_child($$, $3); } else { $$ = $3; } }
    | expression { $$ = $1 ; }
    ;

expression: precedence_6 {$$=$1;}
    | expression TK_OC_OR precedence_6 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_6: precedence_5                {$$=$1;}
    | precedence_6 TK_OC_AND precedence_5 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_5: precedence_4               {$$=$1;}
    | precedence_5 TK_OC_EQ precedence_4 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_5 TK_OC_NE precedence_4 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_4: precedence_3          {$$=$1;}
    | precedence_4 '<' precedence_3 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_4 '>' precedence_3 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_4 TK_OC_LE precedence_3 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_4 TK_OC_GE precedence_3 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_3: precedence_2          {$$=$1;}
    | precedence_3 '+' precedence_2 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_3 '-' precedence_2 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_2: precedence_1 {$$=$1;}
    | precedence_2 '*' precedence_1 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_2 '/' precedence_1 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    | precedence_2 '%' precedence_1 { $$ = ast_new($2, 0); ast_add_child($$, $1); ast_add_child($$, $3); };
    ;

precedence_1: '(' expression ')' {$$=$2;}
    | '!' precedence_1  { $$ = ast_new($1, 0); ast_add_child($$, $2); };
    | '-' precedence_1  { $$ = ast_new($1, 0); ast_add_child($$, $2); };
    | TK_IDENTIFICADOR  { check_err_undeclared(&global_table_list, $1); check_err_function(&global_table_list, $1); $$ = ast_new($1, 0); };
    | literal           { $$ = $1; };
    | function_call     { $$ = $1; };
    ;

literal: TK_LIT_INT {$$ = ast_new($1, 0);}
    | TK_LIT_FLOAT {$$ = ast_new($1, 0);}
    | TK_LIT_FALSE {$$ = ast_new($1, 0);}
    | TK_LIT_TRUE {$$ = ast_new($1, 0);}
    ;

%%

int yyerror (char const *mensagem)
{
    int line = get_line_number();
    fprintf(stderr, "Erro de sintaxe na linha %d. \n", line);
    return 1;
}
