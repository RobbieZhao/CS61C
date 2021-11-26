
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char* name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE* output, uint32_t addr, const char* name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

char* copy_str(char* original) {
    size_t str_len = strlen(original);

    const char *copied = (char *)malloc(str_len + 1);
    if (copied == NULL) {
        allocation_failed();
    }

    strcpy(copied, original);
    memset(copied, '\0', str_len);
}

/*
 * Find the index at which the Symbol has the name.
 *
 * If there's no Symbol with the name, return -1.
 */
int find(Symbol *tbl, char *name, int len) {
    for (int i = 0; i < len; i++) {
        if (strcmp(tbl[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time. 
   If memory allocation fails, you should call allocation_failed(). 
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
// TODO: What does it mean to have non-unique names? 
SymbolTable* create_table(int mode) {
    // Allocate a table
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));  // SymbolTable is a type, so sizeof(SymbolTable)
    if (table == NULL) {
        allocation_failed();
    }

    // Initialize the fields
    int initial_capacity = 10;

    table->tbl = (Symbol *)malloc(initial_capacity * sizeof(Symbol));
    if (table->tbl == NULL) {
        allocation_failed();
    }
    table->len = 0;
    table->cap = initial_capacity;
    table->mode = mode;

    return table;
}

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

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable* table) {
    // First free table->tbl
    Symbol *symbols = table->tbl;
    for (uint32_t i = 0; i < table->cap; i++) {
        // Whether we need to free name depends on whether it's
        // allocated on the stack or the heap
        free(symbols[i].name);
    }
    free(symbols);

    // Side note: I tried freeing symbols[i], and since that is a Symbol, it doesn't work,
    //            then I realized there's no need, since symbols are malloced, which includes symbols[i]

    // Then free the table itself
    free(table);
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE. 
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added. 

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists 
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed(). 

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable* table, const char* name, uint32_t addr) {
    // Sanity check
    if (addr % 4 != 0) {
        addr_alignment_incorrect();
        return -1;
    }
    
    // If on SYMTBL_NON_UNIQUE mode and the name already exists, return -1
    if (table->mode == SYMTBL_NON_UNIQUE) {
        int index = find(table->tbl, name, table->len);
        if (index != -1) {
            name_already_exists(name);
            return -1;
        }
    }

    // Add the entry 
    table->tbl[table->len].name = copy_str(name);
    table->tbl[table->len].addr = addr;
    table->len = table->len + 1;

    if (table->len == table->cap) {
        // 1. create a new array
        Symbol *new_tbl = (Symbol *)malloc(2 * table->cap * sizeof(Symbol));
        if (new_tbl == NULL) {
            allocation_failed();
        }
        // 2. copy the old array into the new array
        memcpy(new_tbl, table->tbl, table->cap * sizeof(Symbol));
        // 3. free the old array
        free(table->tbl);

        table->cap *= 2;
    }

    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable* table, const char* name) {
    int index = find(table->tbl, name, table->len);
    if (index == -1) {
        return -1;
    }

    return table->tbl[index].addr;  // ->, [], . same level of precedence, and left-associative
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable* table, FILE* output) {
    uint32_t table_len = table->len;
    for (uint32_t i = 0; i < table_len; i++) {
        write_symbol(output, table->tbl[i].addr, table->tbl[i].name);
    }
}
