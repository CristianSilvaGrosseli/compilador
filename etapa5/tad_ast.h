// Entrega 5
// Cristian Silva Grosseli - 00243693
// Iuri Mendonça Tinti - 00278043

#pragma once
#ifndef _ARVORE_H_
#define _ARVORE_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define LITERAL 0
#define IDENTIFIER 1
#define TYPE 2
#define CONTROL 3
#define OPERATOR 4 
#define EXPRESSION  5 

#define TOKEN_TYPE_INT 1
#define TOKEN_TYPE_FLOAT 2
#define TOKEN_TYPE_BOOL 3

#define ARVORE_CALL 1

typedef struct lexical_value
{
	int token_line;
	char* token_value;
	int token_type;
  int token_nature;
  int register_number;
} lexical_value_t;

typedef struct node_ast_tree {
  int number_of_children;
  struct node_ast_tree **children;
  struct lexical_value* label;
  int arvore_node_type;
} node_ast_tree_t;

/*
 * Função ast_new, cria um nó sem filhos com o label informado.
 */
node_ast_tree_t *ast_new(lexical_value_t *label, int arvore_node_type);

/*
 * Função node_ast_tree, libera recursivamente o nó e seus filhos.
 */
void ast_free(node_ast_tree_t *tree);

/*
 * Função ast_add_child, adiciona child como filho de tree.
 */
void ast_add_child(node_ast_tree_t *tree, node_ast_tree_t *child);

/*
 * Funções que imprimem recursivamente a árvore.
 */
void print_node(node_ast_tree_t *node);
static void _print_node (FILE *foutput, node_ast_tree_t* node, int depth);
void print_tree(node_ast_tree_t *node);
static void _print_tree (FILE *foutput, node_ast_tree_t *node, int depth);
/*
 * Função ast_print_graphviz, idem, em formato DOT
 */
void ast_print_graphviz (node_ast_tree_t *tree);

lexical_value_t* lexical_value_create(int token_type, char* token_value);

#endif //_ARVORE_H_
