#include <stdio.h>
#include "iloc.h"
#include "command_block.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore);

int main (int argc, char **argv)
{
  int ret = yyparse();
  //exporta (arvore);
  yylex_destroy();
  //printLeaderGraphviz();
  printCommandBlock();
  return ret;
}
