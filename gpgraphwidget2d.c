#include "gpgraphwidget2d.h"
#include <math.h>
#include <stdlib.h>

/**
 * SECTION:gpgraphwidget2d
 * @Short_description: A widget that plots functions in 2D
 * @Title: GPGraphWidget2D
 *
 * GPGraphWidget2D takes functions and plots them in 2D space.
 * The visualization is configurable with a number of settings.
 */

struct _GPGraphWidget2DPrivate
{
    gdouble xmin, xmax;
    gdouble ymin, ymax;
    glong min_grid_spacing;
};

enum {
    PROP_0,
    PROP_XMIN,
    PROP_XMAX,
    PROP_YMIN,
    PROP_YMAX,
    PROP_MIN_GRID_SPACING,
    LAST_PROP
};

static GParamSpec *graph_widget2d_props[LAST_PROP] = { NULL, };

G_DEFINE_TYPE_WITH_CODE (GPGraphWidget2D, gp_graph_widget_2d,
        GTK_TYPE_DRAWING_AREA,
        G_ADD_PRIVATE (GPGraphWidget2D));

static void gp_graph_widget_2d_init (GPGraphWidget2D *widget)
{
/*
    GPGraphWidget2DPrivate *priv;

    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->xmin = -1;
    priv->xmax = 1;
    priv->ymin = -1;
    priv->ymax = 1;
*/
    GtkWidget *gtk_widget = GTK_WIDGET (widget);
    
    gtk_widget_set_events (gtk_widget, GDK_POINTER_MOTION_MASK |
            GDK_BUTTON1_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
}

static void
gp_graph_widget_2d_set_property (GObject      *object,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
    GPGraphWidget2D *widget = GP_GRAPH_WIDGET_2D (object);

    switch (prop_id)
    {
        case PROP_XMIN:
            gp_graph_widget_2d_set_xmin(widget, g_value_get_double (value));
            break;
        case PROP_XMAX:
            gp_graph_widget_2d_set_xmax(widget, g_value_get_double (value));
            break;
        case PROP_YMIN:
            gp_graph_widget_2d_set_ymin(widget, g_value_get_double (value));
            break;
        case PROP_YMAX:
            gp_graph_widget_2d_set_ymax(widget, g_value_get_double (value));
            break;
        case PROP_MIN_GRID_SPACING:
            gp_graph_widget_2d_set_min_grid_spacing(widget, 
                    g_value_get_long (value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void
gp_graph_widget_2d_get_property (GObject    *object,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
    GPGraphWidget2D *widget = GP_GRAPH_WIDGET_2D (object);
    GPGraphWidget2DPrivate *priv = gp_graph_widget_2d_get_instance_private (widget);

    switch (prop_id)
    {
        case PROP_XMIN:
            g_value_set_double (value, priv->xmin);
            break;
        case PROP_XMAX:
            g_value_set_double (value, priv->xmax);
            break;
        case PROP_YMIN:
            g_value_set_double (value, priv->ymin);
            break;
        case PROP_YMAX:
            g_value_set_double (value, priv->ymax);
            break;
        case PROP_MIN_GRID_SPACING:
            g_value_set_long(value, priv->min_grid_spacing);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

/*
static void
gp_graph_widget_2d_finalize (GObject *object)
{
}

static void
gp_graph_widget_2d_destroy (GtkWidget *widget)
{
}
*/

struct Point
{
    double x;
    double y;
};

static inline struct Point
transform_point(double width, double height,
                double xmin, double xmax,
                double ymin, double ymax,
                double x, double y)
{
    double xcenter = .5 * xmin + .5 * xmax;
    double ycenter = .5 * ymin + .5 * ymax;

    x = x - xcenter;
    y = y - ycenter;
    x = .5 * width + x * width/ (xmax - xmin);
    y = .5 * height - y * height/ (ymax - ymin);

    return (struct Point){ x, y };
}

#define cr_transform()\
do {\
    cairo_translate(cr, 0.5*width, 0.5*height);\
    cairo_scale(cr, width/(priv->xmax - priv->xmin),\
                    -height/(priv->ymax - priv->ymin));\
    cairo_translate(cr, -0.5*priv->xmax - 0.5*priv->xmin,\
                        -0.5*priv->ymax - 0.5*priv->ymin);\
} while (0)

#define tr(x,y) transform_point(width, height, priv->xmin, priv->xmax, priv->ymin, priv->ymax, x, y)

static inline double randf() { return (double)random() / RAND_MAX; }

static gboolean
gp_graph_widget_2d_draw (GtkWidget *object,
                         cairo_t   *cr)
{
    GPGraphWidget2D *widget = GP_GRAPH_WIDGET_2D (object);
    GPGraphWidget2DPrivate *priv = gp_graph_widget_2d_get_instance_private (widget);
    double width = gtk_widget_get_allocated_width (object);
    double height = gtk_widget_get_allocated_height (object);
    struct Point pnt;

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, width, height);
    cairo_fill(cr);

    // grid
    double d10 = log10(priv->xmax - priv->xmin);
    long l10 = floor(d10);
    double inc10 = pow(10, l10);
    double inc5 = inc10/5;
    double inc2 = inc10/2;
    double minx;
 
    double x_width = width * inc5 / (priv->xmax - priv->xmin);
    g_print("x_width = %lf\n", x_width);
    if (x_width < priv->min_grid_spacing) {
        ++l10;
        inc10 = pow(10, l10);
        inc5 = inc10/5;
        inc2 = inc10/2;
    }

    if (priv->xmin > 0)
        minx = priv->xmin + (inc5 - fmod(priv->xmin, inc5));
    else
        minx = priv->xmin - fmod(priv->xmin + inc5, inc5);

    g_print("x-increments: %lf, %lf, %lf\n", inc10, inc5, inc2);
    g_print("x-begin: %lf\n", minx);

    double x = minx;
    char buf[20];

    cairo_select_font_face(cr, "serif",
            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_line_width(cr, 1);
    while (islessequal(x,priv->xmax)) {
        double old_x = x;
        for (int i=0; i<4; ++i) {
            if (i == 0) {
                cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
                cairo_set_line_width(cr, 1);
            } else {
                cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
                cairo_set_line_width(cr, 0.5);
            }
            cairo_save(cr);
            cr_transform();
            cairo_move_to(cr, x, priv->ymin);
            cairo_line_to(cr, x, priv->ymax);
            cairo_restore(cr);
            cairo_stroke(cr);

            x += inc5 / 4;
        }

        x = old_x;
        if (fabs(x) < 1e-8)
            x = 0;
        cairo_set_source_rgb(cr, 0, 0, 0);
        snprintf(buf, sizeof(buf), "%2g", x);
        pnt = tr(x, 0);
        cairo_move_to(cr, pnt.x, pnt.y + 12);
        cairo_show_text(cr, buf);
        x += inc5;
    }

    double d10y = log10(priv->ymax - priv->ymin);
    long l10y = floor(d10y);
    double inc10y = pow(10, l10y);
    double inc5y = inc10y/5;
    double inc2y = inc10y/2;
    double miny;

    double y_width = height * inc5y / (priv->ymax - priv->ymin);
    g_print("y_width = %lf\n", y_width);
    if (y_width < priv->min_grid_spacing) {
        ++l10y;
        inc10y = pow(10, l10y);
        inc5y = inc10y/5;
        inc2y = inc10y/2;
    }

    if (priv->ymin > 0)
        miny = priv->ymin + (inc5y - fmod(priv->ymin, inc5y));
    else
        miny = priv->ymin - fmod(priv->ymin + inc5y, inc5y);

    g_print("y-increments: %lf, %lf, %lf\n", inc10y, inc5y, inc2y);
    g_print("y-begin: %lf\n", miny);

    double y = miny;
    while (islessequal(y,priv->ymax)) {
        double old_y = y;
        for (int i=0; i<4; ++i) {
            if (i == 0) {
                cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
                cairo_set_line_width(cr, 1);
            } else {
                cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
                cairo_set_line_width(cr, 0.5);
            }

            cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
            cairo_save(cr);
            cr_transform();
            cairo_move_to(cr, priv->xmin, y);
            cairo_line_to(cr, priv->xmax, y);
            cairo_restore(cr);
            cairo_stroke(cr);
            y += inc5y / 4;
        }
        y = old_y;

        if (fabs(y) < 1e-8) {
            y = inc5y;
            continue;
        }

        cairo_set_source_rgb(cr, 0, 0, 0);
        snprintf(buf, sizeof(buf), "%2g", y);
        pnt = tr(0, y);
        cairo_move_to(cr, pnt.x + 2, pnt.y + 12);
        cairo_show_text(cr, buf);
        y += inc5y;
    }


    cairo_set_line_width(cr, 2);
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);

    // x axis
    cairo_save(cr);
    cr_transform();
    cairo_move_to(cr, priv->xmin, 0);
    cairo_line_to(cr, priv->xmax, 0);
    cairo_restore(cr);
    cairo_stroke(cr);

    // y axis
    cairo_save(cr);
    cr_transform();
    cairo_move_to(cr, 0, priv->ymin);
    cairo_line_to(cr, 0, priv->ymax);
    cairo_restore(cr);
    cairo_stroke(cr);


    /*
    // draw axes
    // y:
    pnt = tr(0, priv->ymin);
    cairo_move_to(cr, pnt.x, pnt.y);
    pnt = tr(0, priv->ymax);
    cairo_line_to(cr, pnt.x, pnt.y);
    cairo_stroke(cr);
    // x:
    pnt = tr(priv->xmin, 0);
    cairo_move_to(cr, pnt.x, pnt.y);
    pnt = tr(priv->xmax, 0);
    cairo_line_to(cr, pnt.x, pnt.y);
    cairo_stroke(cr);
    */

    // draw sin(x)
    double p_dx = (priv->xmax - priv->xmin) / width;
    // double p_dy = (priv->ymax - priv->ymin) / height;

    double (*funcs[])(double) = { sin, tan, j0};
    static double colors[][3] = {
        { 0.1, 0.3, 0.13 },
        { 0.9, 0.2, 0.4 },
        { 0.4, 0.3, 0.2 }
    };

    for (int i=0; i<sizeof(funcs)/sizeof(funcs[0]); ++i) {
        double dr = randf();
        double dg = randf();
        double db = randf();
        double (*func)(double) = funcs[i];
        x = priv->xmin;
        y = (*func)(x);
        pnt = tr(x, y);
        colors[i][0] += dr;
        if (colors[i][0] > 1)
            colors[i][0] -= 1;
        colors[i][1] += dg;
        if (colors[i][1] > 1)
            colors[i][1] -= 1;
        colors[i][2] += db;
        if (colors[i][2] > 1)
            colors[i][2] -= 1;
        cairo_set_source_rgb(cr, colors[i][0], colors[i][1], colors[i][2]);
        cairo_move_to(cr, pnt.x, pnt.y);
        for (long w=1; w<width; ++w) {
            y = (*func)(x);
            pnt = tr(x, y);
            cairo_line_to(cr, pnt.x, pnt.y);
            x += p_dx;
        }
        cairo_stroke(cr);
    }

    return FALSE;
}

/* TODO
static void
gp_graph_widget_2d_realize_cb (GtkWidget *object)
{
    GdkWindow *win = gtk_widget_get_window (object);
    GdkDisplay *display = gtk_widget_get_display (object);

    gdk_window_set_cursor (win,
            gdk_cursor_new_from_name (display, "crosshair"));
}
*/

static void
gp_graph_widget_2d_class_init (GPGraphWidget2DClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

    // object_class->finalize = gp_graph_widget_2d_finalize;
    object_class->get_property = gp_graph_widget_2d_get_property;
    object_class->set_property = gp_graph_widget_2d_set_property;

    // widget_class->destroy = gp_graph_widget_2d_destroy;
    widget_class->draw = gp_graph_widget_2d_draw;

    graph_widget2d_props[PROP_XMIN] =
        g_param_spec_double ("x-min",
                             "x min",
                             "Minimum x",
                             -10000, 10000, -1,
                             G_PARAM_READWRITE | G_PARAM_CONSTRUCT 
                             | G_PARAM_STATIC_STRINGS);
    graph_widget2d_props[PROP_XMAX] =
        g_param_spec_double ("x-max",
                             "x max",
                             "Maximum x",
                             -10000, 10000, 1,
                             G_PARAM_READWRITE | G_PARAM_CONSTRUCT
                             | G_PARAM_STATIC_STRINGS);
    graph_widget2d_props[PROP_YMIN] =
        g_param_spec_double ("y-min",
                             "y min",
                             "Minimum y",
                             -10000, 10000, -1,
                             G_PARAM_READWRITE | G_PARAM_CONSTRUCT
                             | G_PARAM_STATIC_STRINGS);
    graph_widget2d_props[PROP_YMAX] =
        g_param_spec_double ("y-max",
                             "y max",
                             "Maximum x",
                             -10000, 10000, 1,
                             G_PARAM_READWRITE | G_PARAM_CONSTRUCT 
                             | G_PARAM_STATIC_STRINGS);
    graph_widget2d_props[PROP_MIN_GRID_SPACING] =
        g_param_spec_long ("min-grid-spacing",
                           "min grid spacing",
                           "Minimum spacing, in pixels, between grid lines.",
                           0, 10000, 20,
                           G_PARAM_READWRITE | G_PARAM_CONSTRUCT
                           | G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties (object_class, LAST_PROP,
            graph_widget2d_props);
}


GtkWidget
*gp_graph_widget_2d_new(void)
{
    return GTK_WIDGET (g_object_new (GP_TYPE_GRAPH_WIDGET_2D, NULL));
}

void
gp_graph_widget_2d_set_xmin    (GPGraphWidget2D *widget,
                                           gdouble xmin)
{
    GPGraphWidget2DPrivate *priv;

    g_return_if_fail (GP_IS_GRAPH_WIDGET_2D (widget));
    
    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->xmin = xmin;
}

void
gp_graph_widget_2d_set_xmax    (GPGraphWidget2D *widget,
                                           gdouble xmax)
{
    GPGraphWidget2DPrivate *priv;

    g_return_if_fail (GP_IS_GRAPH_WIDGET_2D (widget));
    
    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->xmax = xmax;
}

void
gp_graph_widget_2d_set_ymin    (GPGraphWidget2D *widget,
                                           gdouble ymin)
{
    GPGraphWidget2DPrivate *priv;

    g_return_if_fail (GP_IS_GRAPH_WIDGET_2D (widget));
    
    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->ymin = ymin;
}

void
gp_graph_widget_2d_set_ymax    (GPGraphWidget2D *widget,
                                           gdouble ymax)
{
    GPGraphWidget2DPrivate *priv;

    g_return_if_fail (GP_IS_GRAPH_WIDGET_2D (widget));
    
    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->ymax = ymax;
}

void
gp_graph_widget_2d_set_min_grid_spacing (GPGraphWidget2D *widget,
                                                   glong spacing)
{
    GPGraphWidget2DPrivate *priv;

    g_return_if_fail (GP_IS_GRAPH_WIDGET_2D (widget));

    priv = gp_graph_widget_2d_get_instance_private (widget);
    priv->min_grid_spacing = spacing;
}

void
gp_graph_widget_2d_add_equation (GPGraphWidget2D *widget,
                                            GPEquation *equation)
{

}
