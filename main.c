#include <gtk/gtk.h>
#include "gpgraphwidget2d.h"

GtkWidget *demo_window;
GtkWidget *graph2d;

static void window_close(GtkWidget *widget)
{
    gtk_main_quit();
}

GtkWidget *create_window(void)
{
    GtkWidget *box;
    GtkWidget *window;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GPGraphWidget2D");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(window_close), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE);
    gtk_box_set_spacing(GTK_BOX(box), 6);
    gtk_container_add(GTK_CONTAINER(window), box);

    graph2d = gp_graph_widget_2d_new();
    gtk_widget_set_hexpand(graph2d, TRUE);
    gtk_widget_set_vexpand(graph2d, TRUE);
    gtk_container_add(GTK_CONTAINER(box), graph2d);

    return window;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    demo_window = create_window();

    gtk_widget_show_all(demo_window);

    gtk_main();
    return 0;
}
