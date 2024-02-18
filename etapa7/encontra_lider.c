#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100  // Define o máximo de linhas no arquivo
#define first_line 1

typedef struct Leader {
    char value[50];
    int numero_linha;
    struct Leader* next;
} Leader;

// Definição da estrutura do nó Desvio da lista
typedef struct Desvio {
    char value[50];
    int numero_linha;
    struct Desvio* next;
    char alvo1[20];
    char alvo2[20];
} Desvio;

// Função para criar um novo nó da lista
Leader* createLeader(const char* value, int numero_linha) {
    Leader* newLeader = (Leader*)malloc(sizeof(Leader));
    if (newLeader != NULL) {
        strcpy(newLeader->value, value);
        newLeader->numero_linha = numero_linha;
        newLeader->next = NULL;
    }
    return newLeader;
}

// Função para adicionar um novo valor à lista
void appendLeaderToList(Leader** head, const char* value, int numero_linha) {

    //Checa se lider ja existe na lista
    Leader* currentLeader = *head;
    while (currentLeader != NULL) {
        
        if(numero_linha == currentLeader->numero_linha)
        {
            printf("Leader already exists!\n");
            return;
        }
        currentLeader = currentLeader->next;
    }
    

    Leader* newLeader = createLeader(value, numero_linha);
    if (newLeader == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    if (*head == NULL) {
        *head = newLeader;
    } else {
        Leader* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newLeader;
    }
}



// Função para criar um novo nó da lista
Desvio* createDesvio(const char* value, int numero_linha,  char* alvo1, char* alvo2) {
    Desvio* newDesvio = (Desvio*)malloc(sizeof(Desvio));
    if (newDesvio != NULL) {
        strcpy(newDesvio->value, value);
        newDesvio->numero_linha = numero_linha;
        strcpy(newDesvio->alvo1, alvo1);
        if(alvo2 == NULL){strcpy(newDesvio->alvo2, NULL);}
        else
        {strcpy(newDesvio->alvo2, alvo2);}  
        newDesvio->next = NULL;
    }
    return newDesvio;
}

// Função para adicionar um novo valor à lista
void appendDesvioToList(Desvio** head, const char* value, int numero_linha, char* alvo1, char* alvo2) {
    Desvio* newDesvio = createDesvio(value, numero_linha, alvo1, alvo2);
    if (newDesvio == NULL) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    if (*head == NULL) {
        *head = newDesvio;
    } else {
        Desvio* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newDesvio;
    }
}

char *strdup_without_newline(const char *str) {
    // Encontra a posição do caractere de nova linha
    const char *newline_pos = strchr(str, '\n');
    
    // Se não encontrar, copia toda a string
    if (newline_pos == NULL) {
        return strdup(str);
    }

    // Calcula o comprimento da substring até o caractere de nova linha
    size_t length = newline_pos - str;

    // Aloca memória para a nova string sem o caractere de nova linha
    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(EXIT_FAILURE);
    }

    // Copia os caracteres até o caractere de nova linha
    strncpy(result, str, length);
    result[length] = '\0';  // Adiciona o caractere nulo de término

    return result;
}

char* extrair_destino(const char* instrucao) {
    // Encontra a posição do caractere ">"
    const char* posicao_marcador = strchr(instrucao, '>');
    
    // Se não encontrar o marcador, retorna NULL
    if (posicao_marcador == NULL)
        return NULL;

    // Retorna a string após o marcador ">"
    //return strdup(posicao_marcador + 2); // +2 para ignorar o ">" e o espaço seguinte
    return strdup_without_newline(posicao_marcador+2);
}


// Função para extrair os valores após "=>"
void extractValues(const char* input, char* value1, char* value2) {
    // Encontra a posição do marcador "=>"
    const char* arrowPos = strstr(input, "=>");
    
    // Se encontrar o marcador
    if (arrowPos != NULL) {
        // Avança para o próximo caractere após "=>"
        arrowPos += 3;
        
        // Extrai o primeiro valor (antes da vírgula)
        sscanf(arrowPos, "%[^,],", value1);
        
        // Extrai o segundo valor (depois da vírgula)
        //sscanf(arrowPos, "%*[^,], %s", value2);
        strcpy(value2, strdup_without_newline(arrowPos+4));
    } else {
        // Se não encontrar, retorna strings vazias
        value1[0] = '\0';
        value2[0] = '\0';
    }
}

int l_size(char* input){

    if(input != NULL){
    int i = 0;
    char c = i;
    while( input[i] != '\0'){
        //printf("valor de i: %c\n", input[i]);
        i++;
    }

    return i;
    }
}


int main() {
    FILE *arquivo;
    char linha[1000];
    

    Desvio* Desvio_list = NULL;
    Leader* Leader_list = NULL;

    arquivo = fopen("input.txt", "r");  // Abre o arquivo para leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo");
        return 1;
    }

    int numero_linha = 0;

    // Lê linha por linha até o final do arquivo
    while (fgets(linha, sizeof(linha), arquivo)) {
        numero_linha++;

        // Primeira linha do programa é instrução lider
        if (numero_linha == first_line) {
            
            appendLeaderToList(&Leader_list, linha, numero_linha);
            
        }

        // Procura pela palavra "cbr" na linha atual
        if (strstr(linha, "cbr") != NULL) {

            char alvo1[20], alvo2[20];
            extractValues(linha, alvo1, alvo2);
            
            appendDesvioToList(&Desvio_list, linha, numero_linha, alvo1, alvo2);

            appendLeaderToList(&Leader_list, "", numero_linha+1);

        }

        // Procura pela palavra "jumpI" na linha atual
        else if (strstr(linha, "jumpI") != NULL) {

            char alvo1[20]; 
            //printf("%s\n", alvo1);
            
            //extrair_destino2(linha, alvo1);
            //alvo1 = extrair_destino(linha);
            strcpy(alvo1, extrair_destino(linha));

            appendDesvioToList(&Desvio_list, linha, numero_linha, alvo1, "");

            appendLeaderToList(&Leader_list, "", numero_linha+1);

        }

        //Testa para checar se a linha atual é a linha após o desvio q foi feito
        Leader* current = Leader_list;
        while (current != NULL) {
        if(current->numero_linha == numero_linha) 
            {strcpy(current->value, linha);} 
        current = current->next;
        }
        free(current);

    }

    // Fecha o arquivo
    //    fclose(arquivo);

    rewind(arquivo); //volta para o início do arquivo

    numero_linha = 0;
    // Lê linha por linha até o final do arquivo para avaliar os ALVOS! Os líderes após desvios já foram definidos.
    while (fgets(linha, sizeof(linha), arquivo)){

    numero_linha++;

    //Procura na lista de desvios se há algum que possua o alvo da linha atual
    Desvio* currentDesvio = Desvio_list;
    while (currentDesvio != NULL) {

        printf("Procurando na linha %s se há o alvo 1 %s \n", linha, currentDesvio->alvo1);

        //if(strstr(linha[] , currentDesvio->alvo1) != NULL){
        //strcmp(linha, currentDesvio->alvo1)

        // printf("linha: %s\n", linha);
        // printf("alvo: %s\n", currentDesvio->alvo1);
        // printf("l_size: %d\n", l_size(currentDesvio->alvo1));
        
        printf("Procurando na linha: %s se há o alvo 1: %s \n", linha, currentDesvio->alvo1);
        if(strncmp(linha, currentDesvio->alvo1, l_size(currentDesvio->alvo1)) == 0 )
        {
            printf("encontrado alvo1! \n");
            appendLeaderToList(&Leader_list, linha, numero_linha);
        }
        
        if(!(strcmp(currentDesvio->alvo2, "") == 0 )){
            printf("Procurando na linha: %s se há o alvo 2: %s \n", linha, currentDesvio->alvo2);
            if(strncmp(linha, currentDesvio->alvo2, l_size(currentDesvio->alvo2)) == 0 )
            {
                printf("encontrado alvo2! \n");
                appendLeaderToList(&Leader_list, linha, numero_linha);
            }
        }

        currentDesvio = currentDesvio->next;
    }

    free(currentDesvio);

    }

    fclose(arquivo);
    
    printf("\n\n");
    //################################################
    // Imprimir a lista de desvios
    printf("Desvios:\n");
    Desvio* currentDesvio = Desvio_list;
    while (currentDesvio != NULL) {
        printf("%s", currentDesvio->value);
        printf("alvo 1:%s\n", currentDesvio->alvo1);
        printf("alvo 2:%s\n", currentDesvio->alvo2);
        printf("linha:%d\n", currentDesvio->numero_linha);
        currentDesvio = currentDesvio->next;
    }
    free(currentDesvio);

    printf("\n\n");

    // Imprimir a lista de lideres
    printf("Lideres:\n");
    Leader* currentLeader = Leader_list;
    while (currentLeader != NULL) {
        printf("%s", currentLeader->value);
        printf("linha:%d\n", currentLeader->numero_linha);
        currentLeader = currentLeader->next;
    }
    free(currentLeader);

    return 0;
}
