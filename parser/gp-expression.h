#ifndef GP_EXPRESSION_H
#define GP_EXPRESSION_H

#include "symbols.h"

/* GP_OP:
 * RETURN - if symbol != NULL, return lookup(symbol)
 *          else if arg1 != NULL, return arg1
 *          else return dvalue
 */
enum GPOp {
    GP_OP_RETURN,
    GP_OP_ADD,
    GP_OP_SUB,
    GP_OP_MULT,
    GP_OP_DIV,
    GP_OP_EXP,
    GP_OP_NEGATE,
    GP_OP_FUNC
};

struct GPExpr {
    enum GPOp           op;
    struct GPSymRec    *symbol;
    double              dvalue;
    struct GPExpr      *arg1;
    struct GPExpr      *arg2;
};

const char *gp_expr_get_name(const struct GPExpr *expr);

struct GPExpr *gp_expr_parse(const char *str);

void gp_expr_print(const struct GPExpr *expr);

void gp_expr_destroy(struct GPExpr *expr);

double gp_expr_eval(struct GPExpr *expr);

#endif
