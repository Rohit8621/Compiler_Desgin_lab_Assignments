// CODE (The Scanner): 
%{ 
#include <stdio.h> 
#include <string.h> 
int lookup_keyword(char *s); 
%} 
digit 
letter 
id 
int_const 
keyword 
[0-9] 
[A-Za-z_] 
{letter}({letter}|{digit})* 
{digit}+ 
(auto|break|case|char|const|continue|default|do|double|else|enum|exter 
n|float|for|goto|if|int|long|register|return|short|signed|sizeof|stati 
c|struct|switch|typedef|union|unsigned|void|volatile|while) 
%% 
{keyword} 
{id} 
{int_const} 
{ printf("keyword: %s\n", yytext); } 
{ printf("identifier: %s\n", yytext); } 
{ printf("integer constant: %s\n", yytext); } 
"=="|"!="|"<="|">="|"<"|">" { printf("relational operator: %s\n", 
yytext); } 
"+"|"-"|"*"|"/"|"%" { printf("arithmetic operator: %s\n", yytext); } 
"+="|"-="|"*="|"/="|"%=" { printf("arithmetic assignment operator: 
%s\n", yytext); } 
"&&"|"||"|"!" { printf("logical operator: %s\n", yytext); } 
"&"|"|"|"^"|"<<"|">>" { printf("bitwise operator: %s\n", yytext); } 
"++"|"--"|"=" { printf("assignment operator: %s\n", yytext); } 
";"|","|"."|"["|"]"|"("|")"|"{"|"}" { printf("special character: 
%s\n", yytext); } 
"//".*  
{ /* skip single-line comments */ } 
"/*"([^*]|\*+([^*/]))*"*/" { /* skip multi-line comments */ } 
[ \t\n]+ 
{ /* skip whitespace */ } 
. 
%% 
{ printf("other: %s\n", yytext); } 
int lookup_keyword(char *s) { 
static const char *keywords[] = { 
"auto", "break", "case", "char", "const", "continue", 
"default", "do", "double", 
"else", "enum", "extern", "float", "for", "goto", "if", "int", 
"long", "register", 
"return", "short", "signed", "sizeof", "static", "struct", 
"switch", "typedef", 
"union", "unsigned", "void", "volatile", "while", NULL 
}; 
for (int i = 0; keywords[i] != NULL; i++) { 
if (strcmp(s, keywords[i]) == 0) 
return 1; 
} 
} 
return 0; 
int main() { 
yylex(); 
return 0; 
} 
Input program to be scanned (filename given as command line argument) 
#include<stdio. 
h> main() 
{ 
int a=10,b=20; 
if(a>b) 
printf("a is 
greater"); else 
printf("b is greater");

//run
// flex prog.l
// gcc lex.yy.c -_lock_filea.exe prog.l



//2 code Or

// Aim
//         To implement the Lexical Analyzer using the tool LEX

// Code
// Example input source program 
// greater.c
#include<stdio.h>
//#include <stdlib.h>
/*
#include <ctype.h>
#include <string.h>
*/

int main()
{
    int a=10,b=20;
    float c=10.00;
    double d=20.0000;

    if(a>b)
        printf("a is greater");
    else
        printf("b is greater");
}

Scanner.c

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char identifier_name[100];
    char type[20];
    int num_bytes;
    int address;
    char value[20];
} SymbolTable;

SymbolTable symtab[100];
int symtab_count = 0;

char previous[20];

void add_to_symbol_table(char *name, char *type, int bytes) {
    int already = 0;
    for (int i = 0; i < symtab_count; ++i) {
        if (strcmp(symtab[i].identifier_name, name) == 0) {
            already = 1;
            break;
        }
    }
    
    if (!already) {
        strcpy(symtab[symtab_count].identifier_name, name);
        strcpy(symtab[symtab_count].type, type);
        
        if (symtab_count > 0) {
            if (strcmp(type, "int") == 0) {
                symtab[symtab_count].address = symtab[symtab_count - 1].address + 2;
                symtab[symtab_count].num_bytes=2;
            } else if (strcmp(type, "float") == 0) {
                symtab[symtab_count].address = symtab[symtab_count - 1].address + 4;
                symtab[symtab_count].num_bytes=4;
            } else if (strcmp(type, "double") == 0) {
                symtab[symtab_count].address = symtab[symtab_count - 1].address + 8;
                symtab[symtab_count].num_bytes=8;
            } else if (strcmp(type, "long") == 0) {
                symtab[symtab_count].address = symtab[symtab_count - 1].address + 16;
                symtab[symtab_count].num_bytes=16;
            }
        } else {
            symtab[symtab_count].address = 1000;

            if (strcmp(type, "int") == 0) {
                symtab[symtab_count].num_bytes=2;
            } else if (strcmp(type, "float") == 0) {
                symtab[symtab_count].num_bytes=4;
            } else if (strcmp(type, "double") == 0) {
                symtab[symtab_count].num_bytes=8;
            } else if (strcmp(type, "long") == 0) {
                symtab[symtab_count].num_bytes=16;
            } 
        }
        symtab_count++;
    }
}

void print_symbol_table() {
    printf("\n\nContent of Symbol Table\n\n");
    printf(" ----------------------------------------------------- \n");
    printf("| %-5s | %-10s | %-6s | %-8s | %-10s |\n", "Name", "Type", "Bytes", "Address","Value");
    printf(" ----------------------------------------------------- \n");
    for (int i = 0; i < symtab_count; i++) {
        printf("| %-5s | %-10s | %-6d | %-8d | %-10s |\n", symtab[i].identifier_name, symtab[i].type, symtab[i].num_bytes, symtab[i].address,symtab[i].value);
        printf(" ----------------------------------------------------- \n");
    }
}

int yywrap(void) {
    return 1;
}
%}

%%

^#[ \t]*include[ \t]*<[a-z]+\.[h]+> { printf("%s - preprocessor directive\n", yytext); }
(main|printf)\([^)]*\)                            { printf("%-10s - function call\n", yytext); }

"auto"|"break"|"case"|"char"|"const"|"continue"|"default"|"do"|"double"|"else"|"enum"|"extern"|"float"|"for"|"goto"|"if"|"int"|"long"|"register"|"return"|"short"|"signed"|"sizeof"|"static"|"struct"|"switch"|"typedef"|"union"|"unsigned"|"void"|"volatile"|"while"   { strcpy(previous,yytext);printf("%-10s - keyword\n", yytext); }

"{"|"}"|";"|","|"("|")"            { printf("%-10s - special character\n", yytext); }

[a-zA-Z_][a-zA-Z]*          { add_to_symbol_table(yytext, previous, 2); printf("%-10s - identifier\n", yytext);}

[0-9]+\.[0-9]*                  { strcpy(symtab[symtab_count-1].value,yytext); printf("%-10s - %s              \n", yytext,previous); }
[0-9]+                                 { strcpy(symtab[symtab_count-1].value,yytext); printf("%-10s - integer constant\n", yytext); }

"//".*                                    { printf("%-10s - Single Line Comment\n", yytext); }
"/*"[^*]*"*"*[^/]*"*/"            { printf("%s - Multi Line Comment\n", yytext); }

\"[^\"\n]*\"                               { printf("%s - string constant\n", yytext); }

"+"|"-"|"*"|"/"|"%"                   { printf("%-10s - arithmetic operator\n", yytext); }
"+="|"-="|"*="|"/="|"%="      { printf("%-10s - arithmetic assignment operator\n", yytext); }
"&&"|"||"|"!"                               { printf("%-10s - logical operator\n", yytext); }
"<="|">="|"=="|"!="|"<"|">" { printf("%-10s - relational operator\n", yytext); }
"^"|"&"|"|"|"<<"|">>"             { printf("%-10s - bitwise operator\n", yytext); }
"++"|"--"                                     { printf("%-10s - unary operator\n", yytext); }
"="                                               { printf("%-10s - assignment operator\n", yytext); }

" "                                                {}    
"\n"                                            {}         

%%

int main(int argc, char **argv) {
    if(argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("File not found.\n");
            return -1;
        }
        yyin = file;
    }
    yylex();
    print_symbol_table();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

