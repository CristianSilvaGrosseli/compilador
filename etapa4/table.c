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

    if( new_front_node != NULL)
        {new_front_node->previous = NULL;}

    (*list) = new_front_node;

    free(front_node);

}

void insert_entry_to_table(TableList* list, lexical_value_t* lexical_value)
{
    check_err_declared(list, lexical_value);

    Table* new_table = (Table*)malloc(sizeof(Table));
    new_table->info = (lexical_value_t*)malloc(sizeof(lexical_value_t));

    new_table->info->token_value = strdup(lexical_value->token_value);
    new_table->info->token_line = lexical_value->token_line;
    new_table->info->token_type = lexical_value->token_type;
    new_table->info->token_nature = lexical_value->token_nature;
    new_table->next = NULL;

    Table* table = list->symbol_table;

    if (table == NULL)
    {
        table = new_table;
    }
    else
    {
        Table* current_table = table;
        while (current_table->next != NULL)
        {
            current_table = current_table->next;
        }
        current_table->next = new_table;
    }
}

void check_err_declared(TableList* list, lexical_value_t* lexical_value)
{
    TableList* current_list = list;

    while(current_list->next != NULL)
    {
        Table* current_table = current_list->symbol_table;
        while (current_table->next != NULL)
        {
            if (strcmp(lexical_value->token_value, current_table->info->token_value) == 0)
            {
                printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", lexical_value->token_line, lexical_value->token_value);
                exit(ERR_DECLARED);
            }
            current_table = current_table->next;
        }
        current_list = current_list->next;
    }
}

void print_table_list(TableList** list)
{
    printf("print_table()\n");
    TableList* current_list = *list;
    int list_number = 0;
    int table_number = 0;

    while(current_list->next != NULL)
    {
        printf("list: %d\n", ++list_number);
        Table* current_table = current_list->symbol_table;
        while (current_table->next != NULL)
        {
            printf("table: %d\n", ++table_number);
            printf("token_value: %s\n", current_table->info->token_value);
            printf("token_nature: %d\n", current_table->info->token_nature);
            printf("token_type: %d\n" , current_table->info->token_type);
            printf("token_line: %d\n\n", current_table->info->token_line);

            current_table = current_table->next;
        }
        current_list = current_list->next;
    }
}
