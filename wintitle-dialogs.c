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
void wintitle_plugin_about(XfcePanelPlugin *plugin) {
	const gchar *auth[] = {"AdamYuan <y13916619121@126.com>", NULL};
	gtk_show_about_dialog(NULL,                                                               //
	                      "license", "GNU General Public License version 2 (GPLv2)",          //
	                      "version", "0.0.1",                                                 //
	                      "program-name", "Wintitle",                                         //
	                      "comments", _("Shows activate window title and icon."),             //
	                      "website", "https://github.com/AdamYuan/xfce4-wintitle-plugin",     //
	                      "copyright", _("Copyright \xc2\xa9 2021 Yitian Yuan (AdamYuan)\n"), //
	                      "authors", auth, NULL);
}
