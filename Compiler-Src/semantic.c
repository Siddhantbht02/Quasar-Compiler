#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"
#include "symtab.h"

void traverse_node(ASTNode* node);
SymbolType get_expression_type(ASTNode* node);

void check_semantics(ASTNode* ast_root) {
    symtab_init();
    traverse_node(ast_root);
}

SymbolType get_expression_type(ASTNode* node) {
    if (!node) return SYM_UNDEFINED;
    SymbolType result_type = SYM_UNDEFINED;
    switch(node->type) {
        case LITERAL_NODE: {
            LiteralNode* l = (LiteralNode*)node;
            if(l->literal_type==LIT_INT) result_type=SYM_INT;
            else if(l->literal_type==LIT_DUB) result_type=SYM_DUB;
            else if(l->literal_type==LIT_BOOL) result_type=SYM_BOOL;
            break;
        }
        case IDENTIFIER_NODE: {
            IdentifierNode* i = (IdentifierNode*)node;
            Symbol* s = symtab_lookup(i->name);
            if(s) { result_type = s->type; }
            else { fprintf(stderr, "Semantic Error: Var '%s' undeclared.\n", i->name); result_type = SYM_UNDEFINED; }
            break;
        }
        case BINARY_OP_NODE: {
            BinOpNode* b = (BinOpNode*)node;
            SymbolType l = get_expression_type(b->left);
            SymbolType r = get_expression_type(b->right);
            if(l == SYM_UNDEFINED || r == SYM_UNDEFINED) { result_type = SYM_UNDEFINED; break; }
            switch(b->op) {
                case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV:
                    if(l==SYM_BOOL||r==SYM_BOOL){fprintf(stderr,"Semantic Error: Arithmetic on Bool.\n");result_type=SYM_UNDEFINED;}
                    else if(l==SYM_DUB||r==SYM_DUB){result_type=SYM_DUB;}
                    else{result_type=SYM_INT;}
                    break;
                case OP_MOD:
                    if(l!=SYM_INT||r!=SYM_INT){fprintf(stderr,"Semantic Error: Modulo needs int.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=SYM_INT;}
                    break;
                case OP_LT: case OP_GT: case OP_LTE: case OP_GTE: case OP_EQ: case OP_NEQ:
                    if(l!=r){fprintf(stderr,"Semantic Error: Mismatched types in comparison.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=SYM_BOOL;}
                    break;
                case OP_AND: case OP_OR:
                    if(l!=SYM_BOOL||r!=SYM_BOOL){fprintf(stderr,"Semantic Error: Logical ops need bool.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=SYM_BOOL;}
                    break;
                case OP_ASSIGN:
                    if(b->left->type!=IDENTIFIER_NODE){fprintf(stderr,"Semantic Error: LHS of assign must be var.\n");result_type=SYM_UNDEFINED;}
                    else if(l!=r){fprintf(stderr,"Semantic Error: Type mismatch in assign.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=l;}
                    break;
            }
            break;
        }
        case UNARY_OP_NODE: {
            UnaryOpNode* u = (UnaryOpNode*)node;
            SymbolType o = get_expression_type(u->operand);
            if(o == SYM_UNDEFINED) { result_type = SYM_UNDEFINED; break; }
            switch(u->op) {
                case OP_NEG:
                    if(o!=SYM_INT&&o!=SYM_DUB){fprintf(stderr,"Semantic Error: Unary minus needs numeric.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=o;}
                    break;
                case OP_NOT:
                    if(o!=SYM_BOOL){fprintf(stderr,"Semantic Error: Logical not needs bool.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=SYM_BOOL;}
                    break;
                case OP_POST_INC: case OP_POST_DEC:
                    if(o!=SYM_INT){fprintf(stderr,"Semantic Error: Inc/dec needs int.\n");result_type=SYM_UNDEFINED;}
                    else{result_type=SYM_INT;}
                    break;
            }
            break;
        }
        case CALL_NODE: result_type = SYM_INT; break;
        default: result_type = SYM_UNDEFINED; break;
    }
    node->computed_type = result_type;
    return result_type;
}

void traverse_node(ASTNode* node) {
    if (!node) return;

    // --- NEW, CORRECTED TRAVERSAL LOGIC ---
    ASTNode* current = node;
    while (current != NULL) {
        if(current->type == BLOCK_NODE || current->type == FOR_NODE) symtab_enter_scope();

        switch(current->type) {
            case VAR_DECL_NODE: {
                VarDeclNode* d = (VarDeclNode*)current;
                SymbolType e = SYM_UNDEFINED;
                if(d->init_expr) { e = get_expression_type(d->init_expr); }
                symtab_insert(d->name, e);
                break;
            }
            case IF_NODE: {
                IfNode* i = (IfNode*)current;
                if(get_expression_type(i->condition) != SYM_BOOL) {
                    fprintf(stderr, "Semantic Error: 'if' condition must be Boolean.\n");
                }
                traverse_node(i->if_body);   // Recurse on child list
                traverse_node(i->else_body); // Recurse on child list
                break;
            }
            case WHILE_NODE: {
                WhileNode* w = (WhileNode*)current;
                if(get_expression_type(w->condition) != SYM_BOOL) {
                    fprintf(stderr, "Semantic Error: 'while' condition must be Boolean.\n");
                }
                traverse_node(w->body); // Recurse on child list
                break;
            }
            case FOR_NODE: {
                ForNode* f = (ForNode*)current;
                traverse_node(f->init);
                if(f->condition && get_expression_type(f->condition) != SYM_BOOL) {
                    fprintf(stderr, "Semantic Error: 'for' condition must be Boolean.\n");
                }
                traverse_node(f->update);
                traverse_node(f->body); // Recurse on child list
                break;
            }
            case BINARY_OP_NODE:
            case UNARY_OP_NODE:
            case CALL_NODE:
                get_expression_type(current);
                break;
            
            // Cases with no children to traverse
            case BREAK_NODE: case CONTINUE_NODE: case RETURN_NODE: case PRINT_NODE:
                break;
            
            default: break;
        }

        if(current->type == BLOCK_NODE || current->type == FOR_NODE) symtab_exit_scope();
        
        current = current->next; // Move to the next statement in the list
    }
}