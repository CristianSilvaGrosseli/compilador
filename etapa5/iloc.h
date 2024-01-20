// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Leve em conta que os argumentos das ope-
ações podem ser nomes de temporários, valores cons-
tantes, ou nomes de rótulos */ //---------->>>> FAZER O RELEASE QND LIBERAR AS STRUCTS


// Estrutura para representar uma operação ILOC
typedef struct {
    char* operation;
    char** arguments;
    int num_arguments;
} ILOCOperation;

// Estrutura para manter uma lista de operações ILOC
typedef struct {
    ILOCOperation* operations;
    int num_operations;
} ILOCOperationList;


// Função para criar uma nova operação ILOC
ILOCOperation* newILOCOperation(char* operation, char** arguments, int num_arguments);

// Função para adicionar uma operação à lista de operações ILOC
void addILOCOperation(ILOCOperationList* list, ILOCOperation* operation);

// Função para imprimir uma operação ILOC
void printILOCOperation(ILOCOperation* operation);

// Função para imprimir a lista de operações ILOC
void printILOCOperationList(ILOCOperationList* list);

