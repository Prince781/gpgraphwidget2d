#include "gp-equation.h"
#include "parser/gp-expression.h"

/**
 * SECTION:gpequation
 */

struct _GPEquationPrivate
{
    struct GPExpr *expr;
};

