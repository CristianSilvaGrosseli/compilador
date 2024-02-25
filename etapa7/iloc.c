// Entrega 7
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
    if (campo_1 != NULL)
    {
        op->campo_1 = strdup(campo_1);
    }
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
IlocOperationList* store_AI_operation(int parameter_1, char* parameter_2, int parameter_3)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "r%d", parameter_1);
    char field_3[100];
    snprintf(field_3, sizeof(field_3), "%d", parameter_3);
    return addIlocOperation(newIlocOperation("storeAI", field_1, parameter_2, field_3));
}

IlocOperationList* load_i_operation(char* parameter_1, int parameter_2)
{
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "r%d", parameter_2);
    return addIlocOperation(newIlocOperation("loadI", parameter_1, field_2, NULL));
}

IlocOperationList* load_AI_operation(int parameter_1, char* parameter_2, int parameter_3)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "r%d", parameter_1);
    char field_3[100];
    snprintf(field_3, sizeof(field_3), "%d", parameter_3);
    return addIlocOperation(newIlocOperation("loadAI", field_1, parameter_2, field_3));
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

void add_label_operation(int label)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "L%d", label);
    addIlocOperation(newIlocOperation("label", NULL, NULL, field_1));
}

void add_jump_i_operation(int label)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "L%d", label);
    addIlocOperation(newIlocOperation("jumpI", NULL, NULL, field_1));
}

IlocOperationList* custom_instruction_operation(char* instruction_name, int parameter_1, int parameter_2, int parameter_3)
{
    char field_1[100];
    snprintf(field_1, sizeof(field_1), "r%d", parameter_1);
    char field_2[100];
    snprintf(field_2, sizeof(field_2), "r%d", parameter_2);
    char field_3[100];
    snprintf(field_3, sizeof(field_3), "r%d", parameter_3);
    return addIlocOperation(newIlocOperation(instruction_name, field_1, field_2, field_3));
}


// Função para imprimir uma operação ILOC
void printIlocOperation(IlocOperation* operation)
{
    char* mnemonico = operation->mnemonico;
    if (strcmp(mnemonico, "storeAI") == 0)
    {
        // storeAI r1 => r2, c3 // Memoria(r2 + c3) = r1
        printf("%s %s => %s, %s", operation->mnemonico, operation->campo_1, operation->campo_2, operation->campo_3);
    }
    else if (strcmp(mnemonico, "loadAI") == 0)
    {
        // loadAI r1, c2 => r3 // r3 = Memoria(r1 + c2)
        printf("%s %s, %s => %s", operation->mnemonico, operation->campo_2, operation->campo_3, operation->campo_1);
    }
    else if (strcmp(mnemonico, "loadI") == 0)
    {
        // loadI c1 => r2 // r2 = c1
        printf("%s %s => %s", operation->mnemonico, operation->campo_1, operation->campo_2);
    }
    else if (strcmp(mnemonico, "label") == 0)
    {
        // label
        printf("%s:", operation->campo_3);
    }
    else if (strcmp(mnemonico, "cbr") == 0)
    {
        // cbr r1 -> l2, l3 // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
        printf("%s %s => %s, %s", operation->mnemonico, operation->campo_3, operation->campo_1, operation->campo_2);
    }
    else if (strcmp(mnemonico, "jumpI") == 0)
    {
        // jumpI -> l1 // PC = endereço(l1)
        printf("%s => %s", operation->mnemonico, operation->campo_3);
    }
    else if (
        strcmp(mnemonico, "add") == 0 ||
        strcmp(mnemonico, "sub") == 0 ||
        strcmp(mnemonico, "mult") == 0 ||
        strcmp(mnemonico, "div") == 0 ||
        strcmp(mnemonico, "cmp_EQ") == 0 ||
        strcmp(mnemonico, "cmp_NE") == 0 ||
        strcmp(mnemonico, "cmp_GE") == 0 ||
        strcmp(mnemonico, "cmp_LE") == 0 ||
        strcmp(mnemonico, "cmp_GT") == 0 ||
        strcmp(mnemonico, "cmp_LT") == 0 ||
        strcmp(mnemonico, "and") == 0 ||
        strcmp(mnemonico, "or") == 0
    )
    {
        printf("%s %s, %s => %s", operation->mnemonico, operation->campo_1, operation->campo_2, operation->campo_3);
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

void writeIlocOperation()
{
    FILE *foutput = fopen("input.txt", "w+");

    IlocOperationList* current =  g_IlocOperations;
    while (current != NULL)
    {
        IlocOperation* operation = current->operation;
        char* mnemonico = operation->mnemonico;
        if (strcmp(mnemonico, "storeAI") == 0)
        {
            // storeAI r1 => r2, c3 // Memoria(r2 + c3) = r1
            fprintf(foutput, "%s %s => %s, %s\n", operation->mnemonico, operation->campo_1, operation->campo_2, operation->campo_3);
        }
        else if (strcmp(mnemonico, "loadAI") == 0)
        {
            // loadAI r1, c2 => r3 // r3 = Memoria(r1 + c2)
            fprintf(foutput, "%s %s, %s => %s\n", operation->mnemonico, operation->campo_2, operation->campo_3, operation->campo_1);
        }
        else if (strcmp(mnemonico, "loadI") == 0)
        {
            // loadI c1 => r2 // r2 = c1
            fprintf(foutput, "%s %s => %s\n", operation->mnemonico, operation->campo_1, operation->campo_2);
        }
        else if (strcmp(mnemonico, "label") == 0)
        {
            // label
            fprintf(foutput, "%s:\n", operation->campo_3);
        }
        else if (strcmp(mnemonico, "cbr") == 0)
        {
            // cbr r1 -> l2, l3 // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
            fprintf(foutput, "%s %s => %s, %s\n", operation->mnemonico, operation->campo_3, operation->campo_1, operation->campo_2);
        }
        else if (strcmp(mnemonico, "jumpI") == 0)
        {
            // jumpI -> l1 // PC = endereço(l1)
            fprintf(foutput, "%s => %s\n", operation->mnemonico, operation->campo_3);
        }
        else if (
            strcmp(mnemonico, "add") == 0 ||
            strcmp(mnemonico, "sub") == 0 ||
            strcmp(mnemonico, "mult") == 0 ||
            strcmp(mnemonico, "div") == 0 ||
            strcmp(mnemonico, "cmp_EQ") == 0 ||
            strcmp(mnemonico, "cmp_NE") == 0 ||
            strcmp(mnemonico, "cmp_GE") == 0 ||
            strcmp(mnemonico, "cmp_LE") == 0 ||
            strcmp(mnemonico, "cmp_GT") == 0 ||
            strcmp(mnemonico, "cmp_LT") == 0 ||
            strcmp(mnemonico, "and") == 0 ||
            strcmp(mnemonico, "or") == 0
        )
        {
            fprintf(foutput, "%s %s, %s => %s\n", operation->mnemonico, operation->campo_1, operation->campo_2, operation->campo_3);
        }

        current = current->next_operation;
    }

    fclose(foutput);
}