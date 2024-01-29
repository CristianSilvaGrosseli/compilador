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
typedef struct IlocOperation{
    
    char* mnemonico;
    char* campo_1;
    char* campo_2;
    char* campo_3;

} IlocOperation;

// Estrutura para manter uma lista de operações ILOC
typedef struct IlocOperationList{
    IlocOperation* operation;
    struct IlocOperationList* next_operation;
} IlocOperationList;


// Função para criar uma nova operação ILOC
IlocOperation* newIlocOperation(char* operation, char* campo_1, char* campo_2, char* campo_3);

// Função para adicionar uma operação à lista de operações ILOC
IlocOperationList* addIlocOperation(IlocOperation* newIlocOperation);

// Função para imprimir uma operação ILOC
void printIlocOperation(IlocOperation* operation);

// Função para imprimir a lista de operações ILOC
void printIlocOperationList(IlocOperationList* list);

//store r1 => r2 // Memória(r2) = r1
IlocOperation* store_operation(char* parameter_1, int parameter_2);

void printIlocOperations();

