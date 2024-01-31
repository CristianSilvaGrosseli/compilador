// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "iloc.h"

extern IlocOperationList* IlocOperations;

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

    if (IlocOperations == NULL)
    {
        IlocOperations = newIlocNode;
    }
    else
    {
        IlocOperationList* current = IlocOperations;
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
    char* field_2;
    snprintf(field_2, 100, "r%d", parameter_2);
    newIlocOperation("store", parameter_1, field_2, NULL);
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
    IlocOperationList* current =  IlocOperations;
    while (current != NULL)
    {
        printIlocOperation(current->operation);
        current = current->next_operation;
    }
}