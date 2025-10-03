#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "icg.h"

static Quad* tac_head = NULL; static Quad* tac_tail = NULL;
static int temp_counter = 0; static int label_counter = 0;

Address traverse_expr(ASTNode* node);
void traverse_stmt(ASTNode* node, Address loop_start, Address loop_end);

Address new_temp() { Address a; a.type = ADDR_TEMP; a.value.temp_id = temp_counter++; return a; }
Address new_label() { Address a; a.type = ADDR_LABEL; a.value.label_id = label_counter++; return a; }
void emit(TacOpcode o, Address r, Address a1, Address a2) { Quad* q = (Quad*)malloc(sizeof(Quad)); q->op = o; q->result = r; q->arg1 = a1; q->arg2 = a2; q->next = NULL; if(!tac_head) { tac_head = tac_tail = q; } else { tac_tail->next = q; tac_tail = q; } }
void print_address(Address a) { switch(a.type) { case ADDR_VARIABLE: printf("%s", a.value.variable_name); break; case ADDR_CONSTANT: printf("%d", a.value.int_constant); break; case ADDR_TEMP: printf("t%d", a.value.temp_id); break; case ADDR_LABEL: printf("L%d", a.value.label_id); break; default: break; } }

void print_icg(Quad* h) {
    printf("\n--- Intermediate Code (TAC) ---\n"); Quad* c = h; const char* o = "";
    while(c) { switch(c->op) {
        case TAC_ASSIGN: print_address(c->result); printf(" = "); print_address(c->arg1); break;
        case TAC_ADD: o="+"; goto pb; case TAC_SUB: o="-"; goto pb; case TAC_MUL: o="*"; goto pb; case TAC_DIV: o="/"; goto pb;
        case TAC_MOD: o="%%"; goto pb; case TAC_EQ: o="=="; goto pb; case TAC_NEQ: o="!="; goto pb; case TAC_LT: o="<"; goto pb;
        case TAC_GT: o=">"; goto pb; case TAC_LTE: o="<="; goto pb; case TAC_GTE: o=">="; goto pb; case TAC_AND: o="&&"; goto pb;
        case TAC_OR: o="||"; goto pb; pb: print_address(c->result); printf(" = "); print_address(c->arg1); printf(" %s ", o); print_address(c->arg2); break;
        case TAC_NEG: print_address(c->result); printf(" = -"); print_address(c->arg1); break;
        case TAC_NOT: print_address(c->result); printf(" = !"); print_address(c->arg1); break;
        case TAC_LABEL: printf("L%d:", c->arg1.value.label_id); break;
        case TAC_GOTO: printf("GOTO L%d", c->arg1.value.label_id); break;
        case TAC_IFZ: printf("IFZ "); print_address(c->arg1); printf(" GOTO L%d", c->arg2.value.label_id); break;
        case TAC_RETURN: printf("RETURN "); if(c->arg1.type != ADDR_UNDEF) print_address(c->arg1); break;
        case TAC_PRINT_STR: printf("PRINT \"%s\"", c->arg1.value.variable_name); break;
        default: break;
    } printf("\n"); c = c->next; }
    printf("-----------------------------\n");
}

Address traverse_expr(ASTNode* n) {
    if(!n) return (Address){ADDR_UNDEF};
    switch(n->type) {
        case LITERAL_NODE: { LiteralNode* l = (LiteralNode*)n; Address a = {ADDR_CONSTANT}; if(l->literal_type==LIT_INT)a.value.int_constant=l->value.int_val; if(l->literal_type==LIT_BOOL)a.value.int_constant=l->value.bool_val; if(l->literal_type==LIT_DUB)a.value.int_constant=(int)l->value.dub_val; return a; }
        case IDENTIFIER_NODE: { IdentifierNode* i = (IdentifierNode*)n; Address a = {ADDR_VARIABLE,.value.variable_name=i->name}; return a; }
        case BINARY_OP_NODE: {
            BinOpNode* b = (BinOpNode*)n;
            if(b->op==OP_AND){ Address r=new_temp(),lf=new_label(),le=new_label(),la=traverse_expr(b->left); emit(TAC_IFZ,(Address){ADDR_UNDEF},la,lf); Address ra=traverse_expr(b->right); emit(TAC_ASSIGN,r,ra,(Address){ADDR_UNDEF}); emit(TAC_GOTO,(Address){ADDR_UNDEF},le,(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},lf,(Address){ADDR_UNDEF}); emit(TAC_ASSIGN,r,(Address){ADDR_CONSTANT,.value.int_constant=0},(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},le,(Address){ADDR_UNDEF}); return r; }
            Address la=traverse_expr(b->left), ra=traverse_expr(b->right);
            if(b->op>=OP_ASSIGN && b->op<=OP_MOD_ASSIGN) { if(b->op==OP_ADD_ASSIGN){ Address t=new_temp(); emit(TAC_ADD,t,la,ra); ra=t; } emit(TAC_ASSIGN,la,ra,(Address){ADDR_UNDEF}); return la; }
            else{ Address t=new_temp(); TacOpcode o=TAC_UNDEF; if(b->op==OP_ADD)o=TAC_ADD; if(b->op==OP_SUB)o=TAC_SUB; if(b->op==OP_MUL)o=TAC_MUL; if(b->op==OP_DIV)o=TAC_DIV; if(b->op==OP_MOD)o=TAC_MOD; if(b->op==OP_EQ)o=TAC_EQ; if(b->op==OP_NEQ)o=TAC_NEQ; if(b->op==OP_LT)o=TAC_LT; if(b->op==OP_GT)o=TAC_GT; if(b->op==OP_LTE)o=TAC_LTE; if(b->op==OP_GTE)o=TAC_GTE; if(b->op==OP_OR)o=TAC_OR; emit(o,t,la,ra); return t; }
        }
        case UNARY_OP_NODE: { UnaryOpNode* u = (UnaryOpNode*)n; Address oa=traverse_expr(u->operand); if(u->op==OP_POST_INC){ emit(TAC_ADD,oa,oa,(Address){ADDR_CONSTANT,.value.int_constant=1}); return oa; } Address t=new_temp(); TacOpcode o=(u->op==OP_NEG)?TAC_NEG:TAC_NOT; emit(o,t,oa,(Address){ADDR_UNDEF}); return t; }
        default: return (Address){ADDR_UNDEF};
    }
}

void traverse_stmt(ASTNode* node, Address loop_start, Address loop_end) {
    if (!node) return;

    // --- NEW, CORRECTED TRAVERSAL LOGIC ---
    ASTNode* current = node;
    while (current != NULL) {
        if(current->type == BLOCK_NODE || current->type == FOR_NODE) symtab_enter_scope();

        switch(current->type) {
            case VAR_DECL_NODE: { VarDeclNode* d = (VarDeclNode*)current; if(d->init_expr){ Address v={ADDR_VARIABLE,.value.variable_name=d->name}; Address e=traverse_expr(d->init_expr); emit(TAC_ASSIGN,v,e,(Address){ADDR_UNDEF});} break; }
            case PRINT_NODE: { PrintNode* p = (PrintNode*)current; Address s={ADDR_STRING_LITERAL,.value.variable_name=p->string_to_print}; emit(TAC_PRINT_STR,(Address){ADDR_UNDEF},s,(Address){ADDR_UNDEF}); break; }
            case IF_NODE: { IfNode* i=(IfNode*)current; Address lel=new_label(),lei=new_label(),c=traverse_expr(i->condition); emit(TAC_IFZ,(Address){ADDR_UNDEF},c,lel); traverse_stmt(i->if_body,loop_start,loop_end); emit(TAC_GOTO,(Address){ADDR_UNDEF},lei,(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},lel,(Address){ADDR_UNDEF}); if(i->else_body){traverse_stmt(i->else_body,loop_start,loop_end);} emit(TAC_LABEL,(Address){ADDR_UNDEF},lei,(Address){ADDR_UNDEF}); break; }
            case WHILE_NODE: { WhileNode* w=(WhileNode*)current; Address wls=new_label(),wle=new_label(); emit(TAC_LABEL,(Address){ADDR_UNDEF},wls,(Address){ADDR_UNDEF}); Address c=traverse_expr(w->condition); emit(TAC_IFZ,(Address){ADDR_UNDEF},c,wle); traverse_stmt(w->body,wls,wle); emit(TAC_GOTO,(Address){ADDR_UNDEF},wls,(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},wle,(Address){ADDR_UNDEF}); break; }
            case FOR_NODE: { ForNode* f=(ForNode*)current; Address fls=new_label(),fle=new_label(); traverse_stmt(f->init,(Address){ADDR_UNDEF},(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},fls,(Address){ADDR_UNDEF}); if(f->condition){Address c=traverse_expr(f->condition);emit(TAC_IFZ,(Address){ADDR_UNDEF},c,fle);} traverse_stmt(f->body,fls,fle); traverse_stmt(f->update,(Address){ADDR_UNDEF},(Address){ADDR_UNDEF}); emit(TAC_GOTO,(Address){ADDR_UNDEF},fls,(Address){ADDR_UNDEF}); emit(TAC_LABEL,(Address){ADDR_UNDEF},fle,(Address){ADDR_UNDEF}); break; }
            case RETURN_NODE: { ReturnNode* r=(ReturnNode*)current; Address ra=traverse_expr(r->return_expr); emit(TAC_RETURN,(Address){ADDR_UNDEF},ra,(Address){ADDR_UNDEF}); break; }
            case BREAK_NODE: emit(TAC_GOTO,(Address){ADDR_UNDEF},loop_end,(Address){ADDR_UNDEF}); break;
            case CONTINUE_NODE: emit(TAC_GOTO,(Address){ADDR_UNDEF},loop_start,(Address){ADDR_UNDEF}); break;
            case BINARY_OP_NODE: case UNARY_OP_NODE: case CALL_NODE: traverse_expr(current); break;
            default: break;
        }

        if(current->type == BLOCK_NODE || current->type == FOR_NODE) symtab_exit_scope();

        current = current->next;
    }
}

Quad* generate_icg(ASTNode* r) {
    tac_head = tac_tail = NULL;
    temp_counter = 0;
    label_counter = 0;
    traverse_stmt(r, (Address){ADDR_UNDEF}, (Address){ADDR_UNDEF});
    return tac_head;
}