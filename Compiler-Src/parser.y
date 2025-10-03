%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "ast_printer.h"
#include "icg.h"
#include "codegen.h"
int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
extern int lineno;
ASTNode *ast_root;
%}
%code requires {#include "ast.h"}
%union {int int_val; double dub_val; int bool_val; char *str_val; ASTNode *node;}
%token <int_val> INT_LIT
%token <dub_val> DUB_LIT
%token <bool_val> BOOL_LIT
%token <str_val> IDENTIFIER STRING_LITERAL
%token LET CONST IF WHILE FOR BREAK CONTINUE RETURN PRINT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token OR AND EQ NEQ LTE GTE INC DEC ELSE TYPE_BOOL TYPE_INT TYPE_DUB
%type <node> program decl_or_stmt_list decl_or_stmt stmt block var_decl const_decl
%type <node> if_stmt while_stmt for_stmt return_stmt break_stmt continue_stmt expr_stmt print_stmt
%type <node> expr assignment logical_or logical_and equality relational additive multiplicative unary postfix primary call
%type <node> for_init_opt for_cond_opt for_update_opt expr_list optional_expr_list optional_init
%right '=' ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%left OR %left AND %left EQ NEQ %left '<' '>' LTE GTE %left '+' '-' %left '*' '/' '%'
%right '!' UMINUS %left INC DEC %nonassoc IFX %nonassoc ELSE
%start program
%%
program: decl_or_stmt_list { ast_root = $1; printf("Parse successful! AST created.\n"); };
decl_or_stmt_list: {$$=NULL;} | decl_or_stmt_list decl_or_stmt { ASTNode *h=$1; if(!h){$$=$2;}else{ASTNode *c=h; while(c->next){c=c->next;} c->next=$2; $$=h;} };
decl_or_stmt: var_decl {$$=$1;} | const_decl {$$=$1;} | stmt {$$=$1;};
var_decl: LET IDENTIFIER optional_type optional_init ';' { $$ = new_var_decl_node($2, $4); };
const_decl: CONST IDENTIFIER optional_type '=' expr ';' { $$ = new_var_decl_node($2, $5); };
optional_type: | ':' type; type: TYPE_BOOL | TYPE_INT | TYPE_DUB;
optional_init: {$$=NULL;} | '=' expr {$$=$2;};
stmt: expr_stmt {$$=$1;} | block {$$=$1;} | if_stmt {$$=$1;} | while_stmt {$$=$1;} | for_stmt {$$=$1;} | return_stmt {$$=$1;} | break_stmt {$$=$1;} | continue_stmt {$$=$1;} | print_stmt {$$=$1;} | empty_stmt {$$=NULL;};
print_stmt: PRINT STRING_LITERAL ';' { $$ = new_print_node($2); };
expr_stmt: expr ';' {$$=$1;}; empty_stmt: ';';
block: '{' decl_or_stmt_list '}' {$$=$2;};
if_stmt: IF '(' expr ')' stmt %prec IFX {$$=new_if_node($3,$5,NULL);} | IF '(' expr ')' stmt ELSE stmt {$$=new_if_node($3,$5,$7);};
while_stmt: WHILE '(' expr ')' stmt {$$=new_while_node($3,$5);};
for_stmt: FOR '(' for_init_opt ';' for_cond_opt ';' for_update_opt ')' stmt {$$=new_for_node($3,$5,$7,$9);};
for_init_opt: {$$=NULL;} | expr_list {$$=$1;} | LET IDENTIFIER optional_type optional_init {$$=new_var_decl_node($2,$4);};
for_cond_opt: {$$=NULL;} | expr {$$=$1;}; for_update_opt: {$$=NULL;} | expr_list {$$=$1;};
return_stmt: RETURN ';' {$$=new_return_node(NULL);} | RETURN expr ';' {$$=new_return_node($2);};
break_stmt: BREAK ';' {$$=new_simple_stmt_node(BREAK_NODE);};
continue_stmt: CONTINUE ';' {$$=new_simple_stmt_node(CONTINUE_NODE);};
expr_list: expr {$$=$1;} | expr_list ',' expr {$1->next=$3; $$=$1;};
expr: assignment {$$=$1;};
assignment: logical_or {$$=$1;} | logical_or '=' assignment {$$=new_bin_op_node(OP_ASSIGN,$1,$3);} | logical_or ADD_ASSIGN assignment {$$=new_bin_op_node(OP_ADD_ASSIGN,$1,$3);} | logical_or SUB_ASSIGN assignment {$$=new_bin_op_node(OP_SUB_ASSIGN,$1,$3);} | logical_or MUL_ASSIGN assignment {$$=new_bin_op_node(OP_MUL_ASSIGN,$1,$3);} | logical_or DIV_ASSIGN assignment {$$=new_bin_op_node(OP_DIV_ASSIGN,$1,$3);} | logical_or MOD_ASSIGN assignment {$$=new_bin_op_node(OP_MOD_ASSIGN,$1,$3);};
logical_or: logical_and {$$=$1;} | logical_or OR logical_and {$$=new_bin_op_node(OP_OR,$1,$3);};
logical_and: equality {$$=$1;} | logical_and AND equality {$$=new_bin_op_node(OP_AND,$1,$3);};
equality: relational {$$=$1;} | equality EQ relational {$$=new_bin_op_node(OP_EQ,$1,$3);} | equality NEQ relational {$$=new_bin_op_node(OP_NEQ,$1,$3);};
relational: additive {$$=$1;} | relational '<' additive {$$=new_bin_op_node(OP_LT,$1,$3);} | relational '>' additive {$$=new_bin_op_node(OP_GT,$1,$3);} | relational LTE additive {$$=new_bin_op_node(OP_LTE,$1,$3);} | relational GTE additive {$$=new_bin_op_node(OP_GTE,$1,$3);};
additive: multiplicative {$$=$1;} | additive '+' multiplicative {$$=new_bin_op_node(OP_ADD,$1,$3);} | additive '-' multiplicative {$$=new_bin_op_node(OP_SUB,$1,$3);};
multiplicative: unary {$$=$1;} | multiplicative '*' unary {$$=new_bin_op_node(OP_MUL,$1,$3);} | multiplicative '/' unary {$$=new_bin_op_node(OP_DIV,$1,$3);} | multiplicative '%' unary {$$=new_bin_op_node(OP_MOD,$1,$3);};
unary: postfix {$$=$1;} | '-' unary %prec UMINUS {$$=new_unary_op_node(OP_NEG,$2);} | '!' unary {$$=new_unary_op_node(OP_NOT,$2);};
postfix: primary {$$=$1;} | postfix INC {$$=new_unary_op_node(OP_POST_INC,$1);} | postfix DEC {$$=new_unary_op_node(OP_POST_DEC,$1);};
primary: IDENTIFIER {$$=new_identifier_node($1);} | INT_LIT {$$=new_int_lit_node($1);} | DUB_LIT {$$=new_dub_lit_node($1);} | BOOL_LIT {$$=new_bool_lit_node($1);} | '(' expr ')' {$$=$2;} | call {$$=$1;};
call: IDENTIFIER '(' optional_expr_list ')' {$$=new_call_node(new_identifier_node($1),$3);};
optional_expr_list: {$$=NULL;} | expr_list {$$=$1;};
%%
void yyerror(const char *s){fprintf(stderr,"Syntax Error on line %d: %s\n",lineno,s);}
int main(void){
    FILE *file=fopen("input.lang","r");
    if(!file){printf("Error: Could not open input file 'input.lang'.\n");return 1;}
    yyin=file;
    yyparse();
    if(ast_root){
        check_semantics(ast_root);
        Quad* tac_list=generate_icg(ast_root);
        // print_icg(tac_list); // Optional: uncomment to see TAC
        generate_assembly(tac_list);
    }
    fclose(file);
    return 0;
}