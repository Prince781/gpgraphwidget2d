#include "symbols.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct GPSymRec *gp_symtab = NULL;

struct GPSymRec *gp_symbol_put(const char *name, enum GPSymType type)
{
    struct GPSymRec *symrec;

    if ((symrec = gp_symbol_get(name)) != NULL)
        if (symrec->type == type)
            return symrec;

    symrec = calloc(1, sizeof(*symrec));
    symrec->name = strdup(name);
    symrec->type = type;
    symrec->next = gp_symtab;
    gp_symtab = symrec;

    return symrec;
}

struct GPSymRec *gp_symbol_get(const char *name)
{
    return gp_symbol_get_r(gp_symtab, name);
}

struct GPSymRec *gp_symbol_get_r(struct GPSymRec *symrec,
                                 const char *name)
{
    while (symrec != NULL) {
        if (strcmp(symrec->name, name) == 0)
            return symrec;
        symrec = symrec->next;
    }
    return NULL;
}

void gp_symbol_table_init(void)
{
    struct initv
    {
        const char *vname;
        double val;
    };
    struct initf
    {
        const char *fname;
        double (*func)(double);
    };
    static const struct initv globals[] = {
        { "pi", M_PI },
        { "e", M_E },
        { 0, 0 }
    };
    static const struct initf funcs[] = {
        { "sin", sin },
        { "cos", cos },
        { "tan", tan },
        { "ln",  log },
        { "sqrt", sqrt},
        { 0, 0 }
    };

    struct GPSymRec *symrec;
    for (int i=0; globals[i].vname != 0; ++i) {
        symrec = gp_symbol_put(globals[i].vname, GP_SYMBOL_TYPE_VAR);
        symrec->value.val = globals[i].val;
    }
    for (int i=0; funcs[i].fname != 0; ++i) {
        symrec = gp_symbol_put(funcs[i].fname, GP_SYMBOL_TYPE_FUNC);
        symrec->value.funcptr = funcs[i].func;
    }
}
