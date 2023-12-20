%{
// Entrega 3
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include <stdlib.h>
#include "asd.h"
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
%type<nodo> parameter_tuple
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

%start program_begin;

%%

push_table_scope: %empty { push_table(&global_table_list, global_table); 
printf("push_table_scope. counter = %d\n", ++counter);
print_table_list(&global_table_list);
}


program_begin: push_table_scope program

program: %empty { $$ = NULL; arvore = NULL; }
    | list  {  $$ = $1; arvore = $$; pop_table(&global_table_list); }
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
    | global_declaration { $$ = $1; }
    ;

function_definition: '(' push_table_scope parameter_list ')' TK_OC_GE type '!' TK_IDENTIFICADOR command_block
{
    $8->token_nature = TOKEN_NATURE_FUNCTION;
    insert_entry_to_table(&global_table, $8);
    $$ = asd_new($8, 0); asd_add_child($$,$9);
}
    ;

function_definition: '(' push_table_scope ')' TK_OC_GE type '!' TK_IDENTIFICADOR command_block
{
    $7->token_type = current_type;
    $7->token_nature = TOKEN_NATURE_FUNCTION;
    insert_entry_to_table(&global_table, $7);
    $$ = asd_new($7, 0); asd_add_child($$,$8);
}
    ;

parameter_tuple: type TK_IDENTIFICADOR { $$ = asd_new($2, 0); };

parameter_list: parameter_tuple { $$=$1; }
    | parameter_list ',' parameter_tuple { $$ = $1; asd_add_child($$, $3); };


global_declaration: variable_declaration ';' { $$ = NULL; }
    ;

local_declaration: variable_declaration { $$ = NULL; }
    ;

variable_declaration: type identifier_list
{
    $$ = NULL;
}
    ;

type: TK_PR_INT { current_type = TK_PR_INT; $$ = $1; }
    | TK_PR_FLOAT { current_type = TK_PR_FLOAT; $$ = $1; }
    | TK_PR_BOOL { current_type = TK_PR_BOOL;  $$ = $1; }
    ;

identifier_list: TK_IDENTIFICADOR
{
    $1->token_type = current_type;
    $1->token_nature = TOKEN_NATURE_IDENTIFIER;
    insert_entry_to_table(&global_table, $1);
    $$ = NULL;
}
    | identifier_list ',' TK_IDENTIFICADOR
{
    $3->token_type = current_type;
    $3->token_nature = TOKEN_NATURE_IDENTIFIER;
    insert_entry_to_table(&global_table, $3);
    $$ = NULL;
}
    ;

command_block: '{' '}' { pop_table(&global_table_list); $$ = NULL; }
    | '{' command_list '}' { pop_table(&global_table_list); $$ = $2; }
    ;

command_list: push_table_scope simple_command ';' command_list {
        if($2 == NULL) {
            $$ = $4;
        }
        else
        {
            $$ = $2;
            asd_add_child($$, $4);
        }
    }
    | push_table_scope simple_command ';' {$$ = $2;}
    ;

simple_command: local_declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | function_call { $$ = $1; }
    | return_command { $$ = $1; }
    | conditional_if conditional_else { $$ = $1; asd_add_child($$, $2); }
    | iteration { $$ = $1; }
    | command_block { $$ = $1; }
    ;

assignment: TK_IDENTIFICADOR '=' expression
{
     $$ = asd_new($2, 0);
     asd_add_child($$, asd_new($1, 0));
     asd_add_child($$, $3);
}
    ;

function_call: TK_IDENTIFICADOR '(' expression_list ')' { $$ = asd_new($1, ARVORE_CALL); asd_add_child($$, $3); }
    | TK_IDENTIFICADOR '(' ')'  { $$ = asd_new($1, ARVORE_CALL); }
    ;

return_command: TK_PR_RETURN expression { $$ = asd_new($1, 0); asd_add_child($$, $2); }
    ;

conditional_if: TK_PR_IF '(' expression ')' push_table_scope command_block { $$ = asd_new($1, 0); asd_add_child($$,$3); asd_add_child($$,$6); }
    ;

conditional_else: TK_PR_ELSE push_table_scope command_block { $$ =  $3; }
    | %empty { $$ = NULL; }
    ;

iteration: TK_PR_WHILE '(' expression ')' push_table_scope command_block { $$ = asd_new($1, 0); asd_add_child($$,$3); asd_add_child($$, $6); }
    ;

expression_list: expression  ',' expression_list { if ($1 != NULL) { $$ = $1; asd_add_child($$, $3); } else { $$ = $3; } }
    | expression { $$ = $1 ; }
    | %empty { $$ = NULL; /* causando um shift reduce. Remover, mas refazer os testes */ }
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
