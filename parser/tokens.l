/*** Definition section ***/
%{
/* C code copied verbatim */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "symbols.h"
int yycolumn = 1;
#define YY_USER_ACTION  yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1;\
    yycolumn += yyleng;
%}

/* Tells flex to read only one input file */
%option noyywrap
%option bison-bridge bison-locations
%option yylineno
DIGIT   [0-9]
ID      [a-z][a-z0-9]*
%%
    /*** Rules section **/
{DIGIT}+    {
                /* yytext is a string containing matched text */
                printf("Saw an integer: %s\n", yytext);
                yylval.dval = atof(yytext);
                return NUMBER;
            }
{DIGIT}+\.{DIGIT}*  {
                        printf("Saw a floating point: %s\n", yytext);
                        yylval.dval = atof(yytext);
                        return NUMBER;
                    }
{ID}        {
                /* code */
                struct GPSymRec *sp = gp_symbol_get(yytext);
                printf("Saw a name: %s\n", yytext);
                yylval.symp = sp;
                /*
                if (sp == NULL) {
                    error("Undefined symbol");
                    yyterminate();
                }
                */
                return NAME;
            }
[\t ]+  /* eat up whitespace */
.|\n   { return yytext[0]; }
%%

