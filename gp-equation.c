#include "gp-equation.h"
#include "parser/gp-expression.h"
#include <assert.h>

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

typedef struct _GPEquationPrivate GPEquationPrivate;

enum {
    PROP_0,
    PROP_COLOR,
    LAST_PROP
};

static GParamSpec *equation_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_CODE (GPEquation, gp_equation, G_TYPE_OBJECT,
        G_ADD_PRIVATE (GPEquation));

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
                    g_value_get_boxed (value));
            g_assert (G_VALUE_HOLDS (value, GDK_TYPE_RGBA));
            gp_equation_set_color (equation, 
                    g_value_get_boxed (value));
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

static void
gp_equation_class_init (GPEquationClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);

    object_class->get_property = gp_equation_get_property;
    object_class->set_property = gp_equation_set_property;

    equation_props[PROP_COLOR] =
        g_param_spec_boxed ("color",
                            "color",
                            "The color",
                            GDK_TYPE_RGBA,
                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT
                            | G_PARAM_STATIC_STRINGS);
}

void
gp_equation_set_color (GPEquation *eq, GdkRGBA *rgba)
{
    eq->color = rgba;
}
