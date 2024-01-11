#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Leve em conta que os argumentos das ope-
rações podem ser nomes de temporários, valores cons-
tantes, ou nomes de rótulos */ //---------->>>> FAZER O RELEASE QND LIBERAR AS STRUCTS


// Estrutura para representar uma operação ILOC
typedef struct {
    char* operacao;
    char** argumentos;
    int num_argumentos;
} OperacaoILOC;

// Estrutura para manter uma lista de operações ILOC
typedef struct {
    OperacaoILOC* operacoes;
    int num_operacoes;
} ListaOperacoesILOC;
