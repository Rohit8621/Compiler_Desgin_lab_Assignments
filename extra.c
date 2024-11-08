%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    void yyerror(const char *s);
    int yylex(void);
    int yywrap();
    int flag = 0;

    extern FILE *yyin;

%}

%union {
    int intval;
    float fltval;
    char *str;
}

%token <str> ID IF ELSE WHILE INT_TYPE FLOAT_TYPE INCLUDE
%token <intval> INT
%token <fltval> FLOAT
%token GT LT GE LE EQ NE

%left '+' '-'
%left '*' '/' '%'

%nonassoc UMINUS
%token PLUS MINUS STAR DIV LBRACE RBRACE LPAREN RPAREN EQUAL SEMI

%%

Program : GlobalStmts {
            printf("\n---INPUT CODE---\n");
            system("cat q.c");
            printf("\n---SYNTAX CHECK---\n");
            printf("\nSyntactically Correct.\n");
        }
        ;

GlobalStmts : GlobalStmt GlobalStmts
        | GlobalStmt
        ;

GlobalStmt : INCLUDE                
        | Decl SEMI
        | WhileStmt
        | IfStmt
        ;

Block   : LBRACE Stmts RBRACE
        ;

Stmts   : Stmt Stmts
        | Stmt
        ;

Stmt    : Decl SEMI
        | Assign SEMI
        | WhileStmt
        | IfStmt
        ;

Decl    : Type ID
        | Type Assign
        ;

Type    : INT_TYPE
        | FLOAT_TYPE
        ;

WhileStmt : WHILE LPAREN Condition RPAREN Block
        ;

IfStmt  : IF LPAREN Condition RPAREN Block ELSE Block
        | IF LPAREN Condition RPAREN Block
        ;

Condition : Expr GT Expr
        | Expr LT Expr
        | Expr
        ;

Assign  : ID EQUAL Expr
        ;

Expr    : Expr PLUS Term
        | Expr MINUS Term
        | Term
        ;

Term    : Term STAR Factor
        | Term DIV Factor
        | Term '%' Factor
        | Factor
        ;

Factor  : INT
        | FLOAT
        | ID
        | LPAREN Expr RPAREN
        | MINUS Factor %prec UMINUS
        ;

%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
    flag = 1;
}

int yywrap() {
    return 1;
}

int main(void) {
    FILE *file = fopen("q.c", "r");
    if (!file) {
        printf("Error opening file\n");
        return 1;
    }
    yyin = file;

    printf("\n----------LEXICAL ANALYSIS----------\n\n");
    printf("Token - Type\n");
    printf("--------------------\n");
    yyparse();

    if (flag) {
        printf("\nSyntactically wrong\n");
    }

    fclose(file);
    return 0;
}


//or code 2
//lexical

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
%}

%%
"#include"[ ]"<"[^>]">"    { printf("%s - preprocessor directive\n", yytext); return INCLUDE; }
"int"                        { printf("int - keyword\n"); return INT_TYPE; }
"float"                      { printf("float - keyword\n"); return FLOAT_TYPE; }
"if"                         { printf("if - keyword\n"); return IF; }
"else"                       { printf("else - keyword\n"); return ELSE; }
"while"                      { printf("while - keyword\n"); return WHILE; }
[a-zA-Z_][a-zA-Z0-9_]*      { printf("%s - identifier\n", yytext); yylval.str = strdup(yytext); return ID; }
[0-9]+                       { printf("%s - integer constant\n", yytext); yylval.intval = atoi(yytext); return INT; }
[0-9]+\.[0-9]+              { printf("%s - float constant\n", yytext); yylval.fltval = atof(yytext); return FLOAT; }
">"                          { printf("> - relational operator\n"); return GT; }
"<"                          { printf("< - relational operator\n"); return LT; }
">="                         { printf(">= - relational operator\n"); return GE; }
"<="                         { printf("<= - relational operator\n"); return LE; }
"=="                         { printf("== - relational operator\n"); return EQ; }
"!="                         { printf("!= - relational operator\n"); return NE; }
"="                          { printf("= - assignment operator\n"); return EQUAL; }
"{"                          { printf("{ - special character\n"); return LBRACE; }
"}"                          { printf("} - special character\n"); return RBRACE; }
"("                          { printf("( - special character\n"); return LPAREN; }
")"                          { printf(") - special character\n"); return RPAREN; }
";"                          { printf("; - special character\n"); return SEMI; }
","                          { printf(", - special character\n"); return yytext[0]; }
"//".*                         { printf("%s - multiline comment\n",yytext);}
"+"                          { printf("+ - Arithmetic Operator\n"); return PLUS; }
"-"                          { printf("- - Arithmetic Operator\n"); return MINUS; }
""                          { printf(" - Arithmetic Operator\n"); return STAR; }
"/"                          { printf("/ - Arithmetic Operator\n"); return DIV; }
[ \t\n]                     { ; }
.                           { printf("%s - special character\n", yytext); return yytext[0]; }
%%


// run command for every code
// bison -d parser.y
// flex lexer.l
// gcc parser.tab.c lex.yy.c -o final -w -ll
// ./final