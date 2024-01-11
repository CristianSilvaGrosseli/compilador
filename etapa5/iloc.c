// Função para criar uma nova operação ILOC
OperacaoILOC* novaOperacaoILOC(char* operacao, char** argumentos, int num_argumentos) {
    OperacaoILOC* o = (OperacaoILOC*) malloc(sizeof(OperacaoILOC));
    o->operacao = strdup(operacao);
    o->argumentos = (char**) malloc(num_argumentos * sizeof(char*));
    for (int i = 0; i < num_argumentos; i++) {
        o->argumentos[i] = strdup(argumentos[i]);
    }
    o->num_argumentos = num_argumentos;
    return o;
}

// Função para adicionar uma operação à lista de operações ILOC
void adicionarOperacao(ListaOperacoesILOC* lista, OperacaoILOC* operacao) {
    lista->operacoes = (OperacaoILOC*) realloc(lista->operacoes, (lista->num_operacoes + 1) * sizeof(OperacaoILOC));
    lista->operacoes[lista->num_operacoes] = *operacao;
    lista->num_operacoes++;
}

// Função para imprimir uma operação ILOC
void imprimirOperacaoILOC(OperacaoILOC* operacao) {
    printf("%s ", operacao->operacao);
    for (int i = 0; i < operacao->num_argumentos; i++) {
        printf("%s ", operacao->argumentos[i]);
    }
    printf("\n");
}

// Função para imprimir a lista de operações ILOC
void imprimirListaOperacoesILOC(ListaOperacoesILOC* lista) {
    for (int i = 0; i < lista->num_operacoes; i++) {
        imprimirOperacaoILOC(&lista->operacoes[i]);
    }
}
