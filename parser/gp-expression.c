#include "gp-expression.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const char gp_arithmetic_ops[GP_OP_EXP - GP_OP_ADD + 1] = {
    '+',
    '-',
    '*',
    '/',
    '^'
};

const char *gp_expr_get_name(const struct GPExpr *expr)
{
    char buf[64];

    if (expr->op != GP_OP_FUNC)
        snprintf(buf, sizeof(buf), "%c", gp_arithmetic_ops[expr->op]);
    else 
        snprintf(buf, sizeof(buf) - 1, "%s", op->symbol->name);

    return buf;
}

void gp_expr_print(const struct GPExpr *expr)
{
    if (expr->op == GP_OP_RETURN) {
        if (expr->symbol != NULL)
            printf("%s", expr->symbol->name);
        else if (expr->arg1 != NULL) {
            printf("(");
            gp_expr_print(expr->arg1);
            printf(")");
        } else
            printf("%.10g", expr->dvalue);
    } else if (expr->op == GP_OP_NEGATE) {
        printf("-");
        gp_expr_print(expr->arg1);
    } else if (expr->op == GP_OP_FUNC) {
        printf("%s(", expr->symbol->name);
        gp_expr_print(expr->arg1);
        printf(")");
    } else {
        gp_expr_print(expr->arg1);
        printf(" %c ", gp_arithmetic_ops[expr->op - GP_OP_ADD]);
        gp_expr_print(expr->arg2);
    }
}

void gp_expr_destroy(struct GPExpr *expr)
{
    if (expr->op == GP_OP_RETURN) {
        if (expr->symbol != NULL)
            ;
        else if (expr->arg1 != NULL)
            gp_expr_destroy(expr->arg1);
    } else if (expr->op == GP_OP_NEGATE)
        gp_expr_destroy(expr->arg1);
    else if (expr->op == GP_OP_FUNC) {
        // free(expr->symbol);
        gp_expr_destroy(expr->arg1);
    } else {
        gp_expr_destroy(expr->arg1);
        gp_expr_destroy(expr->arg2);
    }
    free(expr);
}

double gp_expr_eval(struct GPExpr *expr)
{
    if (expr->op == GP_OP_RETURN) {
        if (expr->symbol != NULL)
            return expr->symbol->value.val;
        else if (expr->arg1 != NULL)
            return gp_expr_eval(expr->arg1);
    } else if (expr->op == GP_OP_NEGATE)
        return -gp_expr_eval(expr->arg1);
    else if (expr->op == GP_OP_FUNC)
        return (*expr->symbol->value.funcptr)( gp_expr_eval(expr->arg1) );
    else if (expr->op == GP_OP_ADD)
        return gp_expr_eval(expr->arg1) + gp_expr_eval(expr->arg2);
    else if (expr->op == GP_OP_SUB)
        return gp_expr_eval(expr->arg1) - gp_expr_eval(expr->arg2);
    else if (expr->op == GP_OP_MULT)
        return gp_expr_eval(expr->arg1) * gp_expr_eval(expr->arg2);
    else if (expr->op == GP_OP_DIV) {
        double eval1 = gp_expr_eval(expr->arg1);
        double eval2 = gp_expr_eval(expr->arg2);

        if (eval2 == 0) {
            fprintf(stderr, "%s: division by zero\n");
            return NAN;
        }

        return eval1/eval2;
    } else if (expr->op == GP_OP_EXP)
        return pow(gp_expr_eval(expr->arg1), gp_expr_eval(expr->arg2));

    fprintf(stderr, "%s: unknown operation\n");
    return NAN;
}
