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