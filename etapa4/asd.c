// Entrega 4
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#include "asd.h"
#include "helpers.h"

#define ARQUIVO_SAIDA "saida.dot"

asd_tree_t *asd_new(lexical_value_t *label, int arvore_node_type)
{
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL){
    ret->label = label;
    ret->children = NULL;
    ret->number_of_children = 0;
    ret->arvore_node_type = arvore_node_type;
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

static void _print_node (FILE *foutput, asd_tree_t* node, int depth)
{
  if (node == NULL)
  {
    return;
  }


  char* arvore_label = NULL;
  if (node->arvore_node_type == ARVORE_CALL)
  {
    char* prefix = "call ";
    arvore_label = malloc(strlen(prefix) + strlen(node->label->token_value) + 1);
    strcpy(arvore_label, prefix);
    strcat(arvore_label, node->label->token_value);
  }
  else
  {
    arvore_label = node->label->token_value;
  }
  fprintf(foutput, "%p [label=\"%s\"];\n", node, arvore_label);

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
    fprintf(foutput, "%p, %p\n", node, node->children[i]);
  }
  for (i = 0; i < node->number_of_children; i++){
    _print_tree(foutput, node->children[i], depth+1);
  }
}

static void _asd_print_graphviz_addresses(FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL){
    for (i = 0; i < tree->number_of_children; i++){
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz_addresses(foutput, tree->children[i]);
    }
  }
}

static void _asd_print_graphviz_labels(FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL){
    char* arvore_label = NULL;
    if (tree->arvore_node_type == ARVORE_CALL)
    {
      char* prefix = "call ";
      arvore_label = malloc(strlen(prefix) + strlen(tree->label->token_value) + 1);
      strcpy(arvore_label, prefix);
      strcat(arvore_label, tree->label->token_value);
    }
    else
    {
      arvore_label = tree->label->token_value;
    }

    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, arvore_label);

    for (i = 0; i < tree->number_of_children; i++){
      _asd_print_graphviz_labels(foutput, tree->children[i]);
    }
  }
}

void asd_print_graphviz(asd_tree_t *tree)
{
  FILE *foutput = fopen(ARQUIVO_SAIDA, "w+");
  if(foutput == NULL){
    printf("Erro: %s não pude abrir o arquivo [%s] para escrita.\n", __FUNCTION__, ARQUIVO_SAIDA);
  }

  fprintf(foutput, "digraph grafo {\n");
  fprintf(foutput, "label=\"Ref\";\n");
  if (tree != NULL){
    _asd_print_graphviz_addresses(foutput, tree);
    _asd_print_graphviz_labels(foutput, tree);
  }
  fprintf(foutput, "}\n");
  fclose(foutput);
}


lexical_value_t* lexical_value_create(int token_nature, char* token_value)
{
  lexical_value_t* value = (lexical_value_t*) malloc(sizeof(lexical_value_t));
  value->token_line = get_line_number();
  value->token_type = -1;
  value->token_nature = token_nature;
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
