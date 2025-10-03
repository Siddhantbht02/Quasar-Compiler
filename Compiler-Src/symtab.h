#ifndef SYMTAB_H
#define SYMTAB_H

typedef enum {
    SYM_INT,
    SYM_DUB,
    SYM_BOOL,
    SYM_UNDEFINED
} SymbolType;

typedef struct Symbol {
    char *name;
    SymbolType type;
    struct Symbol *next;
} Symbol;

void symtab_init();
void symtab_enter_scope();
void symtab_exit_scope();
void symtab_insert(char* name, SymbolType type);
Symbol* symtab_lookup(char* name);

#endif // SYMTAB_H