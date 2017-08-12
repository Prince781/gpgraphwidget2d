#include "gp-equation.h"
#include "parser/gp-expression.h"

/**
 * SECTION:gpequation
 * @Short_description: An object that represents a function.
 * @Title: GPEquation
 *
 * GPEquation represents an equation.
 */

struct _GPEquationPrivate
{
    struct GPExpr *expr;
};

enum {
    PROP_0,
    PROP_COLOR,
    LAST_PROP
};

static GParamSpec *equation_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_CODE (GPEquation, gp_equation, G_TYPE_OBJECT,
        G_ADD_PRIVATE (GPEquationPrivate));

static void gp_equation_init (GPEquation *equation)
{
}

static void
gp_equation_set_property (GObject       *object,
                          guint          prop_id,
                          const GValue  *value,
                          GParamSpec    *pspec)
{
    GPEquation *equation = GP_EQUATION (object);
    switch (prop_id)
    {
        case PROP_COLOR:
            assert (G_VALUE_HOLDS (value, GDK_TYPE_RGBA));
            gp_equation_set_color (equation, 
                    g_value_get_boxed (value, GDK_TYPE_RGBA));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
gp_equation_get_property (GObject       *object,
                          guint          prop_id,
                          GValue        *value,
                          GParamSpec    *pspec)
{
    GPEquation *equation = GP_EQUATION (object);

    switch (prop_id)
    {
        case PROP_COLOR:
            g_value_set_boxed (value, equation->color);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

void
gp_equation_set_color (GPEquation *eq, GdkRGBA *rgba)
{
    eq->color = rgba;
}
