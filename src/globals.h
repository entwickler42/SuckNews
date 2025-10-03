#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <gtk/gtk.h>

#if (GTK_MINOR_VERSION <= 4)
#include "gtkcellrendererprogress.h"
#endif

extern GtkWidget *window;
extern GtkWidget *statusbar;
extern GtkTextBuffer *log_buf;
extern GtkWidget *list_status;
extern GtkWidget *list_server;
extern GtkWidget *list_server;
extern GtkWidget *list_all_groups;
extern GtkWidget *list_sub_groups;

enum { GRP_SELECT, GRP_NAME, GRP_ARTICLES, GRP_NEW, GRP_LAST_UPDATE, GRP_COLUMNS };
enum { STAT_NAME, STAT_ACTION, STAT_SPEED, STAT_STATUS, STAT_STATUS_BAR, STAT_COLUMNS };
enum { SRV_CHECKED, SRV_NAME, SRV_COLUMNS };

void sn_log(const gchar* text);
void sn_set_column(GtkTreeViewColumn* column, gboolean resizeable, gboolean clickable, gint sizeing, gint width );
void sn_group_row_new(GtkWidget* list, const gchar* name, guint articles, guint new_articles, const gchar* last_update);
void sn_status_row_new(const gchar* srv_name, const gchar* action, const gchar* status, guint speed, guint status_num);
void sn_server_row_new(const gchar* name, gboolean checked);

GtkWidget* sn_icon_label_from_stock(const gchar* text,const gchar *stock_id);

void gtk_center_window(GtkWindow* window);

#endif
