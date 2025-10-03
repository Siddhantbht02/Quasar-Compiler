#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h" // This include is critical to define SymbolType

// Enum to identify the type of an AST node
typedef enum {
    STMT_LIST_NODE,
    VAR_DECL_NODE,
    CONST_DECL_NODE,
    IF_NODE,
    WHILE_NODE,
    FOR_NODE,
    RETURN_NODE,
    BREAK_NODE,
    CONTINUE_NODE,
    BLOCK_NODE,
    PRINT_NODE,
    BINARY_OP_NODE,
    UNARY_OP_NODE,
    LITERAL_NODE,
    IDENTIFIER_NODE,
    CALL_NODE,
    EXPR_LIST_NODE
} NodeType;

// Enum for literal types
typedef enum {
    LIT_INT,
    LIT_DUB,
    LIT_BOOL
} LiteralType;

// Enum for all operators
typedef enum {
    // Binary Operators
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LTE, OP_GTE,
    OP_AND, OP_OR,
    // Assignment Operators
    OP_ASSIGN, OP_ADD_ASSIGN, OP_SUB_ASSIGN, OP_MUL_ASSIGN, OP_DIV_ASSIGN, OP_MOD_ASSIGN,
    // Unary Operators
    OP_NEG, OP_NOT, OP_POST_INC, OP_POST_DEC
} OperatorType;

// The generic base struct for all AST nodes
typedef struct ASTNode {
    NodeType type;
    struct ASTNode *next;
    SymbolType computed_type;
} ASTNode;

// Specific node structures
typedef struct LiteralNode { ASTNode base; LiteralType literal_type; union { int int_val; double dub_val; int bool_val; } value; } LiteralNode;
typedef struct IdentifierNode { ASTNode base; char *name; } IdentifierNode;
typedef struct BinOpNode { ASTNode base; OperatorType op; ASTNode *left; ASTNode *right; } BinOpNode;
typedef struct UnaryOpNode { ASTNode base; OperatorType op; ASTNode *operand; } UnaryOpNode;
typedef struct VarDeclNode { ASTNode base; char *name; ASTNode *init_expr; } VarDeclNode;
typedef struct IfNode { ASTNode base; ASTNode *condition; ASTNode *if_body; ASTNode *else_body; } IfNode;
typedef struct WhileNode { ASTNode base; ASTNode *condition; ASTNode *body; } WhileNode;
typedef struct ForNode { ASTNode base; ASTNode *init; ASTNode *condition; ASTNode *update; ASTNode *body; } ForNode;
typedef struct CallNode { ASTNode base; ASTNode *function_name; ASTNode *arguments; } CallNode;
typedef struct ReturnNode { ASTNode base; ASTNode *return_expr; } ReturnNode;
typedef struct PrintNode { ASTNode base; char *string_to_print; } PrintNode;

// --- Helper Functions to Create Nodes (Implementations Included) ---

static inline ASTNode* new_bin_op_node(OperatorType op, ASTNode* left, ASTNode* right) {
    BinOpNode* node = (BinOpNode*) malloc(sizeof(BinOpNode));
    node->base.type = BINARY_OP_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->op = op;
    node->left = left;
    node->right = right;
    return (ASTNode*) node;
}

static inline ASTNode* new_unary_op_node(OperatorType op, ASTNode* operand) {
    UnaryOpNode* node = (UnaryOpNode*) malloc(sizeof(UnaryOpNode));
    node->base.type = UNARY_OP_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->op = op;
    node->operand = operand;
    return (ASTNode*) node;
}

static inline ASTNode* new_int_lit_node(int value) {
    LiteralNode* node = (LiteralNode*) malloc(sizeof(LiteralNode));
    node->base.type = LITERAL_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_INT;
    node->literal_type = LIT_INT;
    node->value.int_val = value;
    return (ASTNode*) node;
}

static inline ASTNode* new_dub_lit_node(double value) {
    LiteralNode* node = (LiteralNode*) malloc(sizeof(LiteralNode));
    node->base.type = LITERAL_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_DUB;
    node->literal_type = LIT_DUB;
    node->value.dub_val = value;
    return (ASTNode*) node;
}

static inline ASTNode* new_bool_lit_node(int value) {
    LiteralNode* node = (LiteralNode*) malloc(sizeof(LiteralNode));
    node->base.type = LITERAL_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_BOOL;
    node->literal_type = LIT_BOOL;
    node->value.bool_val = value;
    return (ASTNode*) node;
}

static inline ASTNode* new_identifier_node(char* name) {
    IdentifierNode* node = (IdentifierNode*) malloc(sizeof(IdentifierNode));
    node->base.type = IDENTIFIER_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->name = strdup(name);
    return (ASTNode*) node;
}

static inline ASTNode* new_var_decl_node(char* name, ASTNode* init_expr) {
    VarDeclNode* node = (VarDeclNode*) malloc(sizeof(VarDeclNode));
    node->base.type = VAR_DECL_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->name = strdup(name);
    node->init_expr = init_expr;
    return (ASTNode*) node;
}

static inline ASTNode* new_if_node(ASTNode* condition, ASTNode* if_body, ASTNode* else_body) {
    IfNode* node = (IfNode*) malloc(sizeof(IfNode));
    node->base.type = IF_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->condition = condition;
    node->if_body = if_body;
    node->else_body = else_body;
    return (ASTNode*) node;
}

static inline ASTNode* new_while_node(ASTNode* condition, ASTNode* body) {
    WhileNode* node = (WhileNode*) malloc(sizeof(WhileNode));
    node->base.type = WHILE_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->condition = condition;
    node->body = body;
    return (ASTNode*) node;
}

static inline ASTNode* new_for_node(ASTNode* init, ASTNode* condition, ASTNode* update, ASTNode* body) {
    ForNode* node = (ForNode*) malloc(sizeof(ForNode));
    node->base.type = FOR_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->init = init;
    node->condition = condition;
    node->update = update;
    node->body = body;
    return (ASTNode*) node;
}

static inline ASTNode* new_call_node(ASTNode* name, ASTNode* args) {
    CallNode* node = (CallNode*) malloc(sizeof(CallNode));
    node->base.type = CALL_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->function_name = name;
    node->arguments = args;
    return (ASTNode*) node;
}

static inline ASTNode* new_simple_stmt_node(NodeType type) {
    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));
    node->type = type;
    node->next = NULL;
    node->computed_type = SYM_UNDEFINED;
    return (ASTNode*) node;
}

static inline ASTNode* new_return_node(ASTNode* expr) {
    ReturnNode* node = (ReturnNode*) malloc(sizeof(ReturnNode));
    node->base.type = RETURN_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->return_expr = expr;
    return (ASTNode*) node;
}

static inline ASTNode* new_print_node(char* str) {
    PrintNode* node = (PrintNode*) malloc(sizeof(PrintNode));
    node->base.type = PRINT_NODE;
    node->base.next = NULL;
    node->base.computed_type = SYM_UNDEFINED;
    node->string_to_print = str;
    return (ASTNode*) node;
}

#endif // AST_H