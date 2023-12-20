// Entrega 4
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#pragma once
#ifndef _TABELA_H_
#define _TABELA_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"

/* Estrutura representando cada entrada de uma tabela, implementada como uma lista simplesmente encadeada. */
typedef struct table
{
    lexical_value_t* info;
    struct table *next;

} Table;

/* Estrutura representando cada tabela de uma lista de tabelas de simbolos, implementada como uma lista duplamente encadeada. */
typedef struct table_list
{
    struct table_list* next;
    struct table_list* previous;
    Table* symbol_table;

} TableList;

void push_table(TableList** list, Table* new_table);
void pop_table(TableList** list);
void insert_entry_to_table(Table** table, lexical_value_t* lexical_value);

#endif //_TABELA_H_
