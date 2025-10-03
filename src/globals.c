#include "globals.h"
#include <gdk/gdk.h>

GtkWidget *window;
GtkWidget *statusbar;
GtkTextBuffer *log_buf;
GtkWidget *list_status;
GtkWidget *list_server;
GtkWidget *list_all_groups;
GtkWidget *list_sub_groups;

void sn_log(const gchar* text)
{
	gtk_text_buffer_insert_at_cursor(log_buf,text,-1);
	gtk_text_buffer_insert_at_cursor(log_buf,"\n",-1);
}

void sn_set_column(GtkTreeViewColumn* column, gboolean resizeable, gboolean clickable, gint sizeing, gint width )
{
	gtk_tree_view_column_set_resizable(column,resizeable);
	gtk_tree_view_column_set_sizing(column,sizeing);
	gtk_tree_view_column_set_fixed_width(column,width);
	gtk_tree_view_column_set_clickable(column,clickable);
}

void sn_group_row_new(GtkWidget* list, const gchar* name, guint articles, guint new_articles, const gchar* last_update)
{
	GtkListStore* store_groups;
	GtkTreeIter iter;
	gchar b_articles[256], b_new[256];

	sscanf(b_articles,"%i",articles);
	sscanf(b_new,"%i",new_articles);

	store_groups = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

	gtk_list_store_append(store_groups, &iter);
	gtk_list_store_set(	store_groups, &iter,
								GRP_SELECT, FALSE,
								GRP_NAME, name,
								GRP_ARTICLES,b_articles,
								GRP_NEW,b_new,
								GRP_LAST_UPDATE,last_update
								-1 );
}

void sn_status_row_new(const gchar* srv_name, const gchar* action, const gchar* status, guint speed, guint status_num)
{
	GtkListStore* store_status;
	GtkTreeIter iter;
	gchar b_stat[256],b_speed[256];

	strncpy(b_stat,status,256);
	sscanf(b_speed,"%0.2f kB",speed);

	store_status = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_status)));

	gtk_list_store_append (store_status,&iter);
	gtk_list_store_set(	store_status, &iter,
								STAT_NAME,srv_name,
								STAT_ACTION,action,
								STAT_SPEED,b_speed,
								STAT_STATUS,b_stat,
								STAT_STATUS_BAR,status_num,
								-1 );
}

void sn_server_row_new(const gchar* name, gboolean checked)
{
	GtkListStore* store_server;
	GtkTreeIter iter;

	store_server = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list_server)));

	gtk_list_store_append(store_server,&iter);
	gtk_list_store_set(	store_server, &iter,
								SRV_CHECKED,checked,
								SRV_NAME,name,
								-1 );
}

GtkWidget* sn_icon_label_from_stock(const gchar* text,const gchar *stock_id)
{
	GtkWidget *tab_label;
	GtkWidget *hbox;
	GtkWidget *icon;

	icon		 = gtk_image_new_from_stock(stock_id,GTK_ICON_SIZE_SMALL_TOOLBAR);
	tab_label = gtk_label_new(text);
	hbox		 = gtk_hbox_new(FALSE,0);

	gtk_box_pack_start(GTK_BOX(hbox),GTK_WIDGET(icon),FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(hbox),GTK_WIDGET(tab_label),FALSE,FALSE,0);

	gtk_widget_show_all(hbox);

	return hbox;
}

void gtk_center_window(GtkWindow* window)
{
	guint w_width,w_height;
	guint s_width,s_height;
	gint x_pos,y_pos;

	gtk_window_get_size(window,&w_width,&w_height);
	s_width  = gdk_screen_width();
	s_height = gdk_screen_height();

	x_pos = (gint)s_width/2 - (gint)w_width/2;
	y_pos = (gint)s_height/2 - (gint)w_height/2;

	if(x_pos < 0) x_pos = 0;
	if(y_pos < 0) y_pos = 0;

	gtk_window_move(window,x_pos,y_pos);
}

