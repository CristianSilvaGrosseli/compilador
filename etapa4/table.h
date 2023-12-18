

/* Estrutura representando cada entrada de uma tabela, implementada como uma lista simplesmente encadeada. */
typedef struct table
{
	lexical_value_t* info;
	struct table *next;

} Table;

/* Estrutura representando cada tabela de uma lista de tabelas de simbolos, implementada como uma lista duplamente encadeada. */
typedef struct table_list
{
	struct table_list* next;
	struct table_list* previous;
	Table* symbol_table;

} TableList;

void push_table(TableList** list, Table* new_table);
void pop_table(TableList** list);