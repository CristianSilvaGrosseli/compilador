// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "iloc.h"

// Função para criar uma nova operação ILOC
ILOCOperation* newILOCOperation(char* operation, char* campo_1, char* campo_2, char* campo_3) {

    ILOCOperation* op = (ILOCOperation*)malloc(sizeof(ILOCOperation));

    op->mnemonico = strdup(operation);
    
    op->campo_1 = strdup(campo_1);

    if(campo_2 != NULL){ op->campo_2 = strdup(campo_2);} else { op->campo_2 = NULL; }

    if(campo_3 != NULL){ op->campo_3 = strdup(campo_3);} else { op->campo_3 = NULL; }
    
    return op;
}


// Função para imprimir uma operação ILOC
void printILOCOperation(ILOCOperation* operation) {
    
    printf("%s ", operation->mnemonico);

    printf("%s ", operation->campo_1);

    if(operation->campo_2 != NULL) {printf("%s ", operation->campo_2);}

    if(operation->campo_3 != NULL) {printf("%s ", operation->campo_3);}

    printf("\n");
}


// Função para adicionar uma operação à lista de operações ILOC
void addILOCOperation(ILOCOperationList* list, ILOCOperation* operation) {
    list->operations = (ILOCOperation*)realloc(list->operations, (list->num_operations + 1) * sizeof(ILOCOperation));
    list->operations[list->num_operations] = *operation;
    list->num_operations++;
}


// Função para imprimir a lista de operações ILOC
void printILOCOperationList(ILOCOperationList* list) {
    for (int i = 0; i < list->num_operations; i++) {
        printILOCOperation(&list->operations[i]);
    }
}

