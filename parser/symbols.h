#ifndef SYMBOLS_H
#define SYMBOLS_H

enum GPSymType
{
    GP_SYMBOL_TYPE_VAR,
    GP_SYMBOL_TYPE_FUNC
};

struct GPSymRec {
    char *name;
    enum GPSymType type;
    union
    {
        double val;
        double (*funcptr)(double);
    } value;
    struct GPSymRec *next;
};

extern struct GPSymRec *gp_symtab;

struct GPSymRec *gp_symbol_put(const char *name, enum GPSymType type);

struct GPSymRec *gp_symbol_get(const char *name);

struct GPSymRec *gp_symbol_get_r(const struct GPSymRec *symrec,
                                 const char *name);

#endif
