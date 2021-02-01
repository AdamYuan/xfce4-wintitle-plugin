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

#define BORDER 16
#define GRID_PADDING 8

void bind_spinner_value_to(GtkSpinButton *spinner, GObject *object, const gchar *property) {
	guint value = 0;

	g_object_get(object, property, &value, NULL);
	gtk_spin_button_set_value(spinner, value);

	GObject *adjustment;
	g_object_get(G_OBJECT(spinner), "adjustment", &adjustment, NULL);
	g_object_bind_property(G_OBJECT(adjustment), "value", G_OBJECT(object), property,
	                       G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
	g_object_unref(adjustment);
}

void bind_toggle_value_to(GtkToggleButton *toggle, GObject *object, const gchar *property) {
	gboolean value;
	g_object_get(object, property, &value, NULL);
	gtk_toggle_button_set_active(toggle, value);

	g_object_bind_property(G_OBJECT(toggle), "active", G_OBJECT(object), property,
	                       G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
}

void bind_toggle_button_value_to(GtkToggleButton *toggle, GObject *object, const gchar *property) {
	gboolean value;
	g_object_get(object, property, &value, NULL);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(toggle), value); // Seemed to be nessessary.
	g_object_bind_property(G_OBJECT(toggle), "active", G_OBJECT(object), property,
	                       G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
}

static void wintitle_plugin_configure_response(GtkWidget *dialog, gint response, XfcePanelPlugin *panel_plugin) {
	g_object_set_data(G_OBJECT(panel_plugin), "dialog", NULL);
	xfce_panel_plugin_unblock_menu(panel_plugin);
	gtk_widget_destroy(dialog);
}

void wintitle_plugin_configure(XfcePanelPlugin *panel_plugin) {
	WintitlePlugin *plugin = XFCE_WINTITLE_PLUGIN(panel_plugin);

	xfce_panel_plugin_block_menu(panel_plugin);

	GtkWidget *dialog = xfce_titled_dialog_new_with_buttons(
	    _("Window Title"), GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(panel_plugin))),
	    GTK_DIALOG_DESTROY_WITH_PARENT, "gtk-close", GTK_RESPONSE_OK, NULL);

	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_name(GTK_WINDOW(dialog), "xfce4-settings");

	g_object_set_data(G_OBJECT(panel_plugin), "dialog", dialog);
	g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(wintitle_plugin_configure_response), plugin);
	gtk_container_set_border_width(GTK_CONTAINER(dialog), BORDER);

	GtkWidget *container = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	GtkWidget *widget = NULL;
	// title-max-chars
	widget = gtk_spin_button_new_with_range(TITLE_MAX_CHARS_MIN, TITLE_MAX_CHARS_MAX, 1.0);
	bind_spinner_value_to(GTK_SPIN_BUTTON(widget), G_OBJECT(plugin), "title-max-chars");
	gtk_container_add(GTK_CONTAINER(container), gtk_label_new("Window title max width (chars)"));
	gtk_container_add(GTK_CONTAINER(container), widget);

	// spacing
	widget = gtk_spin_button_new_with_range(SPACING_MIN, SPACING_MAX, 1.0);
	bind_spinner_value_to(GTK_SPIN_BUTTON(widget), G_OBJECT(plugin), "spacing");
	gtk_container_add(GTK_CONTAINER(container), gtk_label_new("Spacing between icon and title (pixels)"));
	gtk_container_add(GTK_CONTAINER(container), widget);

	// use-mini-icon
	widget = gtk_check_button_new_with_label("Use mini icon");
	bind_toggle_value_to(GTK_TOGGLE_BUTTON(widget), G_OBJECT(plugin), "use-mini-icon");
	gtk_container_add(GTK_CONTAINER(container), widget);

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