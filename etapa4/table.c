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
    lexical_value_t* symbol_found = find_table_symbol(list, lexical_value);
    if (symbol_found != NULL)
    {
        printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", lexical_value->token_line, lexical_value->token_value);
        exit(ERR_DECLARED);
    }
}

void check_err_undeclared(TableList* list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_table_symbol(list, lexical_value);
    if (symbol_found == NULL)
    {
        printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", lexical_value->token_line, lexical_value->token_value);
        exit(ERR_UNDECLARED);
    }
}

void check_err_variable(TableList* list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_table_symbol(list, lexical_value);
    if (symbol_found != NULL)
    {
        if (symbol_found->token_nature != TOKEN_NATURE_FUNCTION)
        {
            printf("ERRO DE SEMANTICA - LINHA %d - VARIAVEL '%s' SENDO INVOCADA COMO FUNCAO\n", lexical_value->token_line, lexical_value->token_value);
            exit(ERR_VARIABLE);
        }
    }
}

void check_err_function(TableList* list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_table_symbol(list, lexical_value);
    if (symbol_found != NULL)
    {
        if (symbol_found->token_nature != TOKEN_NATURE_VARIABLE)
        {
            printf("ERRO DE SEMANTICA - LINHA %d - FUNCTION '%s' SENDO USADA COMO VARIAVEL\n", lexical_value->token_line, lexical_value->token_value);
            exit(ERR_FUNCTION);
        }
    }
}

lexical_value_t* find_table_symbol(TableList* list, lexical_value_t* lexical_value)
{
 
    TableList* current_list = list;
  
    if(current_list == NULL) { return NULL; }
    
    while(current_list->next != NULL)
    {
        if(current_list->symbol_table == NULL){ current_list = current_list->next; continue; }

        Table* current_table = current_list->symbol_table;
        
        while (current_table->next != NULL)
        {
            if (strcmp(lexical_value->token_value, current_table->info->token_value) == 0)
            {
                return current_table->info;
            }
            current_table = current_table->next;
        }
        
        current_list = current_list->next;
    }

    return NULL;
}

int infer_type(asd_tree_t* root_node)
{
    int infered_type = -1;

    if (root_node != NULL)
    {
        infered_type = root_node->label->token_type;
        for (int i = 0; i < root_node->number_of_children; i++)
        {
            infered_type = aux_infer_type(infered_type, root_node->children[i]->label->token_type);
        }
    }

    return infered_type;
}

int aux_infer_type(int type1, int type2)
{
    if (type1 == TOKEN_TYPE_FLOAT || type2 == TOKEN_TYPE_FLOAT)
    {
        return TOKEN_TYPE_FLOAT;
    }
    if (type1 == TOKEN_TYPE_INT || type2 == TOKEN_TYPE_INT)
    {
        return TOKEN_TYPE_INT;
    }
    if (type1 == TOKEN_TYPE_BOOL || type2 == TOKEN_TYPE_BOOL)
    {
        return TOKEN_TYPE_BOOL;
    }
    return -1;
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
