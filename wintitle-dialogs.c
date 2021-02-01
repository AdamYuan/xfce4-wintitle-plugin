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
