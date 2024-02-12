#include <stdio.h>
#include "iloc.h"
#include "table.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore);
TableList* global_table_list = NULL;
Table* global_table = NULL;

int main (int argc, char **argv)
{
  int ret = yyparse();
  //exporta (arvore);
  yylex_destroy();
  //printIlocOperations();
  printAssemblyInstructions();
  return ret;
}
