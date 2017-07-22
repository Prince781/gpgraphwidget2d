#ifndef GP_EQUATION_H
#define GP_EQUATION_H

#include <glib-object.h>

G_BEGIN_DECLS

#define GP_TYPE_EQUATION        (gp_equation_get_type ())

G_DECLARE_FINAL_TYPE (GPEquation, gp_equaiton, GP, EQUATION, GObject)

typedef struct _GPEquationPrivate   GPEquationPrivate;

struct _GPEquation
{
    GObject              parent_instance;
    GPEquationPrivate   *priv;
    float                color[3];
};

struct _GPEquationClass
{
    GObjectClass     parent_class;
};

GPEquation  *gp_equation_new    (char dval, const char *expression);

G_END_DECLS

#endif
