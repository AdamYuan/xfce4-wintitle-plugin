#define DEFAULT_TITLE_MAX_CHARS 80
#define DEFAULT_SPACING 5
#define DEFAULT_MINI_ICON FALSE
#include <common/panel-private.h>
#include <common/panel-utils.h>
#include <common/panel-xfconf.h>
#include <libwnck/libwnck.h>
#include <libxfce4panel/xfce-panel-macros.h>
#include <libxfce4panel/xfce-panel-plugin-provider.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4util/libxfce4util.h>

#include "wintitle-dialogs.h"

#define XFCE_TYPE_WINTITLE_PLUGIN (wintitle_plugin_get_type())
#define XFCE_WINTITLE_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), XFCE_TYPE_WINTITLE_PLUGIN, WintitlePlugin))
#define XFCE_WINTITLE_PLUGIN_CLASS(class)                                                                              \
	(G_TYPE_CHECK_CLASS_CAST((class), XFCE_TYPE_WINTITLE_PLUGIN, WintitlePluginClass))
#define IS_XFCE_WINTITLE_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), XFCE_TYPE_WINTITLE_PLUGIN))
#define IS_XFCE_WINTITLE_PLUGIN_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), XFCE_TYPE_WINTITLE_PLUGIN))
#define XFCE_WINTITLE_PLUGIN_GET_CLASS(obj)                                                                            \
	(G_TYPE_INSTANCE_GET_CLASS((obj), XFCE_TYPE_WINTITLE_PLUGIN), WintitlePluginClass)

typedef struct _WintitlePluginClass {
	XfcePanelPluginClass __parent__;
} WintitlePluginClass;

typedef struct _WintitlePlugin {
	XfcePanelPlugin __parent__;

	// Configuration
	guint title_max_chars;
	guint spacing;
	gboolean mini_icon;

	GtkWidget *box;
	GtkWidget *icon;
	GtkWidget *label;

	WnckScreen *screen;
	WnckWindow *window;
} WintitlePlugin;

XFCE_PANEL_DEFINE_PLUGIN(WintitlePlugin, wintitle_plugin);

///////////////////
// STATE UPDATES //
///////////////////
static void wintitle_plugin_update_window_title(WintitlePlugin *plugin) {
	gtk_label_set_text(GTK_LABEL(plugin->label), wnck_window_get_name(plugin->window));
}

static void wintitle_plugin_update_window_icon(WintitlePlugin *plugin) {
	GdkPixbuf *pixbuf = NULL;
	if (!wnck_window_get_icon_is_fallback(plugin->window)) {
		pixbuf = plugin->mini_icon ? wnck_window_get_mini_icon(plugin->window) : wnck_window_get_icon(plugin->window);
	}
	gtk_image_set_from_pixbuf(GTK_IMAGE(plugin->icon), pixbuf);
	gtk_box_set_spacing(GTK_BOX(plugin->box), pixbuf ? plugin->spacing : 0);
}

static void wintitle_plugin_update_orientation(WintitlePlugin *plugin, GtkOrientation orientation) {
	if (orientation == GTK_ORIENTATION_HORIZONTAL) {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(plugin->box), orientation);
		gtk_label_set_angle(GTK_LABEL(plugin->label), 0);
	} else {
		gtk_orientable_set_orientation(GTK_ORIENTABLE(plugin->box), orientation);
		gtk_label_set_angle(GTK_LABEL(plugin->label), 270);
	}
}

//////////////////////
// SIGNAL CALLBACKS //
//////////////////////
static void wintitle_plugin_orientation_changed(XfcePanelPlugin *panel_plugin, GtkOrientation orientation) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
	wintitle_plugin_update_orientation(plugin, orientation);
}

static void wintitle_plugin_window_name_changed(WnckWindow *window, WintitlePlugin *plugin) {
	wintitle_plugin_update_window_title(plugin);
}

static void wintitle_plugin_window_icon_changed(WnckWindow *window, WintitlePlugin *plugin) {
	wintitle_plugin_update_window_icon(plugin);
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
	wintitle_plugin_update_window_title(plugin);
	wintitle_plugin_update_window_icon(plugin);

	g_signal_connect(G_OBJECT(plugin->window), "name-changed", G_CALLBACK(wintitle_plugin_window_name_changed), plugin);
	g_signal_connect(G_OBJECT(plugin->window), "icon-changed", G_CALLBACK(wintitle_plugin_window_icon_changed), plugin);
}

////////////////////////
// GENERAL XFCE STUFF //
////////////////////////

static void wintitle_plugin_configure_plugin(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
	panel_return_if_fail(IS_XFCE_WINTITLE_PLUGIN(plugin));
}

static void wintitle_plugin_construct(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);

	xfce_panel_plugin_menu_show_configure(panel_plugin);
	xfce_panel_plugin_menu_show_about(panel_plugin);

	const PanelProperty properties[] = {
	    {"title-max-chars", G_TYPE_UINT}, {"spacing", G_TYPE_UINT}, {"mini-icon", G_TYPE_BOOLEAN}, {NULL}};
	panel_properties_bind(NULL, G_OBJECT(plugin), xfce_panel_plugin_get_property_base(panel_plugin), properties, FALSE);
}

static void wintitle_plugin_free_data(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
}

static void wintitle_plugin_init(WintitlePlugin *plugin) {
	// default properties
	plugin->title_max_chars = DEFAULT_TITLE_MAX_CHARS;
	plugin->spacing = DEFAULT_SPACING;
	plugin->mini_icon = DEFAULT_MINI_ICON;

	plugin->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, plugin->spacing);
	plugin->icon = gtk_image_new();
	plugin->label = gtk_label_new("");
	gtk_label_set_ellipsize(GTK_LABEL(plugin->label), PANGO_ELLIPSIZE_END);
	gtk_label_set_max_width_chars(GTK_LABEL(plugin->label), plugin->title_max_chars);

	gtk_container_add(GTK_CONTAINER(plugin), plugin->box);
	gtk_container_add(GTK_CONTAINER(plugin->box), plugin->icon);
	gtk_container_add(GTK_CONTAINER(plugin->box), plugin->label);
	gtk_widget_show(plugin->icon);
	gtk_widget_show(plugin->label);
	gtk_widget_show(plugin->box);

	wintitle_plugin_update_orientation(plugin, xfce_panel_plugin_get_orientation(XFCE_PANEL_PLUGIN(plugin)));

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	plugin->screen = wnck_screen_get(gdk_screen_get_number(gtk_widget_get_screen(GTK_WIDGET(plugin))));
	G_GNUC_END_IGNORE_DEPRECATIONS
	plugin->window = wnck_screen_get_active_window(plugin->screen);
	wintitle_plugin_update_window_icon(plugin);
	wintitle_plugin_update_window_title(plugin);

	g_signal_connect(G_OBJECT(plugin->screen), "active-window-changed",
	                 G_CALLBACK(wintitle_plugin_active_window_changed), plugin);
}

static void wintitle_plugin_class_init(WintitlePluginClass *class) {
	XfcePanelPluginClass *plugin_class = XFCE_PANEL_PLUGIN_CLASS(class);
	plugin_class->construct = wintitle_plugin_construct;
	plugin_class->free_data = wintitle_plugin_free_data;
	plugin_class->orientation_changed = wintitle_plugin_orientation_changed;
	plugin_class->configure_plugin = NULL; // wintitle_plugin_configure;
	plugin_class->about = wintitle_plugin_about;

	GObjectClass *gobject_class = G_OBJECT_CLASS(class);
	gobject_class->get_property = NULL; // wintitle_plugin_get_property;
	gobject_class->set_property = NULL; // wintitle_plugin_set_property;
}
