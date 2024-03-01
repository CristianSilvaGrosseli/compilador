#include "command_block.h"
#include "iloc.h"
#define FIRST_LINE 1

// Função para criar um novo nó da lista
Leader* createLeader(const char* value, int numero_linha, IlocOperation* ilocOperation) {
    Leader* newLeader = (Leader*)malloc(sizeof(Leader));
    if (newLeader != NULL) {
        strcpy(newLeader->value, value);
        newLeader->numero_linha = numero_linha;
        newLeader->ilocOperation = ilocOperation;
        newLeader->ilocOperation->isLeader=1;
        newLeader->next = NULL;
    }
    return newLeader;
}


// Função para adicionar um novo valor à lista
void appendLeaderToList(Leader** head, const char* value, int numero_linha,  IlocOperation* ilocOperation) {

    //Checa se lider ja existe na lista
    Leader* currentLeader = *head;
    while (currentLeader != NULL) {
        
        if(numero_linha == currentLeader->numero_linha)
        {
            //printf("Leader already exists!\n");
            return;
        }
        currentLeader = currentLeader->next;
    }
    

    Leader* newLeader = createLeader(value, numero_linha, ilocOperation);
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
Desvio* createDesvio(const char* value, int numero_linha,  char* alvo1, char* alvo2, IlocOperation* ilocOperation) {
    //printf("entrou para criar desvio\n");

    Desvio* newDesvio = (Desvio*)malloc(sizeof(Desvio));


    if (newDesvio != NULL) {
        //printf("mallocou desvio:\n");
        
        newDesvio->ilocOperation = ilocOperation;
        newDesvio->value = strdup(value);
        newDesvio->numero_linha = numero_linha;

        if(strstr(value, "cbr"))
        {
        newDesvio->alvo1 = strdup(alvo1);
        newDesvio->alvo2 = strdup(alvo2);

        }

        if(strstr(value, "jumpI"))
        {
         newDesvio->alvo1 = strdup(alvo2);
         newDesvio->alvo2 = "";   
        }

    }

    return newDesvio;
}

// Função para adicionar um novo valor à lista
void appendDesvioToList(Desvio** head, const char* value, int numero_linha, char* alvo1, char* alvo2, IlocOperation* ilocOperation) {
    //printf("entrou para adicionar desvio\n");

    Desvio* newDesvio = createDesvio(value, numero_linha, alvo1, alvo2, ilocOperation);
    //printf("crisou o novo desvio\n");
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

    return;
}


int printCommandBlock() {

    printf("entrou na print command block\n");
    //função que percorre a variável global g_IlocOperations, que é uma lista
    //de instruções ILOC    
    //varre a lista, identifica as lideres
    //

    Desvio* Desvio_list = NULL;
    Leader* Leader_list = NULL;
    Label*  Label_list = NULL;
    IlocOperationList* current =  get_iloc_list();
    atribuirIDs(current);
    BasicBlock* Block_list = NULL;
    
   // printIlocOperations();

    int line_number = 0;

    //percorre para encontrar desvios
    while (current != NULL)
    {

        line_number++;
        printf("%d: ", line_number);

        IlocOperation* operation = current->operation;
        printIlocOperation(current->operation);

        if(line_number==FIRST_LINE){

        appendLeaderToList(&Leader_list, "FIRST", line_number, operation);
            
        }


        //testa se a operação atual é desvio condicional
        if(strstr(operation->mnemonico, "cbr")){
    
            appendDesvioToList(&Desvio_list, "cbr", line_number, operation->campo_1, operation->campo_2, operation);
        
            appendLeaderToList(&Leader_list, "after cbr", line_number+1, current->next_operation->operation);

        }

        //testa se a operação atual é desvio INcondicional
        if(strstr(operation->mnemonico, "jumpI")){
 
            appendDesvioToList(&Desvio_list, "jumpI", line_number, "", operation->campo_3, operation);
            appendLeaderToList(&Leader_list, "after jumpI", line_number+1, current->next_operation->operation);
        }

        if(strstr(operation->mnemonico, "label")){

            Label* newLabel = criarLabel(line_number, operation->campo_3, current->operation);
            inserirLabel(&Label_list, newLabel);
        }

        //fazer uma segunda passagem para encontrar qual a linha dos alvos dos desvios!

    current = current->next_operation;

    }

    //printf("terminou primeira passagem!\n");

    //IlocOperationList* current_iloc =  get_iloc_list();
    //line_number = 0;

    //agora, percorrer a lista de labels para preenhcer a linha dos alvos

    Desvio* current_desvio = Desvio_list;
    Label* current_label;

    //para cada desvio da lista, verifica qual a linha do alvo do desvio
    while(current_desvio != NULL){

        current_label = Label_list;
        
        //percorre a lista de labels
        while(current_label != NULL){

            if((strstr(current_desvio->alvo1, current_label->label_id))){

                //printf("encontrou ocorrencia de alvo 1\n");

                current_desvio->linha_alvo_1 = current_label->numero_linha;

                appendLeaderToList(&Leader_list, "alvo jump", current_label->numero_linha, current_label->ilocOperation);
               
            }

            if((strstr(current_desvio->alvo2, current_label->label_id))){

                //printf("encontrou ocorrencia de alvo 2\n");

                current_desvio->linha_alvo_2 = current_label->numero_linha;          
                appendLeaderToList(&Leader_list, "alvo jump", current_label->numero_linha, current_label->ilocOperation);
            }

            current_label = current_label->next;

        }

        current_desvio = current_desvio->next;
    }

    imprimirListaDesvios(Desvio_list);
    //imprimirListaLabels(Label_list);
    imprimirListaLeaders(Leader_list);

    //ESTÁ IDENTIFICANDO AS FUNCOES LIDERES APÓS DESVIOS. NECESSÁRIO IDENTIFICAR AS ALVO DE DESVIO - OK
    //SALVAR A INSTRUCAO ILOC NO LABEL PARA PODER RECUPERAR DPS.

    //AGORA, É NECESSÁRIO COMPOR OS BLOCOS BÁSICOS.
    //OS BLOCOS BÁSICOS POSSUEM: UMA INSTRUÇÃO LIDER + INSTRUCOES ATÉ O BLOCO ANTERIOR.

    Leader* current_leader = Leader_list;

    //percorre lista de lideres e define o início de cada bloco!
    while(current_leader != NULL){

        BasicBlock* newBlock = criarBasicBlock(current_leader, current_leader->ilocOperation);

        inserirBasicBlock(&Block_list, newBlock);

        current_leader = current_leader->next;
    }

    //printBasickBlocks(Block_list);

    //percorrer a lista de blocks para determinar a ilocEnd
    BasicBlock* current_block = Block_list;
    IlocOperationList* iloc_list =  get_iloc_list();

    //Desvio* Desvio_list = NULL;
    //Leader* Leader_list = NULL;
    //Label*  Label_list = NULL;
    //IlocOperationList* current =  get_iloc_list();
    //BasicBlock* Block_list = NULL;


//encontrarCabeca(IlocOperationList* lista, IlocOperation* operacao)

    //preenchido sobre as instrucoes de ilocEnd
    while(current_block != NULL){
        
        IlocOperationList* current_iloc = encontrarCabeca(iloc_list, current_block->ilocInit);
        

        while(current_iloc != NULL){
            //current_iloc = encontrarCabeca(iloc_list, current_block->ilocInit);

            if(current_iloc->next_operation != NULL){   
                         
                if(current_iloc->next_operation->operation->isLeader == 1 ){
                    //printf("caiu aqui\n");
                  current_block->ilocEnd = current_iloc->operation;

                  printf("\ncurrent_block->ilocinit->id: %d\n", current_block->ilocInit->id);
                  printf("current_block->ilocEnd->id: %d\n", current_block->ilocEnd->id);
                  //current_block = current_block->next;
                  current_iloc = current_iloc->next_operation;
                  break;
                }

            }

            else if(current_iloc->next_operation == NULL){

            if(current_iloc->operation->isLeader == 1){
                printf("encontrou final da lista de lideres\n");

                current_block->ilocEnd = current_iloc->operation;

                printf("\ncurrent_block->ilocinit->id: %d\n", current_block->ilocInit->id);
                printf("current_block->ilocEnd->id: %d\n", current_block->ilocEnd->id);

            }

            }


            current_iloc = current_iloc->next_operation;
        
        }

        current_block = current_block->next;
    }
    
    //agora, é necessário definir qual o link de cada basic block.

    //para isso, vamos percorrer a lista de basic blocks criadas e, para cada ilocEnd, vamos verificar se 
    //faz parte da lista de desvios.
    //para isso, vamos precisar de uma função que receba como entrada uma instrução iloc e verifique
    //se a instrução está contida na lista de desvios.
    //se estiver, preencher a informação de q aql basick block possui alvos.
    //definir os alvos como null, por default.(necessário modificar a sruct basic_block e a funcao (cria_bloco_basico))

    //percorrer a lista de blocks para determinar a BasicBlock->block_desvio
    current_block = Block_list;


    while(current_block != NULL){
        //IlocOperationList* current_iloc = encontrarCabeca(iloc_list, current_block->ilocEnd); //parte de iloc end

            printf("verificando o bloco: %d\n ", current_block->block_leader->ilocOperation->id);
            current_desvio = Desvio_list;

            //consultar a current_block.ilocEnd para preencher  quais os alvos 
            //da current_block.desvio
            printf("entrou aqui\n");

            //if(isDesvio( current_iloc))
            while(current_desvio!=NULL){

                printf("entrou aqui 2 \n");

                printf("%s\n", current_desvio->value);
                printf("current_block->ilocEnd->id: %d\n", current_block->ilocEnd->id);
                printf("current_desvio->ilocOperation->id: %d\n", current_desvio->ilocOperation->id);
                printf("current_desvio->linha_alvo_1: %d\n", current_desvio->linha_alvo_1);
                //printf("current_block->block_desvio->linha_alvo_1: %d\n", current_block->block_desvio->linha_alvo_1);
                

                if(current_block->ilocEnd->id == current_desvio->ilocOperation->id)
                {

                    printf("entrou aqui 3 \n");
                   // printf("current_block->block_desvio->linha_alvo_1: %d\n", current_block->block_desvio->linha_alvo_1);
                   // printf("current_desvio->linha_alvo_1: %d", current_desvio->linha_alvo_1);
                   
                    current_block->block_desvio->linha_alvo_1  =  current_desvio->linha_alvo_1;
                   

                    if(strstr(current_desvio->ilocOperation->mnemonico,"cbr") )
                    {

                        printf("entrou aqui 4 \n");
                        current_block->block_desvio->linha_alvo_2 = current_desvio->linha_alvo_2;
                    }

                } 

                current_desvio = current_desvio->next;

            }        


        current_block = current_block->next;
    }
    

    liberarListaLabel(Label_list);

    printBasickBlocks(Block_list);

   printGraph(Block_list);

    return 1;

}


 void printGraph(BasicBlock* Block_list){

    BasicBlock* currentLeaderGraphic = Block_list;
    printf("entrou aqui");
    FILE *foutput = fopen("CFD.dot", "w");
    fprintf(foutput, "digraph grafo {\n");

    fprintf(foutput, "label=\"Ref\";\n");
    while(currentLeaderGraphic != NULL)
    {

    if(currentLeaderGraphic->block_desvio->linha_alvo_1 != 0){
        fprintf(foutput, "  %d -> %d;\n", currentLeaderGraphic->ilocEnd->id , currentLeaderGraphic->block_desvio->linha_alvo_1);
            if(currentLeaderGraphic->block_desvio->linha_alvo_2 != 0){
                fprintf(foutput, "  %d -> %d;\n", currentLeaderGraphic->ilocEnd->id , currentLeaderGraphic->block_desvio->linha_alvo_2);
            }
            
        }
    currentLeaderGraphic = currentLeaderGraphic->next;
    }
    fprintf(foutput, "}\n");
    fclose(foutput);

    printf("escreveu no arquivo!\n");
    return;

}


void liberarListaLabel(Label* listaLabel) {
    Label* temp;
    while (listaLabel != NULL) {
        temp = listaLabel;
        listaLabel = listaLabel->next;
        // Liberar memória alocada para label_id
        free(temp->label_id);
        // Liberar memória alocada para ilocOperation, se necessário
        // free(temp->ilocOperation);  // Descomente esta linha se ilocOperation precisar ser liberado
        free(temp);
    }
}

// Função para percorrer a lista de operações ILOC e atribuir IDs únicos
void atribuirIDs(IlocOperationList* lista) {
    printf("atribuindo ids!\n");
    int id = 1;
    IlocOperationList* current = lista;
    while (current != NULL) {
        current->operation->id = id;
        id++;
        current = current->next_operation;
    }
}

// Função para inserir um novo nó de label na lista de labels
void inserirLabel(Label** listaLabels, Label* novoLabel) {
    if (*listaLabels == NULL) {
        *listaLabels = novoLabel;
    } else {
        Label* temp = *listaLabels;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novoLabel;
    }
}

IlocOperationList* encontrarCabeca(IlocOperationList* lista, IlocOperation* operacao) {
    while (lista != NULL) {
        if (lista->operation == operacao) {
            return lista;
        }
        lista = lista->next_operation;
    }
    return NULL; // Se a operação não for encontrada na lista
}

void printBasickBlocks(BasicBlock* Block_list){

    BasicBlock* current_basic_block = Block_list;
    int counter = 0;


    while(current_basic_block != NULL){

        
        printf("\nBloco %d: \n", current_basic_block->block_leader->ilocOperation->id);
        printf("iloc init:");
        printIlocOperation(current_basic_block->ilocInit);
        printf("iloc end:");
        printIlocOperation(current_basic_block->ilocEnd);
       

        if(current_basic_block->block_desvio!=NULL){

             printf("\nalvos bloco basico %d: ", current_basic_block->block_leader->ilocOperation->id);

            printf("\nalvo 1: %d\n", current_basic_block->block_desvio->linha_alvo_1);

            if(strstr(current_basic_block->ilocEnd->mnemonico,"cbr") ){
                printf("alvo 2: %d\n", current_basic_block->block_desvio->linha_alvo_2);
              }

        }
        current_basic_block = current_basic_block->next;
    }


    }


    // número da linha da instrução líder,
    // seguido de um traço,
    // terminado pelo número da linha da última instrução do bloco básico
 


// Função para criar um novo nó BasicBlock
BasicBlock* criarBasicBlock(Leader* leader, IlocOperation* ilocInit) {
    BasicBlock* novoBloco = (BasicBlock*)malloc(sizeof(BasicBlock));
    if (novoBloco == NULL) {
        printf("Erro: Falha ao alocar memória para novo bloco básico.\n");
        exit(EXIT_FAILURE);
    }
    novoBloco->block_leader = leader;
    novoBloco->ilocInit=leader->ilocOperation;
    novoBloco->block_desvio = (Desvio*)malloc(sizeof(Desvio));;
    novoBloco->next = NULL;


    return novoBloco;
}

// Função para inserir um novo nó BasicBlock na lista de blocos básicos
void inserirBasicBlock(BasicBlock** listaBlocos, BasicBlock* novoBloco) {
    if (*listaBlocos == NULL) {
        *listaBlocos = novoBloco;
    } else {
        BasicBlock* temp = *listaBlocos;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novoBloco;
    }
}

void imprimirListaLeaders(Leader* listaLeaders) {

    Leader* temp = listaLeaders;

    printf("\nInstrucoes lideres: \n");

    while (temp != NULL) {
        
        printIlocOperation(temp->ilocOperation);
        printf("ID: %d\n", temp->ilocOperation->id);

        temp = temp->next;
    }
}

// Função para imprimir a lista de labels (para fins de depuração)
void imprimirListaLabels(Label* listaLabels) {
    Label* temp = listaLabels;
    while (temp != NULL) {
        printf("Número da linha: %d, Label ID: %s\n", temp->numero_linha, temp->label_id);
        temp = temp->next;
    }
}

// Função para imprimir a lista de desvios (para fins de depuração)
void imprimirListaDesvios(Desvio* listaDesvios) {
    Desvio* temp = listaDesvios;
    while (temp != NULL) {
        printf("Valor: %s, Numero Linha: %d, Alvo1: %s, Linha Alvo1: %d, Alvo2: %s, Linha Alvo2: %d\nInstrucao:",
               temp->value, temp->numero_linha, temp->alvo1, temp->linha_alvo_1, temp->alvo2, temp->linha_alvo_2);
               printIlocOperation(temp->ilocOperation);
        temp = temp->next;
    }
}

// Função para criar um novo nó de label
Label* criarLabel(int numero_linha, char* label_id,  IlocOperation* ilocOperation) {
    Label* novoLabel = (Label*)malloc(sizeof(Label));
    if (novoLabel == NULL) {
        printf("Erro: Falha ao alocar memória para novo label.\n");
        exit(EXIT_FAILURE);
    }
    novoLabel->numero_linha = numero_linha;
    novoLabel->label_id = strdup(label_id);
    novoLabel->ilocOperation = ilocOperation;
    novoLabel->next = NULL;
    return novoLabel;
}

