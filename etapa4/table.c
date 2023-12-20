// Entrega 4
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "table.h"

void push_table(TableList** list, Table* new_table)
{
    TableList* new_table_node = (TableList*)malloc(sizeof(TableList));
    new_table_node->symbol_table = new_table;

    new_table_node->next = (*list);
    new_table_node->previous = NULL;

    (*list) = new_table_node;
}

void pop_table(TableList** list)
{
    TableList* front_node = (*list);

    TableList* new_front_node = front_node->next;
    new_front_node->previous = NULL;
    (*list) = new_front_node;

    free(front_node);
}

void insert_entry_to_table(Table** table, lexical_value_t* lexical_value)
{
    Table* new_table = (Table*)malloc(sizeof(Table));
    new_table->info = (lexical_value_t*)malloc(sizeof(lexical_value_t));

    new_table->info->token_value = strdup(lexical_value->token_value);
    new_table->info->token_line = lexical_value->token_line;
    new_table->info->token_type = lexical_value->token_type;
    new_table->info->token_nature = lexical_value->token_nature;
    new_table->next = NULL;

    if (*table == NULL)
    {
        *table = new_table;
    }
    else
    {
        Table* current_table = *table;
        while (current_table->next != NULL)
        {
            current_table = current_table->next;
        }
        current_table->next = new_table;
    }
}