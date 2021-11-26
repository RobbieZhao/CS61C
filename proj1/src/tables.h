#ifndef TABLES_H
#define TABLES_H

#include <stdint.h>

extern const int SYMTBL_NON_UNIQUE;      // allows duplicate names in table
extern const int SYMTBL_UNIQUE_NAME;     // duplicate names not allowed

/* Complete the following definition of SymbolTable and implement the following
   functions. You are free to declare additional structs or functions, but you
   must build this data structure yourself. 
 */

/* SOLUTION CODE BELOW */
typedef struct {
    char *name;
    uint32_t addr;
} Symbol;

typedef struct {
    Symbol* tbl;   // don't get confused, think of this as an array of Symbols instead of a pointer to a Symbol
    uint32_t len;  // number of entries in the table
    uint32_t cap;  // current capacity of the table
    int mode;
} SymbolTable;     // Based on this definition, it's natural to think of building a hash table

/* Helper functions: */

void allocation_failed();

void addr_alignment_incorrect();

void name_already_exists(const char* name);

void write_symbol(FILE* output, uint32_t addr, const char* name);

/* IMPLEMENT ME - see documentation in tables.c */
SymbolTable* create_table();

/* IMPLEMENT ME - see documentation in tables.c */
void free_table(SymbolTable* table);

/* IMPLEMENT ME - see documentation in tables.c */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr);

/* IMPLEMENT ME - see documentation in tables.c */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name);

/* IMPLEMENT ME - see documentation in tables.c */
void write_table(SymbolTable* table, FILE* output);

#endif