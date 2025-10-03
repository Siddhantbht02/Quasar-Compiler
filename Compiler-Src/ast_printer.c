#include <stdio.h>
#include "ast_printer.h"
static void print_indent(int l){for(int i=0;i<l;i++)printf("  ");}
static void print_node(ASTNode* n, int l) {
    if(!n)return; print_indent(l);
    switch(n->type){
        case VAR_DECL_NODE:{VarDeclNode*d=(VarDeclNode*)n;printf("VAR_DECL (Name: %s)\n",d->name);if(d->init_expr)print_node(d->init_expr,l+1);break;}
        case BINARY_OP_NODE:{BinOpNode*o=(BinOpNode*)n;printf("BINARY_OP (Op: %d)\n",o->op);print_node(o->left,l+1);print_node(o->right,l+1);break;}
        case UNARY_OP_NODE:{UnaryOpNode*o=(UnaryOpNode*)n;printf("UNARY_OP (Op: %d)\n",o->op);print_node(o->operand,l+1);break;}
        case LITERAL_NODE:{LiteralNode*li=(LiteralNode*)n;if(li->literal_type==LIT_INT)printf("INT_LITERAL (Value: %d)\n",li->value.int_val);else if(li->literal_type==LIT_DUB)printf("DUB_LITERAL (Value: %f)\n",li->value.dub_val);else if(li->literal_type==LIT_BOOL)printf("BOOL_LITERAL (Value: %s)\n",li->value.bool_val?"true":"false");break;}
        case IDENTIFIER_NODE:{IdentifierNode*i=(IdentifierNode*)n;printf("IDENTIFIER (Name: %s)\n",i->name);break;}
        case IF_NODE:{IfNode*i=(IfNode*)n;printf("IF_STMT\n");print_indent(l+1);printf("CONDITION:\n");print_node(i->condition,l+2);print_indent(l+1);printf("IF_BODY:\n");print_node(i->if_body,l+2);if(i->else_body){print_indent(l+1);printf("ELSE_BODY:\n");print_node(i->else_body,l+2);}break;}
        case FOR_NODE:{ForNode*f=(ForNode*)n;printf("FOR_STMT\n");print_indent(l+1);printf("INIT:\n");print_node(f->init,l+2);print_indent(l+1);printf("CONDITION:\n");print_node(f->condition,l+2);print_indent(l+1);printf("UPDATE:\n");print_node(f->update,l+2);print_indent(l+1);printf("BODY:\n");print_node(f->body,l+2);break;}
        case RETURN_NODE:{ReturnNode*r=(ReturnNode*)n;printf("RETURN_STMT\n");if(r->return_expr)print_node(r->return_expr,l+1);break;}
        case PRINT_NODE:{PrintNode*p=(PrintNode*)n;printf("PRINT_STMT (Value: \"%s\")\n",p->string_to_print);break;}
        case CONTINUE_NODE:{printf("CONTINUE_STMT\n");break;}
        case BREAK_NODE:{printf("BREAK_STMT\n");break;}
        default:printf("Unknown Node Type: %d\n",n->type);break;
    }
    print_node(n->next,l);
}
void print_ast(ASTNode* ast_root){printf("\n--- Abstract Syntax Tree ---\n");print_node(ast_root,0);printf("--------------------------\n");}