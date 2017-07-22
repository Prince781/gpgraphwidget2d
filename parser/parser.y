%{
/** Prologue **/
/* C code */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gp-expression.h"
//extern FILE *yyin;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yycolumn, yylineno;
extern int yylex();
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
void yyerror(const char *s);

static struct GPExpr *global_expr;
%}

/* Bison declarations */
%locations

%union {
    double dval;
    struct GPSymRec *symp;
    struct GPExpr *exprp;
}
%token <symp> NAME
%token <dval> NUMBER

%left '-' '+'
%left '*' '/'
%right '^'

%type <exprp> expr
%type <exprp> expr_list

%glr-parser
%expect-rr 1

%%
input:
  %empty
| expr          { global_expr = $1; }
;

expr:
  NUMBER        {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_RETURN;
                    expr->dvalue = $1;
                    $$ = expr;
                }
| NAME          {
                    if ($1 == NULL) {
                        fprintf(stderr, "%d.%d-%d.%d: Undefined symbol.\n",
                        @1.first_line, @1.first_column,
                        @1.last_line, @1.last_column);
                    }
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_RETURN;
                    expr->symbol = $1;
                    $$ = expr;
                }
| NUMBER NAME   {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    struct GPExpr *expr1 = calloc(1, sizeof(*expr));
                    struct GPExpr *expr2 = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_MULT;
                    expr1->op = GP_OP_RETURN;
                    expr1->dvalue = $1;
                    expr2->op = GP_OP_RETURN;
                    expr2->symbol = $2;
                    expr->arg1 = expr1;
                    expr->arg2 = expr2;
                    $$ = expr;
                }
| expr '+' expr {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_ADD;
                    expr->arg1 = $1;
                    expr->arg2 = $3;
                    $$ = expr;
                }
| expr '-' expr {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_SUB;
                    expr->arg1 = $1;
                    expr->arg2 = $3;
                    $$ = expr;
                }
| expr '*' expr {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_MULT;
                    expr->arg1 = $1;
                    expr->arg2 = $3;
                    $$ = expr;
                }
| expr '/' expr {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_DIV;
                    expr->arg1 = $1;
                    expr->arg2 = $3;
                    $$ = expr;
                }
| expr '^' expr {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_EXP;
                    expr->arg1 = $1;
                    expr->arg2 = $3;
                    $$ = expr;
                }
| '-' expr      {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_NEGATE;
                    expr->arg1 = $2;
                    $$ = expr;
                }
| '(' error     {
                    fprintf(stderr, "%d.%d: Unbalanced parenthesis.\n",
                            @1.first_line, @1.first_column);
                }
| '(' expr ')'  {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_RETURN;
                    expr->arg1 = $2;
                    $$ = expr;
                }   %dprec 1
| expr_list         %dprec 2
| NAME '(' expr ')' {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_FUNC;
                    expr->symbol = $1;
                    expr->arg1 = $3;
                    $$ = expr;
                }
;

expr_list:
  '(' expr ')'  {
                    struct GPExpr *expr = calloc(1, sizeof(*expr));
                    expr->op = GP_OP_RETURN;
                    expr->arg1 = $2;
                    $$ = expr;
                }
| expr_list '(' expr ')'{
                            struct GPExpr *elist = calloc(1, sizeof(*elist));
                            struct GPExpr *expr = calloc(1, sizeof(*expr));
                            elist->op = GP_OP_MULT;
                            elist->arg1 = $1;
                            expr->op = GP_OP_RETURN;
                            expr->arg1 = $3;
                            elist->arg2 = expr;
                            $$ = elist;
                        }
;
%%
/** Epilogue **/
/*** C code section ***/
/*
int main(int argc, char *argv[])
{
    yyin = stdin;
    do {
        yyparse();
    } while (!feof(yyin));
}
*/
struct GPExpr *gp_expr_parse(const char *str)
{
    global_expr = NULL;
    yycolumn = 1;
    yylineno = 1;

    YY_BUFFER_STATE buffer = yy_scan_string(str);
    yyparse();
    yy_delete_buffer(buffer);
    return global_expr;
}

void yyerror(const char *s)
{
    fprintf(stderr, "Parser error at position %d: %s\n", yycolumn, s);
}
