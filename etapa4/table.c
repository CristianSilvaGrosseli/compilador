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

void print_table_list(TableList** list){

    printf("entrou na print table\n");

    TableList* atual =  *list;

    while(atual != NULL){

        printf("entrou no while \n");

        //ESTA DANDO SEGMENTATION FAULT A PARTIR DESTE PONTO:
        printf("token_value: %s\n", atual->symbol_table->info->token_value);
        printf("token_type: %d\n" , atual->symbol_table->info->token_type);
        printf("token_line: %d\n", atual->symbol_table->info->token_line);

        atual = atual->next;
    }

}
