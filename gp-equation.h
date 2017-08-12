#ifndef GP_EQUATION_H
#define GP_EQUATION_H

#include <glib-object.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS

#define GP_TYPE_EQUATION        (gp_equation_get_type ())

G_DECLARE_FINAL_TYPE (GPEquation, gp_equaiton, GP, EQUATION, GObject)

typedef struct _GPEquationPrivate   GPEquationPrivate;

struct _GPEquation
{
    GObject              parent_instance;
    GPEquationPrivate   *priv;
    GdkRGBA             *color;
};

struct _GPEquationClass
{
    GObjectClass     parent_class;
};

GPEquation  *gp_equation_new        (const char *str);

void        gp_equation_set_color   (GPEquation *eq, GdkRGBA *rgba);

G_END_DECLS

#endif
