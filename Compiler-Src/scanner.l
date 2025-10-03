%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h" 
int lineno = 1;
%}
%option noyywrap
digit       [0-9]
letter      [A-Za-z]
identifier  {letter}({letter}|{digit}|_)*
exponent    [eE][+-]?{digit}+
%%
[ \t]+      ;
\n          { lineno++; }
"//".* ;
"print"     { return PRINT; }
"let"       { return LET; }
"const"     { return CONST; }
"Bool"      { return TYPE_BOOL; }
"Int"       { return TYPE_INT; }
"Dub"       { return TYPE_DUB; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"for"       { return FOR; }
"break"     { return BREAK; }
"continue"  { return CONTINUE; }
"return"    { return RETURN; }
"true"      { yylval.bool_val = 1; return BOOL_LIT; }
"false"     { yylval.bool_val = 0; return BOOL_LIT; }
\"[^"\n]*\" {
    yytext[strlen(yytext) - 1] = '\0'; 
    yylval.str_val = strdup(yytext + 1);
    return STRING_LITERAL;
}
{digit}+\.{digit}*({exponent})?   { yylval.dub_val = atof(yytext); return DUB_LIT; }
\.{digit}+({exponent})?           { yylval.dub_val = atof(yytext); return DUB_LIT; }
{digit}+{exponent}                { yylval.dub_val = atof(yytext); return DUB_LIT; }
{digit}+    { yylval.int_val = atoi(yytext); return INT_LIT; }
{identifier} { yylval.str_val = strdup(yytext); return IDENTIFIER; }
"+="        { return ADD_ASSIGN; }
"-="        { return SUB_ASSIGN; }
"*="        { return MUL_ASSIGN; }
"/="        { return DIV_ASSIGN; }
"%="        { return MOD_ASSIGN; }
"||"        { return OR; }
"&&"        { return AND; }
"=="        { return EQ; }
"!="        { return NEQ; }
"<="        { return LTE; }
">="        { return GTE; }
"++"        { return INC; }
"--"        { return DEC; }
";"         { return ';'; }
":"         { return ':'; }
","         { return ','; }
"="         { return '='; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
"<"         { return '<'; }
">"         { return '>'; }
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"%"         { return '%'; }
"!"         { return '!'; }
.           { printf("Lexical Error on line %d: Unexpected character '%s'\n", lineno, yytext); }
%%