#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"
#include "helpers.h"

#define ARQUIVO_SAIDA "saida.dot"

asd_tree_t *asd_new(lexical_value_t *label)
{
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL){
    ret->label = label;
    ret->children = NULL;
    ret->number_of_children = 0;
  }
  return ret;
}

void asd_free(asd_tree_t *tree)
{
  if (tree != NULL){
    int i;
    for (i = 0; i < tree->number_of_children; i++){
      asd_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->label);
    free(tree);
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_add_child(asd_tree_t *tree, asd_tree_t *child)
{
  if (tree == NULL){
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, child);
    return;
  }

  if (child != NULL){
    tree->number_of_children++;
    tree->children = realloc(tree->children, tree->number_of_children * sizeof(asd_tree_t*));
    tree->children[tree->number_of_children-1] = child;
  }
}

/*
static void _asd_print (FILE *foutput, asd_tree_t *tree, int profundidade)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade, profundidade*2, "", tree->label, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++){
      _asd_print(foutput, tree->children[i], profundidade+1);
    }
  }else{
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}
*/

static void _print_node (FILE *foutput, asd_tree_t* node, int depth)
{
  if (node == NULL)
  {
    return;
  }

  fprintf(foutput, "%p [label=\"%d\"];\n", node, node->label->token_type);
  
  int i;
  for (i = 0; i < node->number_of_children; i++){
    _print_node(foutput, node->children[i], depth+1);
  }
}

void print_node(asd_tree_t *node)
{
  FILE *foutput = stderr;
  if (node != NULL){
    _print_node(foutput, node, 0);
  }
}

void print_tree(asd_tree_t *node)
{
  FILE *foutput = stderr;
  if (node != NULL){
    _print_tree(foutput, node, 0);
  }
}

static void _print_tree (FILE *foutput, asd_tree_t *node, int depth)
{
  if (node == NULL)
  {
    return;
  }

  int i;
  for (i = 0; i < node->number_of_children; i++){
    fprintf(foutput, "%p - %p\n", node, node->children[i]);
  }
  for (i = 0; i < node->number_of_children; i++){
    _print_tree(foutput, node->children[i], depth+1);
  }
}

static void _asd_print_graphviz (FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL){
    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label->token_value);
    for (i = 0; i < tree->number_of_children; i++){
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz(foutput, tree->children[i]);
    }
  }
}

void asd_print_graphviz(asd_tree_t *tree)
{
  FILE *foutput = fopen(ARQUIVO_SAIDA, "w+");
  if(foutput == NULL){
    printf("Erro: %s não pude abrir o arquivo [%s] para escrita.\n", __FUNCTION__, ARQUIVO_SAIDA);
  }
  if (tree != NULL){
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
    fclose(foutput);
  }
}

lexical_value_t* lexical_value_create(int token_type, char* token_value)
{
  lexical_value_t* value = (lexical_value_t*) malloc(sizeof(lexical_value_t));
  value->token_line = get_line_number();
  value->token_type = token_type;
  value->token_value = strdup(token_value);
}


void exporta(void *arvore)
{
  asd_tree_t* tree_node;
  tree_node = (asd_tree_t*) arvore;
  print_tree(arvore);
  print_node(arvore);
  asd_print_graphviz(arvore);
  return;
}