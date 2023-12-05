#pragma once
#ifndef _ARVORE_H_
#define _ARVORE_H_

#define LITERAL 0
#define IDENTIFIER 1
#define TYPE 2
#define CONTROL 3
#define OPERATOR 4 
#define EXPRESSION  5 

typedef struct lexical_value
{
	int token_line;
	char* token_value;
	int token_type;
} lexical_value_t;

typedef struct asd_tree {
  int number_of_children;
  struct asd_tree **children;
  struct lexical_value* label;
  char* lexeme;
} asd_tree_t;

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(lexical_value_t *label);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Funções que imprimem recursivamente a árvore.
 */
void print_node(asd_tree_t *node);
static void _print_node (FILE *foutput, asd_tree_t* node, int depth);
void print_tree(asd_tree_t *node);
static void _print_tree (FILE *foutput, asd_tree_t *node, int depth);
/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);

lexical_value_t* lexical_value_create(int token_type, char* token_value);

#endif //_ARVORE_H_
