/*
 * Copyright (C) 2021 Yitian Yuan (AdamYuan)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include "wintitle-dialogs.h"
#include "wintitle.h"

enum {
	PROP_0,
	PROP_TITLE_MAX_CHARS,
	PROP_SPACING,
	PROP_USE_MINI_ICON,
};

struct _WintitlePluginClass {
	XfcePanelPluginClass __parent__;
};

struct _WintitlePlugin {
	XfcePanelPlugin __parent__;

	// Configuration
	guint title_max_chars;
	guint spacing;
	gboolean use_mini_icon;

	GtkWidget *box;
	GtkWidget *icon;
	GtkWidget *label;

	WnckScreen *screen;
	WnckWindow *window;
};

XFCE_PANEL_DEFINE_PLUGIN(WintitlePlugin, wintitle_plugin);

///////////////////
// STATE UPDATES //
///////////////////
static void wintitle_plugin_update_window_title(WintitlePlugin *plugin) {
	if (!plugin->window || !WNCK_IS_WINDOW(plugin->window)) {
		gtk_label_set_text(GTK_LABEL(plugin->label), "");
		return;
	}
	gtk_label_set_text(GTK_LABEL(plugin->label), wnck_window_get_name(plugin->window));
}

static void wintitle_plugin_update_window_icon(WintitlePlugin *plugin) {
	if (!plugin->window || !WNCK_IS_WINDOW(plugin->window)) {
		gtk_image_set_from_pixbuf(GTK_IMAGE(plugin->icon), NULL);
		gtk_box_set_spacing(GTK_BOX(plugin->box), 0);
		return;
	}
	GdkPixbuf *pixbuf = NULL;
	if (!wnck_window_get_icon_is_fallback(plugin->window)) {
		pixbuf =
		    plugin->use_mini_icon ? wnck_window_get_mini_icon(plugin->window) : wnck_window_get_icon(plugin->window);
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
	if (plugin->window && WNCK_IS_WINDOW(plugin->window)) {
		g_signal_handlers_disconnect_by_func(G_OBJECT(plugin->window), G_CALLBACK(wintitle_plugin_window_name_changed),
		                                     plugin);
		g_signal_handlers_disconnect_by_func(G_OBJECT(plugin->window), G_CALLBACK(wintitle_plugin_window_icon_changed),
		                                     plugin);
	}

	plugin->window = wnck_screen_get_active_window(screen);
	wintitle_plugin_update_window_title(plugin);
	wintitle_plugin_update_window_icon(plugin);

	if (plugin->window && WNCK_IS_WINDOW(plugin->window)) {
		g_signal_connect(G_OBJECT(plugin->window), "name-changed", G_CALLBACK(wintitle_plugin_window_name_changed),
		                 plugin);
		g_signal_connect(G_OBJECT(plugin->window), "icon-changed", G_CALLBACK(wintitle_plugin_window_icon_changed),
		                 plugin);
	}
}

////////////////////////
// GENERAL XFCE STUFF //
////////////////////////
static void wintitle_plugin_construct(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);

	xfce_panel_plugin_menu_show_configure(panel_plugin);
	xfce_panel_plugin_menu_show_about(panel_plugin);

	const PanelProperty properties[] = {
	    {"title-max-chars", G_TYPE_UINT}, {"spacing", G_TYPE_UINT}, {"use-mini-icon", G_TYPE_BOOLEAN}, NULL};
	panel_properties_bind(NULL, G_OBJECT(plugin), xfce_panel_plugin_get_property_base(panel_plugin), properties, TRUE);
}

static void wintitle_plugin_free_data(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
	g_signal_handlers_disconnect_by_func(G_OBJECT(plugin->screen), G_CALLBACK(wintitle_plugin_active_window_changed),
	                                     plugin);
}

static void wintitle_plugin_get_property(GObject *object, guint prop_id, GValue *value, GParamSpec *pspec) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(object);
	switch (prop_id) {
	case PROP_TITLE_MAX_CHARS:
		g_value_set_uint(value, plugin->title_max_chars);
		break;
	case PROP_SPACING:
		g_value_set_uint(value, plugin->spacing);
		break;
	case PROP_USE_MINI_ICON:
		g_value_set_boolean(value, plugin->use_mini_icon);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
	}
}

static void wintitle_plugin_set_property(GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(object);
	switch (prop_id) {
	case PROP_TITLE_MAX_CHARS:
		plugin->title_max_chars = g_value_get_uint(value);
		gtk_label_set_max_width_chars(GTK_LABEL(plugin->label), plugin->title_max_chars);
		break;
	case PROP_SPACING:
		plugin->spacing = g_value_get_uint(value);
		gtk_box_set_spacing(GTK_BOX(plugin->box), gtk_image_get_pixbuf(GTK_IMAGE(plugin->icon)) ? plugin->spacing : 0);
		break;
	case PROP_USE_MINI_ICON:
		plugin->use_mini_icon = g_value_get_boolean(value);
		wintitle_plugin_update_window_icon(plugin);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
	}
}

static void wintitle_plugin_init(WintitlePlugin *plugin) {
	// default properties
	plugin->title_max_chars = DEFAULT_TITLE_MAX_CHARS;
	plugin->spacing = DEFAULT_SPACING;
	plugin->use_mini_icon = DEFAULT_USE_MINI_ICON;

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
	plugin_class->configure_plugin = wintitle_plugin_configure;
	plugin_class->about = wintitle_plugin_about;

	GObjectClass *gobject_class = G_OBJECT_CLASS(class);
	gobject_class->get_property = wintitle_plugin_get_property;
	gobject_class->set_property = wintitle_plugin_set_property;
	g_object_class_install_property(gobject_class, PROP_TITLE_MAX_CHARS,
	                                g_param_spec_uint("title-max-chars", NULL, NULL, //
	                                                  TITLE_MAX_CHARS_MIN, TITLE_MAX_CHARS_MAX, DEFAULT_TITLE_MAX_CHARS,
	                                                  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property(gobject_class, PROP_SPACING,
	                                g_param_spec_uint("spacing", NULL, NULL, //
	                                                  SPACING_MIN, SPACING_MAX, DEFAULT_SPACING,
	                                                  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
	g_object_class_install_property(gobject_class, PROP_USE_MINI_ICON,
	                                g_param_spec_boolean("use-mini-icon", NULL, NULL, DEFAULT_USE_MINI_ICON, //
	                                                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}
