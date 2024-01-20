// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "iloc.h"

// Função para criar uma nova operação ILOC
ILOCOperation* newILOCOperation(char* operation, char** arguments, int num_arguments) {
    ILOCOperation* op = (ILOCOperation*)malloc(sizeof(ILOCOperation));
    op->operation = strdup(operation);
    op->arguments = (char**)malloc(num_arguments * sizeof(char*));
    for (int i = 0; i < num_arguments; i++) {
        op->arguments[i] = strdup(arguments[i]);
    }
    op->num_arguments = num_arguments;
    return op;
}

// Função para adicionar uma operação à lista de operações ILOC
void addILOCOperation(ILOCOperationList* list, ILOCOperation* operation) {
    list->operations = (ILOCOperation*)realloc(list->operations, (list->num_operations + 1) * sizeof(ILOCOperation));
    list->operations[list->num_operations] = *operation;
    list->num_operations++;
}

// Função para imprimir uma operação ILOC
void printILOCOperation(ILOCOperation* operation) {
    printf("%s ", operation->operation);
    for (int i = 0; i < operation->num_arguments; i++) {
        printf("%s ", operation->arguments[i]);
    }
    printf("\n");
}

// Função para imprimir a lista de operações ILOC
void printILOCOperationList(ILOCOperationList* list) {
    for (int i = 0; i < list->num_operations; i++) {
        printILOCOperation(&list->operations[i]);
    }
}

