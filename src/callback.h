#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "globals.h"

gboolean delete_event( GtkWidget *widget, GdkEvent *event, gpointer data);
void destroy( GtkWidget *widget, gpointer data);
void quit( GtkWidget *widget, gpointer data);

#endif
