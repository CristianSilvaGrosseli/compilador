// Entrega 6
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "table.h"

int g_rbss_displacement = 0;
int g_rfp_displacement = 0;

void push_scope(TableList** list)
{
    TableList* new_table_node = (TableList*)malloc(sizeof(TableList));
    new_table_node->symbol_table = NULL;
    new_table_node->next = *list;
    *list = new_table_node;
}

void pop_scope(TableList** list)
{
    TableList* front_node = *list;
    TableList* new_front_node = front_node->next;
    *list = new_front_node;
    free(front_node);
}

TableList* get_global_scope(TableList** list)
{
    TableList* current_table = *list;
    while (current_table->next != NULL)
    {
        current_table = current_table->next;
    }
    return current_table;
}

int has_local_scope(TableList** list)
{
    TableList* current_table = *list;
    int scope_count = 0;
    while (current_table != NULL)
    {
        scope_count++;
        if (scope_count > 1)
        {
            return 1;
        }
        current_table = current_table->next;
    }
    return 0;
}

void insert_symbol_to_global_scope(TableList** list, lexical_value_t* lexical_value)
{
    if (lexical_value == NULL)
    {
        return;
    }
    if (*list == NULL)
    {
        push_scope(list); // adiciona o escopo global
    }
    check_err_declared(list, lexical_value);
    TableList* global_scope = get_global_scope(list);
    insert_entry_to_table(&(global_scope->symbol_table), lexical_value, GLOBAL_SYMBOL_SCOPE);
}

void insert_symbol_to_current_scope(TableList** list, lexical_value_t* lexical_value)
{
    if (lexical_value == NULL)
    {
        return;
    }
    if (*list == NULL)
    {
        push_scope(list); // adiciona o escopo global
    }
    if (!has_local_scope(list))
    {
        push_scope(list); // adiciona um escopo local
    }
    check_err_declared(list, lexical_value);
    insert_entry_to_table(&((*list)->symbol_table), lexical_value, LOCAL_SYMBOL_SCOPE);
}

void insert_entry_to_table(Table** table, lexical_value_t* lexical_value, int symbol_scope)
{
    Table* new_table = (Table*)malloc(sizeof(Table));
    if (new_table == NULL)
    {
        printf("Memory allocation failed");
        return;
    }

    new_table->info = (lexical_value_t*)malloc(sizeof(lexical_value_t));
    if (symbol_scope == GLOBAL_SYMBOL_SCOPE)
    {
        new_table->base = "rbss";
        new_table->displacement = g_rbss_displacement;
        g_rbss_displacement += 4;
    }
    else
    {
        new_table->base = "rfp";
        new_table->displacement = g_rfp_displacement;
        g_rfp_displacement += 4;
    }

    new_table->info->token_value = strdup(lexical_value->token_value);
    new_table->info->token_line = lexical_value->token_line;
    new_table->info->token_type = lexical_value->token_type;
    new_table->info->token_nature = lexical_value->token_nature;
    new_table->next = *table;

    *table = new_table;
}

void check_err_declared(TableList** list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_value_through_scopes(list, lexical_value);
    if (symbol_found != NULL)
    {
        printf("ERRO DE SEMANTICA - LINHA %d - REDECLARACAO DO IDENTIFICADOR '%s'\n", lexical_value->token_line, lexical_value->token_value);
        exit(ERR_DECLARED);
    }
}

void check_err_undeclared(TableList** list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_value_through_scopes(list, lexical_value);
    if (symbol_found == NULL)
    {
        printf("ERRO DE SEMANTICA - LINHA %d - IDENTIFICADOR '%s' NAO DECLARADO\n", lexical_value->token_line, lexical_value->token_value);
        exit(ERR_UNDECLARED);
    }
}

void check_err_variable(TableList** list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_value_through_scopes(list, lexical_value);
    if (symbol_found != NULL)
    {
        if (symbol_found->token_nature != TOKEN_NATURE_FUNCTION)
        {
            printf("ERRO DE SEMANTICA - LINHA %d - VARIAVEL '%s' SENDO INVOCADA COMO FUNCAO\n", lexical_value->token_line, lexical_value->token_value);
            exit(ERR_VARIABLE);
        }
    }
}

void check_err_function(TableList** list, lexical_value_t* lexical_value)
{
    lexical_value_t* symbol_found = find_value_through_scopes(list, lexical_value);
    if (symbol_found != NULL)
    {
        if (symbol_found->token_nature != TOKEN_NATURE_VARIABLE)
        {
            printf("ERRO DE SEMANTICA - LINHA %d - FUNCTION '%s' SENDO USADA COMO VARIAVEL\n", lexical_value->token_line, lexical_value->token_value);
            exit(ERR_FUNCTION);
        }
    }
}

lexical_value_t* find_value_through_scopes(TableList** list, lexical_value_t* lexical_value)
{
    Table* table = find_table_node_by_value(list, lexical_value->token_value);
    if (table != NULL && table->info != NULL)
    {
        return table->info;
    }
    return NULL;
}

Table* find_table_node_by_value(TableList** list, char* value)
{
    TableList* current_scope = *list;
    while (current_scope != NULL)
    {
        Table* current_table = current_scope->symbol_table;
        while (current_table != NULL)
        {
            if (strcmp(value, current_table->info->token_value) == 0)
            {
                return current_table;
            }
            current_table = current_table->next;
        }
        current_scope = current_scope->next;
    }
    return NULL;
}

int infer_type(node_ast_tree_t* root_node)
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
    TableList* current_list = *list;
    int list_number = 0;
    int table_number = 0;

    while(current_list != NULL)
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
