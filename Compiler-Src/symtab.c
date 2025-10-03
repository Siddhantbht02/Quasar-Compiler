#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#define MAX_SCOPES 100
typedef struct Scope { Symbol *head; } Scope;
Scope* scope_stack[MAX_SCOPES];
int scope_top = -1;
void symtab_init() { symtab_enter_scope(); }
void symtab_enter_scope() { if(scope_top>=MAX_SCOPES-1)return; Scope* n=(Scope*)malloc(sizeof(Scope)); n->head=NULL; scope_stack[++scope_top]=n; }
void symtab_exit_scope() {
    if(scope_top<0)return; Symbol*c=scope_stack[scope_top]->head;
    while(c){Symbol*t=c;c=c->next;free(t->name);free(t);}
    free(scope_stack[scope_top]); scope_top--;
}
void symtab_insert(char* name, SymbolType type) {
    if(scope_top<0)return; Symbol*c=scope_stack[scope_top]->head;
    while(c){if(strcmp(c->name,name)==0){fprintf(stderr,"Semantic Error: Var '%s' redeclared.\n",name);return;}c=c->next;}
    Symbol*n=(Symbol*)malloc(sizeof(Symbol));n->name=strdup(name);n->type=type;n->next=scope_stack[scope_top]->head;scope_stack[scope_top]->head=n;
}
Symbol* symtab_lookup(char* name) {
    for(int i=scope_top;i>=0;i--){ Symbol*c=scope_stack[i]->head;
    while(c){if(strcmp(c->name,name)==0)return c;c=c->next;}}
    return NULL;
}