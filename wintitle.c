#include "wintitle.h"

/*typedef struct Wintitle {
    XfcePanelPlugin *plugin;
    WnckScreen *screen;
    GdkDisplay *display;
    GtkWidget *icon;
    GtkWidget *label;
    gint size;
} Wintitle;*/

struct _WintitlePluginClass {
	XfcePanelPluginClass __parent__;
};

struct _WintitlePlugin {
	XfcePanelPlugin __parent__;

	// Configuration

	GtkWidget *box;
	GtkWidget *icon;
	GtkWidget *label;

	WnckScreen *screen;
	WnckWindow *window;
};
XFCE_PANEL_DEFINE_PLUGIN(WintitlePlugin, wintitle_plugin);

static GdkPixbuf *get_window_icon_from_theme(WnckWindow *window, GdkPixbuf *fallback) {
	GdkPixbuf *pixbuf;
	int size = gdk_pixbuf_get_width(fallback);
	GtkIconTheme *theme = gtk_icon_theme_get_default();
	const char *name = wnck_window_get_class_instance_name(window);

	/* return the most likely icon if found */
	pixbuf = gtk_icon_theme_load_icon(theme, name, size, GTK_ICON_LOOKUP_FORCE_SIZE, NULL);

	if (pixbuf)
		return pixbuf;
	return fallback;
}
static GdkPixbuf *get_window_icon(WnckWindow *window, int size) {
	GdkPixbuf *pixbuf;

	if (size <= 31)
		pixbuf = wnck_window_get_mini_icon(window);
	else
		pixbuf = wnck_window_get_icon(window);

	/* check if the icon is fallback, in that case just try with the theme */
	if (wnck_window_get_icon_is_fallback(window))
		pixbuf = get_window_icon_from_theme(window, pixbuf);
	return pixbuf;
}

static void wintitle_plugin_construct(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
}

static void wintitle_plugin_free_data(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
}

static void wintitle_plugin_window_name_changed(WnckWindow *window, WintitlePlugin *plugin) {
	gtk_label_set_text(GTK_LABEL(plugin->label), wnck_window_get_name(plugin->window));
}

static void wintitle_plugin_window_icon_changed(WnckWindow *window, WintitlePlugin *plugin) {
	gtk_image_set_from_pixbuf(GTK_IMAGE(plugin->icon), get_window_icon(plugin->window, 25));
}

static void wintitle_plugin_active_window_changed(WnckScreen *screen, WnckWindow *previous_window,
                                                  WintitlePlugin *plugin) {
	if (plugin->window) {
		g_signal_handlers_disconnect_by_func(G_OBJECT(plugin->window), G_CALLBACK(wintitle_plugin_window_name_changed),
		                                     plugin);
		g_signal_handlers_disconnect_by_func(G_OBJECT(plugin->window), G_CALLBACK(wintitle_plugin_window_icon_changed),
		                                     plugin);
	}
	plugin->window = wnck_screen_get_active_window(screen);
	gtk_image_set_from_pixbuf(GTK_IMAGE(plugin->icon), get_window_icon(plugin->window, 25));
	gtk_label_set_text(GTK_LABEL(plugin->label), wnck_window_get_name(plugin->window));

	g_signal_connect(G_OBJECT(plugin->window), "icon-changed", G_CALLBACK(wintitle_plugin_window_icon_changed), plugin);
	g_signal_connect(G_OBJECT(plugin->window), "name-changed", G_CALLBACK(wintitle_plugin_window_name_changed), plugin);
}

static void wintitle_plugin_init(WintitlePlugin *plugin) {
	plugin->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	plugin->icon = gtk_image_new();
	plugin->label = gtk_label_new("");

	gtk_container_add(GTK_CONTAINER(plugin), plugin->box);
	gtk_container_add(GTK_CONTAINER(plugin->box), plugin->icon);
	gtk_container_add(GTK_CONTAINER(plugin->box), plugin->label);
	gtk_widget_show(plugin->icon);
	gtk_widget_show(plugin->label);
	gtk_widget_show(plugin->box);

	plugin->window = NULL;

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	GdkScreen *screen = gtk_widget_get_screen(GTK_WIDGET(plugin));
	plugin->screen = wnck_screen_get(gdk_screen_get_number(screen));
	G_GNUC_END_IGNORE_DEPRECATIONS

	g_signal_connect(G_OBJECT(plugin->screen), "active-window-changed",
	                 G_CALLBACK(wintitle_plugin_active_window_changed), plugin);
}

static void wintitle_plugin_class_init(WintitlePluginClass *class) {
	XfcePanelPluginClass *plugin_class = XFCE_PANEL_PLUGIN_CLASS(class);
	plugin_class->construct = wintitle_plugin_construct;
	plugin_class->free_data = wintitle_plugin_free_data;
	plugin_class->configure_plugin = NULL; // wintitle_plugin_configure;
	plugin_class->about = NULL;            // wintitle_plugin_about;

	GObjectClass *gobject_class = G_OBJECT_CLASS(class);
	gobject_class->get_property = NULL; // wintitle_plugin_get_property;
	gobject_class->set_property = NULL; // wintitle_plugin_set_property;
}
