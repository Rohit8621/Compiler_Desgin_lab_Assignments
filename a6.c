// Develop an intermediate code generator to generate three address code for the following statements by writing suitable syntax directed translation rules 
// 1. Assignment statements 
// 2. Boolean expressions

// Aim
//       To Develop an intermediate code generator to generate three address code for the Assignment statements Boolean expressions.

// Code
// Input.txt
// a := b * -c + b* -c

// Intermediate.y
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int temp_count = 0;   // Counter for temporary variables
int label_count = 100; // Counter for labels

int new_temp() { return temp_count++; }
int new_label() { return label_count++; }

void emit(const char* op, const char* arg1, const char* arg2, const char* result) {
    if (strcmp(op, "=") == 0) {
        printf("%s = %s\n", result, arg1);
    } else if (arg2) {
        printf("%s = %s %s %s\n", result, arg1, op, arg2);
    } else {
        printf("%s = %s %s\n", result, op, arg1);
    }
}

void emit_if_goto(const char* op, const char* arg1, const char* arg2, const char* label) {
    printf("if %s %s %s goto %s\n", arg1, op, arg2, label);
}

void emit_label(const char* label) { printf("%s:\n", label); }
void emit_goto(const char* label) { printf("goto %s\n", label); }

void yyerror(const char* s) {  }
int yylex();
%}

%union { int num; char* id; }
%token <id> ID
%token <num> NUM
%token PLUS MINUS MULT DIV ASSIGN LT AND OR
%token LPAREN RPAREN SEMICOLON
%type <id> expr assignment boolean_expr
%left OR
%left AND
%nonassoc LT
%left PLUS MINUS
%left MULT DIV
%right ASSIGN UMINUS

%% 

stmt_list:
    stmt_list stmt SEMICOLON | stmt SEMICOLON ;

stmt:
    assignment | boolean_expr ;

assignment:
    ID ASSIGN expr { emit("=", $3, NULL, $1); } ;

expr:
    expr PLUS expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("+", $1, $3, temp); $$ = strdup(temp); }
    | expr MINUS expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("-", $1, $3, temp); $$ = strdup(temp); }
    | expr MULT expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("*", $1, $3, temp); $$ = strdup(temp); }
    | expr DIV expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("/", $1, $3, temp); $$ = strdup(temp); }
    | MINUS expr %prec UMINUS { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("-", $2, NULL, temp); $$ = strdup(temp); }
    | ID { $$ = strdup($1); }
    | NUM { char temp[10]; sprintf(temp, "%d", $1); $$ = strdup(temp); }
    | LPAREN expr RPAREN { $$ = $2; }
    ;


boolean_expr:
    expr LT expr { char label_true[10], label_end[10]; sprintf(label_true, "L%d", new_label()); sprintf(label_end, "L%d", new_label()); emit_if_goto("<", $1, $3, label_true); emit_goto(label_end); emit_label(label_true); printf("1\n"); emit_label(label_end); }
    | expr OR expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("or", $1, $3, temp); $$ = strdup(temp); }
    | expr AND expr { int t = new_temp(); char temp[10]; sprintf(temp, "t%d", t); emit("and", $1, $3, temp); $$ = strdup(temp); }
    ;

%% 

int main() {
    return yyparse();
}

// Intermediate.l
%{
#include "intermediate.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%% 
"and"     { return AND; }
"or"      { return OR; }
":="      { return ASSIGN; }
"+"       { return PLUS; }
"-"       { return MINUS; }
"*"       { return MULT; }
"/"       { return DIV; }
"<"       { return LT; }
"("       { return LPAREN; }
")"       { return RPAREN; }
";"       { return SEMICOLON; }
[0-9]+               { yylval.num = atoi(yytext); return NUM; }
[a-zA-Z][a-zA-Z0-9]* { yylval.id = strdup(yytext); return ID; }
[ \t\n]+  { /* ignore whitespace */ }
.         { printf("Unknown character: %s\n", yytext); }




%% 

int yywrap() {
    return 1;
}


//run command 
/*
bison -d intermediate.y
flex intermediate.l
intermediate < input.txt
gcc -o intermediate lex.yy.c intermediate.tab.c
*/