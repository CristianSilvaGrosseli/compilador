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
// Uma instrução ILOC possui um mnemônico e até 3 argumentos
// mnemonico e campo_1 é obrigatório
// os campos(2,3) são opcionais
typedef struct {
    
    char* mnemonico;
    char* campo_1;
    char* campo_2;
    char* campo_3;

} ILOCOperation;

// Estrutura para manter uma lista de operações ILOC
typedef struct {
    ILOCOperation* operations;
    int num_operations;
} ILOCOperationList;


// Função para criar uma nova operação ILOC
ILOCOperation* newILOCOperation(char* operation, char* campo_1, char* campo_2, char* campo_3);
// Função para adicionar uma operação à lista de operações ILOC
void addILOCOperation(ILOCOperationList* list, ILOCOperation* operation);

// Função para imprimir uma operação ILOC
void printILOCOperation(ILOCOperation* operation);

// Função para imprimir a lista de operações ILOC
void printILOCOperationList(ILOCOperationList* list);

