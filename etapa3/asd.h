#pragma once
#ifndef _ARVORE_H_
#define _ARVORE_H_

#define LITERAL 0
#define IDENTIFIER 1
#define EXPRESSION_OPERATOR 2
#define LANGUAGE_OPERATOR 3
#define CONTROL 4
#define TYPE 5
#define SYNTAX_TOKEN 6
#define FUNCTION_CALL 7

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
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);

static void _asd_print_dfs(FILE *foutput, asd_tree_t* root);

lexical_value_t* lexical_value_create(int token_type, char* token_value);

#endif //_ARVORE_H_
