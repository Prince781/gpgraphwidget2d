#ifndef GP_GRAPH_WIDGET_2D_H
#define GP_GRAPH_WIDGET_2D_H

#include <gtk/gtk.h>
#include "gp-equation.h"

G_BEGIN_DECLS

#define GP_TYPE_GRAPH_WIDGET_2D             (gp_graph_widget_2d_get_type ())

G_DECLARE_FINAL_TYPE(GPGraphWidget2D, gp_graph_widget_2d, GP, GRAPH_WIDGET_2D, GtkDrawingArea)

typedef struct _GPGraphWidget2DPrivate      GPGraphWidget2DPrivate;

struct _GPGraphWidget2D
{
    GtkDrawingArea drawing_area;
};

struct _GPGraphWidget2DClass
{
    GtkDrawingArea parent_class;
};

GtkWidget   *gp_graph_widget_2d_new         (void);

void         gp_graph_widget_2d_set_xmin    (GPGraphWidget2D *widget,
                                                        gdouble xmin);
void         gp_graph_widget_2d_set_xmax    (GPGraphWidget2D *widget,
                                                        gdouble xmax);

void         gp_graph_widget_2d_set_ymin    (GPGraphWidget2D *widget,
                                                        gdouble ymin);
void         gp_graph_widget_2d_set_ymax    (GPGraphWidget2D *widget,
                                                        gdouble ymax);
void         gp_graph_widget_2d_set_min_grid_spacing (GPGraphWidget2D *widget,
                                                                glong spacing);

/* TODO */
void         gp_graph_widget_2d_add_equation(GPGraphWidget2D *widget,
                                                GPEquation *equation);


G_END_DECLS

#endif
