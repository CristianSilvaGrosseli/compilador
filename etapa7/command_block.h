#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iloc.h"

typedef struct Leader {
    char* value;
    int numero_linha;
    IlocOperation* ilocOperation;
    struct Leader* next;
} Leader;

// Definição da estrutura do nó Desvio da lista
typedef struct Desvio {
    char* value; //"cbr" ou "jumpI"
    int numero_linha;
    char* alvo1;
    int linha_alvo_1;
    char* alvo2;
    int linha_alvo_2;
    IlocOperation* ilocOperation;

    struct Desvio* next;

} Desvio;

typedef struct Label{

    int numero_linha;
    char* label_id;
    IlocOperation* ilocOperation;

    struct Label* next;

} Label;

typedef struct BasicBlock{

    struct Leader* block_leader;
    struct IlocOperation* ilocInit;
    struct IlocOperation* ilocEnd;
    struct Desvio* block_desvio;
    

    int info;

    struct BasicBlock* next;


} BasicBlock;

void atribuirIDs(IlocOperationList* lista);
void printBasickBlocks(BasicBlock* Block_list);
BasicBlock* criarBasicBlock(Leader* leader, IlocOperation* ilocInit);
void inserirBasicBlock(BasicBlock** listaBlocos, BasicBlock* novoBloco);
void appendDesvioToList(Desvio** head, const char* value, int numero_linha, char* alvo1, char* alvo2, IlocOperation* ilocOperation);
Desvio* createDesvio(const char* value, int numero_linha,  char* alvo1, char* alvo2, IlocOperation* ilocOperation);
void appendLeaderToList(Leader** head, const char* value, int numero_linha, IlocOperation* ilocOperation);
Leader* createLeader(const char* value, int numero_linha, IlocOperation* ilocOperation);
int printCommandBlock();
void imprimirListaDesvios(Desvio* listaDesvios);
Label* criarLabel(int numero_linha, char* label_id, IlocOperation* ilocOperation);
void inserirLabel(Label** listaLabels, Label* novoLabel);
void imprimirListaLabels(Label* listaLabels);
void imprimirListaLeaders(Leader* listaLeaders);
IlocOperationList* encontrarCabeca(IlocOperationList* lista, IlocOperation* operacao);
void printGraph(BasicBlock* Block_list);
void liberarListaLabel(Label* listaLabel);
