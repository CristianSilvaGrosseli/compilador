// Entrega 7
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#pragma once
#ifndef _TABELA_H_
#define _TABELA_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tad_ast.h"

#define ERR_UNDECLARED 10
#define ERR_DECLARED 11
#define ERR_VARIABLE 20
#define ERR_FUNCTION 21

#define TOKEN_NATURE_LITERAL 1
#define TOKEN_NATURE_IDENTIFIER 2
#define TOKEN_NATURE_FUNCTION 3
#define TOKEN_NATURE_VARIABLE 4
#define TOKEN_NATURE_TYPE 5
#define TOKEN_NATURE_CONTROL 6
#define TOKEN_NATURE_OPERATOR 7
#define TOKEN_NATURE_EXPRESSION 8

#define GLOBAL_SYMBOL_SCOPE 1
#define LOCAL_SYMBOL_SCOPE 2

/* Estrutura representando cada entrada de uma tabela, implementada como uma lista simplesmente encadeada. */
typedef struct table
{
    lexical_value_t* info;
    char* base;
    int displacement;
    struct table *next;

} Table;

/* Estrutura representando cada tabela de uma lista de tabelas de simbolos, implementada como uma lista duplamente encadeada. */
typedef struct table_list
{
    struct table_list* next;
    Table* symbol_table;

} TableList;

void push_scope(TableList** list);
void pop_scope(TableList** list);
TableList* get_global_scope(TableList** list);
void insert_symbol_to_global_scope(TableList** list, lexical_value_t* lexical_value);
void insert_symbol_to_current_scope(TableList** list, lexical_value_t* lexical_value);
void insert_entry_to_table(Table** table, lexical_value_t* lexical_value, int symbol_scope);
void check_err_declared(TableList** list, lexical_value_t* lexical_value);
void check_err_undeclared(TableList** list, lexical_value_t* lexical_value);
void check_err_variable(TableList** list, lexical_value_t* lexical_value);
void check_err_function(TableList** list, lexical_value_t* lexical_value);
int infer_type(node_ast_tree_t* root_node);
int aux_infer_type(int type1, int type2);
lexical_value_t* find_value_through_scopes(TableList** list, lexical_value_t* lexical_value);
Table* find_table_node_by_value(TableList** list, char* value);
void print_table_list(TableList** list);


#endif //_TABELA_H_
