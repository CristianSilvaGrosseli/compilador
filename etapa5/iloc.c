// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "iloc.h"

IlocOperationList* g_IlocOperations;

// Função para criar uma nova operação ILOC
IlocOperation* newIlocOperation(char* operation, char* campo_1, char* campo_2, char* campo_3) {

    IlocOperation* op = (IlocOperation*)malloc(sizeof(IlocOperation));
    if (op == NULL)
    {
        fprintf(stderr, "Erro na funcao newIlocOperation() \n");
        fprintf(stderr, "Alocacao de memória falhou. \n");
        exit(EXIT_FAILURE);
    }

    op->mnemonico = strdup(operation);
    op->campo_1 = strdup(campo_1);
    if (campo_2 != NULL)
    {
        op->campo_2 = strdup(campo_2);
    }
    else
    {
        op->campo_2 = NULL;
    }

    if (campo_3 != NULL)
    {
        op->campo_3 = strdup(campo_3);
    }
    else
    {
        op->campo_3 = NULL;
    }

    return op;
}

IlocOperationList* addIlocOperation(IlocOperation* newIlocOperation)
{
    IlocOperationList *newIlocNode = (IlocOperationList *)malloc(sizeof(IlocOperationList));
    if (newIlocNode == NULL)
    {
        fprintf(stderr, "Erro na funcao addIlocOperation() \n");
        fprintf(stderr, "Alocacao de memória falhou. \n");
        exit(EXIT_FAILURE);
    }

    newIlocNode->operation = newIlocOperation;
    newIlocNode->next_operation = NULL;

    if (g_IlocOperations == NULL)
    {
        g_IlocOperations = newIlocNode;
    }
    else
    {
        IlocOperationList* current = g_IlocOperations;
        while (current->next_operation != NULL)
        {
            current = current->next_operation;
        }
        current->next_operation = newIlocNode;
    }

    return newIlocNode;
}

//store r1 => r2 // Memória(r2) = r1
IlocOperation* store_operation(char* parameter_1, int parameter_2)
{
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "r%d", parameter_2);
    return newIlocOperation("store", parameter_1, field_2, NULL);
}

IlocOperation* load_i_operation(char* parameter_1, int parameter_2)
{
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "r%d", parameter_2);
    return newIlocOperation("loadi", parameter_1, field_2, NULL);
}

void cbr_operation(int register_number, int label_1, int label_2)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "L%d", label_1);
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "L%d", label_2);
    char field_3[100];
    snprintf(field_3, sizeof(field_3), "r%d", register_number);
    addIlocOperation(newIlocOperation("cbr", field_1, field_2, field_3));
}

IlocOperation* custom_instruction_operation(char* instruction_name, int parameter_1, int parameter_2, int parameter_3)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "r%d", parameter_1);
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "r%d", parameter_2);
    char field_3[100];
    snprintf(field_3, sizeof(field_3), "r%d", parameter_3);
    return newIlocOperation(instruction_name, field_1, field_2, field_3);
}


// Função para imprimir uma operação ILOC
void printIlocOperation(IlocOperation* operation)
{
    char* mnemonico = operation->mnemonico;
    if (strcmp(mnemonico, "store"))
    {
        //store r1 => r2 // Memória(r2) = r1
        printf("%s %s => %s", operation->mnemonico, operation->campo_1, operation->campo_2);
    }
    else
    {
        printf("%s ", operation->mnemonico);
        printf("%s ", operation->campo_1);
        if(operation->campo_2 != NULL)
        {
            printf("%s ", operation->campo_2);
        }
        if (operation->campo_3 != NULL)
        {
            printf("%s ", operation->campo_3);
        }
    }
    printf("\n");
}

void printIlocOperations()
{
    IlocOperationList* current =  g_IlocOperations;
    while (current != NULL)
    {
        printIlocOperation(current->operation);
        current = current->next_operation;
    }
}