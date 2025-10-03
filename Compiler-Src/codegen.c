#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

#define MAX_SYMBOLS 512
typedef struct {char* names[MAX_SYMBOLS];int count;} SymbolList;

void add_symbol_if_new(SymbolList* l,char* n){for(int i=0;i<l->count;i++){if(strcmp(l->names[i],n)==0)return;}if(l->count<MAX_SYMBOLS){l->names[l->count++]=strdup(n);}}
void get_addr_str(Address a,char* b){switch(a.type){case ADDR_VARIABLE:sprintf(b,"%s",a.value.variable_name);break;case ADDR_CONSTANT:sprintf(b,"$%d",a.value.int_constant);break;case ADDR_TEMP:sprintf(b,"t%d",a.value.temp_id);break;case ADDR_LABEL:sprintf(b,"L%d",a.value.label_id);break;default:b[0]='\0';break;}}

void generate_assembly(Quad* h) {
    FILE*f=fopen("output.s","w");if(!f){perror("Could not open output.s");return;}
    SymbolList variables={.count=0},strings={.count=0};char tmp[20];
    Quad*c=h;
    while(c){
        if(c->arg1.type==ADDR_STRING_LITERAL)add_symbol_if_new(&strings,c->arg1.value.variable_name);
        if(c->result.type==ADDR_VARIABLE)add_symbol_if_new(&variables,c->result.value.variable_name);
        if(c->arg1.type==ADDR_VARIABLE)add_symbol_if_new(&variables,c->arg1.value.variable_name);
        if(c->arg2.type==ADDR_VARIABLE)add_symbol_if_new(&variables,c->arg2.value.variable_name);
        if(c->result.type==ADDR_TEMP){sprintf(tmp,"t%d",c->result.value.temp_id);add_symbol_if_new(&variables,tmp);}
        if(c->arg1.type==ADDR_TEMP){sprintf(tmp,"t%d",c->arg1.value.temp_id);add_symbol_if_new(&variables,tmp);}
        if(c->arg2.type==ADDR_TEMP){sprintf(tmp,"t%d",c->arg2.value.temp_id);add_symbol_if_new(&variables,tmp);}
        c=c->next;
    }
    if(strings.count>0){fprintf(f,".section .rodata\n");for(int i=0;i<strings.count;i++){fprintf(f,".LC%d:\n    .string \"%s\"\n",i,strings.names[i]);}}
    if(variables.count>0){fprintf(f,"\n.bss\n");for(int i=0;i<variables.count;i++){fprintf(f,"    .comm %s, 4, 4\n",variables.names[i]);}}
    fprintf(f,"\n.text\n.globl _my_program\n_my_program:\n    pushl %%ebp\n    movl %%esp, %%ebp\n    andl $-16, %%esp\n");
    c=h;char res[50],a1[50],a2[50];
    while(c){
        get_addr_str(c->result,res);get_addr_str(c->arg1,a1);get_addr_str(c->arg2,a2);
        switch(c->op){
            case TAC_ASSIGN:fprintf(f,"    movl %s, %%eax\n    movl %%eax, %s\n",a1,res);break;
            case TAC_ADD:fprintf(f,"    movl %s, %%eax\n    addl %s, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_SUB:fprintf(f,"    movl %s, %%eax\n    subl %s, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_MUL:fprintf(f,"    movl %s, %%eax\n    imull %s, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_MOD:fprintf(f,"    movl %s, %%eax\n    movl %s, %%ebx\n    cdq\n    idivl %%ebx\n    movl %%edx, %s\n",a1,a2,res);break;
            case TAC_EQ:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    sete %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_NEQ:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    setne %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_LT:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    setl %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_GT:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    setg %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_LTE:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    setle %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_GTE:fprintf(f,"    movl %s, %%eax\n    cmpl %s, %%eax\n    setge %%al\n    movzbl %%al, %%eax\n    movl %%eax, %s\n",a1,a2,res);break;
            case TAC_LABEL:fprintf(f,"%s:\n",a1);break;
            case TAC_GOTO:fprintf(f,"    jmp %s\n",a1);break;
            case TAC_IFZ:fprintf(f,"    movl %s, %%eax\n    testl %%eax, %%eax\n    jz %s\n",a1,a2);break;
            case TAC_RETURN:fprintf(f,"    movl %s, %%eax\n",a1);goto end_function;
            case TAC_PRINT_STR:{
                char* l=NULL;
                for(int i=0;i<strings.count;i++){if(strcmp(strings.names[i],c->arg1.value.variable_name)==0){l=malloc(10);sprintf(l,".LC%d",i);break;}}
                if(l){
                    fprintf(f,"    pushl $%s\n",l);
                    fprintf(f,"    call _display\n"); // Use _display to match the C compiler
                    fprintf(f,"    addl $4, %%esp\n",l);
                    free(l);
                }
                break;
            }
            default:break;
        }
        c=c->next;
    }

end_function:
    fprintf(f,"    leave\n");
    fprintf(f,"    ret\n");

    for(int i=0;i<variables.count;i++){free(variables.names[i]);}
    for(int i=0;i<strings.count;i++){free(strings.names[i]);}
    fclose(f);
    printf("Assembly code generated in output.s\n");
}