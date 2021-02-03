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

#define DIALOG_BORDER 16
#define DIALOG_GRID_SPACING 8

guint wintitle_plugin_bind_spinner_value_to(GtkSpinButton *spinner, GObject *object, const gchar *property) {
	guint value = 0;

	g_object_get(object, property, &value, NULL);
	gtk_spin_button_set_value(spinner, value);

	GObject *adjustment;
	g_object_get(G_OBJECT(spinner), "adjustment", &adjustment, NULL);
	g_object_bind_property(G_OBJECT(adjustment), "value", G_OBJECT(object), property,
	                       G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
	g_object_unref(adjustment);

	return value;
}

gboolean wintitle_plugin_bind_toggle_value_to(GtkToggleButton *toggle, GObject *object, const gchar *property) {
	gboolean value;
	g_object_get(object, property, &value, NULL);
	gtk_toggle_button_set_active(toggle, value);
	g_object_bind_property(G_OBJECT(toggle), "active", G_OBJECT(object), property,
	                       G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
	return value;
}

void wintitle_plugin_configure_response(GtkWidget *dialog, gint response, XfcePanelPlugin *panel_plugin) {
	g_object_set_data(G_OBJECT(panel_plugin), "dialog", NULL);
	xfce_panel_plugin_unblock_menu(panel_plugin);
	gtk_widget_destroy(dialog);
}

void wintitle_plugin_configure(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);
	panel_return_if_fail(IS_XFCE_WINTITLE_PLUGIN(plugin));

	xfce_panel_plugin_block_menu(panel_plugin);

	GtkWidget *dialog = xfce_titled_dialog_new_with_buttons(
	    _("Window Title"), GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(panel_plugin))),
	    GTK_DIALOG_DESTROY_WITH_PARENT, "gtk-close", GTK_RESPONSE_OK, NULL);

	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_name(GTK_WINDOW(dialog), "xfce4-settings");

	g_object_set_data(G_OBJECT(panel_plugin), "dialog", dialog);
	g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(wintitle_plugin_configure_response), plugin);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), DIALOG_BORDER);

	GtkWidget *container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), DIALOG_GRID_SPACING);
	gtk_grid_set_row_spacing(GTK_GRID(grid), DIALOG_GRID_SPACING);

	GtkWidget *widget = NULL, *label = NULL;
	// title-max-chars
	widget = gtk_spin_button_new_with_range(TITLE_MAX_CHARS_MIN, TITLE_MAX_CHARS_MAX, 1.0);
	wintitle_plugin_bind_spinner_value_to(GTK_SPIN_BUTTON(widget), G_OBJECT(plugin), "title-max-chars");
	label = gtk_label_new("Max title characters");
	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), widget, 1, 0, 1, 1);

	// show-icon
	GtkWidget *show_icon_toggle = gtk_check_button_new_with_label("");
	gboolean show_icon =
	    wintitle_plugin_bind_toggle_value_to(GTK_TOGGLE_BUTTON(show_icon_toggle), G_OBJECT(plugin), "show-icon");
	label = gtk_label_new("Show icon");
	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), show_icon_toggle, 1, 1, 1, 1);

	// spacing
	widget = gtk_spin_button_new_with_range(SPACING_MIN, SPACING_MAX, 1.0);
	wintitle_plugin_bind_spinner_value_to(GTK_SPIN_BUTTON(widget), G_OBJECT(plugin), "spacing");
	label = gtk_label_new("Icon spacing (pixels)");
	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), widget, 1, 2, 1, 1);

	g_object_bind_property(G_OBJECT(show_icon_toggle), "active", G_OBJECT(widget), "sensitive", G_BINDING_DEFAULT);
	g_object_bind_property(G_OBJECT(show_icon_toggle), "active", G_OBJECT(label), "sensitive", G_BINDING_DEFAULT);
	gtk_widget_set_sensitive(widget, show_icon);
	gtk_widget_set_sensitive(label, show_icon);

	// use-mini-icon
	widget = gtk_check_button_new_with_label("");
	wintitle_plugin_bind_toggle_value_to(GTK_TOGGLE_BUTTON(widget), G_OBJECT(plugin), "use-mini-icon");
	label = gtk_label_new("Use mini icon");
	gtk_label_set_xalign(GTK_LABEL(label), 0);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), widget, 1, 3, 1, 1);

	g_object_bind_property(G_OBJECT(show_icon_toggle), "active", G_OBJECT(widget), "sensitive", G_BINDING_DEFAULT);
	g_object_bind_property(G_OBJECT(show_icon_toggle), "active", G_OBJECT(label), "sensitive", G_BINDING_DEFAULT);
	gtk_widget_set_sensitive(widget, show_icon);
	gtk_widget_set_sensitive(label, show_icon);

	// add and show
	gtk_container_add(GTK_CONTAINER(container), grid);
	gtk_widget_show_all(dialog);
}

void wintitle_plugin_about(XfcePanelPlugin *plugin) {
	const gchar *auth[] = {"Yitian Yuan (AdamYuan) <y13916619121@126.com>", NULL};
	return gtk_show_about_dialog(NULL,                                                               //
	                             "logo-icon-name", NULL,                                             //
	                             "license-type", GTK_LICENSE_LGPL_2_1_ONLY,                          //
	                             "version", "0.0.1",                                                 //
	                             "program-name", "Wintitle",                                         //
	                             "comments", _("Shows activate window title and icon."),             //
	                             "website", "https://github.com/AdamYuan/xfce4-wintitle-plugin",     //
	                             "copyright", _("Copyright \xc2\xa9 2021 Yitian Yuan (AdamYuan)\n"), //
	                             "authors", auth,                                                    //
	                             NULL);
}
