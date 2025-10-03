#define MW_TITEL "SuckNews GUI-DEMO"

#include "callback.h"


GtkWidget* sn_group_tab_new()
{
	GtkWidget *list_view;
	GtkListStore *store_groups;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	store_groups = gtk_list_store_new ( GRP_COLUMNS, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_VALUE, G_TYPE_VALUE , G_TYPE_STRING );
	list_view	 = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store_groups));

	renderer = gtk_cell_renderer_toggle_new();
	column	= gtk_tree_view_column_new_with_attributes( 	"", renderer,"boolean", GRP_SELECT, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,25);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_view),column);

	renderer = gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Name", renderer, "text", GRP_NAME, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,365);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_view),column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Articles", renderer, "value", GRP_ARTICLES, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,75);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "New", renderer, "value", GRP_NEW, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,75);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Last update", renderer, "value", GRP_LAST_UPDATE, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,60);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_view), column);

	return list_view;
}

GtkWidget* sn_all_groups_tab()
{
	GtkWidget *scroll;

	scroll 			 = gtk_scrolled_window_new(NULL,NULL);
	list_all_groups = sn_group_tab_new();

	gtk_container_add(GTK_CONTAINER(scroll),list_all_groups);
	gtk_widget_show_all(scroll);

	return scroll;
}

GtkWidget* sn_subscribed_groups_tab()
{
	GtkWidget* scroll;

	scroll 			 = gtk_scrolled_window_new(NULL,NULL);
	list_sub_groups = sn_group_tab_new();

	gtk_container_add(GTK_CONTAINER(scroll),list_sub_groups);
	gtk_widget_show_all(scroll);

	return scroll;

	gtk_widget_show(scroll);

	return scroll;
}

GtkWidget* sn_server_tab()
{
	GtkWidget *paned;
	GtkWidget *scroll_left;
	GtkWidget *notebook;
	GtkListStore *store_server;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	scroll_left  = gtk_scrolled_window_new(NULL,NULL);
	paned	 		 = gtk_hpaned_new();

	// left part
	store_server = gtk_list_store_new( SRV_COLUMNS, G_TYPE_BOOLEAN, G_TYPE_STRING );
	list_server  = gtk_tree_view_new_with_model( GTK_TREE_MODEL(store_server) );

	renderer	= gtk_cell_renderer_toggle_new();
	column	= gtk_tree_view_column_new_with_attributes( "Use", renderer, "boolean", SRV_CHECKED, NULL );
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_server), column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Name", renderer, "text", SRV_NAME, NULL );
	gtk_tree_view_column_set_resizable(column,TRUE);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_server), column);

	gtk_container_add(GTK_CONTAINER(scroll_left),list_server);

	// right part
	notebook = gtk_notebook_new();

	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_subscribed_groups_tab(),
										sn_icon_label_from_stock("Subscribed Groups",GTK_STOCK_DND));
	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_all_groups_tab(),
										sn_icon_label_from_stock("All Groups",GTK_STOCK_DND_MULTIPLE));

	// prepare and show
	gtk_paned_pack1(GTK_PANED(paned), scroll_left, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(paned), notebook, TRUE, TRUE);
	gtk_paned_set_position (GTK_PANED(paned), 150);
	gtk_widget_show_all(paned);

	return paned;
}

GtkWidget* sn_article_tab()
{
	GtkWidget* scroll;

	scroll = gtk_scrolled_window_new(NULL,NULL);


	gtk_widget_show(scroll);

	return scroll;
}

GtkWidget* sn_queue_tab()
{
	GtkWidget* scroll;

	scroll = gtk_scrolled_window_new(NULL,NULL);


	gtk_widget_show(scroll);

	return scroll;
}


GtkWidget* sn_search_tab()
{
	GtkWidget* scroll;

	scroll = gtk_scrolled_window_new(NULL,NULL);


	gtk_widget_show(scroll);

	return scroll;
}

GtkWidget* sn_log_tab()
{
	GtkWidget* scroll;
	GtkWidget* log_text;

	scroll 	= gtk_scrolled_window_new(NULL,NULL);
	log_text	= gtk_text_view_new();
	log_buf	= gtk_text_view_get_buffer (GTK_TEXT_VIEW (log_text));

	gtk_container_add(GTK_CONTAINER(scroll),log_text);

	GTK_TEXT_VIEW(log_text)->editable 		 = FALSE;
	GTK_TEXT_VIEW(log_text)->cursor_visible = FALSE;
	GTK_TEXT_VIEW(log_text)->wrap_mode		 = GTK_WRAP_WORD;

	gtk_widget_show_all(scroll);

	return scroll;
}

GtkWidget* sn_upper_paned()
{
	GtkWidget *notebook;

	notebook = gtk_notebook_new();

	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_server_tab(),
										sn_icon_label_from_stock("Usenet Manager",GTK_STOCK_NETWORK));
	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_article_tab(),
										sn_icon_label_from_stock("Active Group",GTK_STOCK_INDEX));
	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_queue_tab(),
										sn_icon_label_from_stock("Download Queue",GTK_STOCK_SAVE));
	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_search_tab(),
										sn_icon_label_from_stock("Search",GTK_STOCK_FIND));
	gtk_notebook_append_page(	GTK_NOTEBOOK(notebook),sn_log_tab(),
										sn_icon_label_from_stock("Log",GTK_STOCK_ZOOM_IN));

	gtk_widget_show(notebook);

	return notebook;
}

GtkWidget* sn_lower_paned()
{
	GtkWidget* scroll;
	GtkListStore *store_status;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;

	scroll 		 = gtk_scrolled_window_new(NULL,NULL);
	store_status = gtk_list_store_new( STAT_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT );
	list_status  = gtk_tree_view_new_with_model( GTK_TREE_MODEL(store_status) );

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Active Server", renderer, "text", STAT_NAME , NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,200);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_status), column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Action", renderer, "text", STAT_ACTION, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,75);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_status), column);

	renderer	= gtk_cell_renderer_text_new();
	column	= gtk_tree_view_column_new_with_attributes( "Speed", renderer, "text", STAT_SPEED, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,75);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_status), column);

	renderer	= gtk_cell_renderer_progress_new();
	column	= gtk_tree_view_column_new_with_attributes( "Progress", renderer, "value", STAT_STATUS_BAR, "text", STAT_STATUS, NULL );
	sn_set_column(column,TRUE,TRUE,GTK_TREE_VIEW_COLUMN_FIXED,0);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list_status), column);

	gtk_container_add(GTK_CONTAINER(scroll),list_status);
	gtk_widget_show_all(list_status);

	return scroll;
}

GtkWidget* sn_menu_bar()
{
	GtkItemFactory* factory;
	GtkAccelGroup *accel_group;
	GtkItemFactoryEntry entries[] =
	{
		{ "/_SuckNews",						NULL,						NULL,		0,	"<Branch>" },
		{ "/SuckNews/Check for update",	"<CTRL>U",				NULL,		0,	"<StockItem>", GTK_STOCK_CONVERT },
		{ "/SuckNews/seb1",					NULL,						NULL,		0, "<Separator>"},
		{ "/SuckNews/Quit",					"<CTRL>Q",				quit,		0,	"<StockItem>", GTK_STOCK_QUIT },
		{ "/_Server",							NULL,						NULL,		0,	"<Branch>" },
		{ "/Server/Add new Server",		"<CTRL>A",				NULL,		0,	"<StockItem>", GTK_STOCK_APPLY },
		{ "/Server/Remove Server",			"<SHIFT><CTRL>A",		NULL,		0,	"<StockItem>", GTK_STOCK_CANCEL },
		{ "/_Groups",							NULL,						NULL,		0,	"<Branch>" },
		{ "/Groups/Fetch new Group List","<CTRL>G",				NULL,		0,	"<StockItem>", GTK_STOCK_REFRESH },
		{ "/Groups/Fetch new Articles",	"<CTRL>A",				NULL,		0,	"<StockItem>", GTK_STOCK_GO_DOWN },
		{ "/Groups/Cleanup Groups",		"<CTRL>C",				NULL,		0,	"<StockItem>", GTK_STOCK_UNDELETE },
		{ "/Groups/seb2",						NULL,						NULL,		0, "<Separator>"},
		{ "/Groups/Subscribe Group",		"<CTRL>P",				NULL,		0,	"<StockItem>", GTK_STOCK_OK },
		{ "/Groups/Unsubscribe Group",	"<SHIFT><CTRL>P",		NULL,		0,	"<StockItem>", GTK_STOCK_NO },
		{ "/_Options",							NULL,						NULL,		0,	"<Branch>" },
		{ "/Options/Settings",				"<CTRL>S",				NULL,		0,	"<StockItem>", GTK_STOCK_PREFERENCES },
		{ "/_?",									NULL,						NULL,		0,	"<Branch>" },
		{ "/?/Help",							"<CTRL>H",				NULL,		0,	"<StockItem>", GTK_STOCK_HELP },
		{ "/?/About",							"<CTRL>A",				NULL,		0,	"<StockItem>", GTK_STOCK_HOME }
	};
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	accel_group = gtk_accel_group_new();

	factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR,"<fooMain>",accel_group);
	gtk_item_factory_create_items(factory,19,entries,NULL);
	gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);

	return gtk_item_factory_get_widget(factory,"<fooMain>");
}

GtkWidget* sn_tool_bar()
{
	GtkWidget *toolbar;

	toolbar = gtk_toolbar_new();

	gtk_toolbar_set_style (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_BOTH);

	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Add Server",	"Add a new nntp Server",	NULL,
										gtk_image_new_from_stock(GTK_STOCK_APPLY,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Remove Server",	"Remove selected Server",	NULL,
										gtk_image_new_from_stock(GTK_STOCK_CANCEL,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Fetch Group list",	"Fetch a new list of all groups", NULL,
										gtk_image_new_from_stock(GTK_STOCK_REFRESH,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Fetch Articles",	"Fetch new Articles in selected group", NULL,
										gtk_image_new_from_stock(GTK_STOCK_GO_DOWN,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Clean up",	"Remove old Articles", NULL,
										gtk_image_new_from_stock(GTK_STOCK_UNDELETE,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Subscribe",	"Subscribe selected group", NULL,
										gtk_image_new_from_stock(GTK_STOCK_OK,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Unsubscribe",	"Unsubscribe selected group", NULL,
										gtk_image_new_from_stock(GTK_STOCK_NO,GTK_ICON_SIZE_SMALL_TOOLBAR),
										NULL, NULL );
	gtk_toolbar_append_item(	GTK_TOOLBAR(toolbar),	"Quit NewsSuck",	"Exit NewsSuck",	NULL,
										gtk_image_new_from_stock(GTK_STOCK_QUIT,GTK_ICON_SIZE_SMALL_TOOLBAR),
										GTK_SIGNAL_FUNC(quit), NULL );

	return toolbar;
}

int main(int argc, char* argv[])
{
	GtkWidget *vbox;
	GtkWidget *hpanel;
	GtkWidget *vpanel;
	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *lower_paned;
	GtkWidget *upper_paned;

	gtk_init(&argc,&argv);

	window  = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	// connect some signals
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(delete_event),NULL);
	g_signal_connect(G_OBJECT (window), "destroy",G_CALLBACK (destroy), NULL);

	// build a layout for the mainwindow
	vbox	  = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show(vbox);

	// build a menubar
	menubar = sn_menu_bar();
	gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,0);
	gtk_widget_show_all(menubar);

	// build a toolbar
	toolbar = sn_tool_bar();
	gtk_box_pack_start(GTK_BOX(vbox),toolbar,FALSE,FALSE,0);
	gtk_widget_show_all(toolbar);

	// setup the vertical panel
	vpanel	 = gtk_vpaned_new();
	gtk_box_pack_start(GTK_BOX(vbox),vpanel,TRUE,TRUE,0);
	gtk_paned_set_position (GTK_PANED(vpanel), 400);
	gtk_widget_show(vpanel);

	// upper paned
	upper_paned = sn_upper_paned();
	gtk_paned_pack1(GTK_PANED(vpanel), upper_paned, TRUE, TRUE);
	gtk_widget_show(upper_paned);

	// lower paned
	lower_paned = sn_lower_paned();
	gtk_paned_pack2(GTK_PANED(vpanel), lower_paned, TRUE, TRUE);
	gtk_widget_show(lower_paned);

	// setup status bar
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox),statusbar,FALSE,FALSE,0);
	gtk_widget_show(statusbar);

	// Set Mainwindow attributes and finnaly show it
	gtk_window_set_title(GTK_WINDOW(window),MW_TITEL);
	gtk_window_resize(GTK_WINDOW(window),800,600);
	gtk_center_window(GTK_WINDOW(window));
	gtk_widget_show(window);

	sn_log("SuckNews started");

	gtk_main();

	return 0;
}

