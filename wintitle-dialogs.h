#ifndef WINTITLE_DIALOGS_H
#define WINTITLE_DIALOGS_H

#include <gtk/gtk.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4ui/libxfce4ui.h>
#include <string.h>

G_BEGIN_DECLS

typedef struct _WintitlePluginDialog WintitlePluginDialog;

void wintitle_plugin_configure(XfcePanelPlugin *panel_plugin);
void wintitle_plugin_about(XfcePanelPlugin *panel_plugin);

G_END_DECLS

#endif
