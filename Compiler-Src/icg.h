#ifndef ICG_H
#define ICG_H

#include "ast.h" // <-- THIS INCLUDE IS CRITICAL

typedef enum {
    TAC_UNDEF, TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV, TAC_MOD, TAC_EQ, TAC_NEQ, TAC_LT, TAC_GT, TAC_LTE, TAC_GTE,
    TAC_AND, TAC_OR, TAC_NOT, TAC_NEG, TAC_ASSIGN, TAC_LABEL, TAC_GOTO, TAC_IFZ, TAC_RETURN, TAC_CALL, TAC_PRINT_STR
} TacOpcode;

typedef enum {
    ADDR_UNDEF, ADDR_VARIABLE, ADDR_CONSTANT, ADDR_TEMP, ADDR_LABEL, ADDR_STRING_LITERAL
} AddressType;

typedef struct Address {
    AddressType type;
    union {char *variable_name; int int_constant; double dub_constant; int temp_id; int label_id;} value;
} Address;

typedef struct Quad {
    TacOpcode op;
    Address result, arg1, arg2;
    struct Quad *next;
} Quad;

Quad* generate_icg(ASTNode* ast_root);
void print_icg(Quad* tac_head);

#endif // ICG_H