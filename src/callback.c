#include "callback.h"

gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data)
{
	return FALSE;
}

void destroy( GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

void quit( GtkWidget *widget, gpointer data)
{
	destroy(widget, data);
}
