// Write Lex program to recognize relevant tokens required for the Yacc parser to implement desk calculator. Write the Grammar for the expression involving the operators namely, + , - ,* , / , ^ , ( , ). Precedence and associativity has to be preserved. 
// Verify your calculator with the following inputs 
// 1. 3+9 
// 2. 3+9*6 
// 3. (3+4)*7 
// 4. (3-4)+(7*6)
// 5. 5/7+2 
// 6. 4^2^1 
// 7. (2^3)^2

// Aim
// Write Lex program to recognize relevant tokens required for the Yacc parser to implement desk calculator

// Code
// Calc.l

%{
#include "calc.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%% 
[0-9]+     { yylval = atoi(yytext); return NUMBER; }
"+"        { return '+'; }
"-"        { return '-'; }
"*"        { return '*'; }
"/"        { return '/'; }
"^"        { return '^'; }
"("        { return '('; }
")"        { return ')'; }
[ \t\n]+   { /* ignore whitespace */ }
.          { printf("Unknown character: '%s'\n", yytext); }

%%

int yywrap() {
    return 1;
}

// Calc.y

%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right '^'

%%
// Grammar rules
expr: expr '+' expr { $$ = $1 + $3; printf("Result: %d\n", $$); }
     | expr '-' expr { $$ = $1 - $3; printf("Result: %d\n", $$); }
     | expr '*' expr { $$ = $1 * $3; printf("Result: %d\n", $$); }
     | expr '/' expr { 
           if ($3 == 0) {
               yyerror("Error: Division by zero!");
               $$ = 0; // Return zero in case of error
           } else {
               $$ = $1 / $3; 
               printf("Result: %d\n", $$);
           }
         }
     | expr '^' expr  { $$ = pow($1, $3); printf("Result: %d\n", $$); }
     | '(' expr ')'   { $$ = $2; }
     | NUMBER         { $$ = $1; }
     ;
%% 
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    printf("Enter expression:\n");
    while (1) {
        yyparse();
    }
    return 0;
}

// run commands
/*
bison -d calc.y
flex calc.l
gcc -o calc calc.tab.c lex.yy.c -lfl -lm

./calc
*/