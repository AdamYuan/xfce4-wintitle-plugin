#ifndef WINTITLE_H
#define WINTITLE_H

#include <libwnck/libwnck.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4panel/xfce-panel-macros.h>
#include <libxfce4util/libxfce4util.h>

typedef struct _WintitlePlugin WintitlePlugin;
typedef struct _WintitlePluginClass WintitlePluginClass;

#define XFCE_TYPE_WINTITLE_PLUGIN            (wintitle_plugin_get_type())
#define XFCE_WINTITLE_PLUGIN(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), XFCE_TYPE_WINTITLE_PLUGIN, WintitlePlugin))
#define XFCE_WINTITLE_PLUGIN_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST((class), XFCE_TYPE_WINTITLE_PLUGIN, WintitlePluginClass))
#define IS_XFCE_WINTITLE_PLUGIN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), XFCE_TYPE_WINTITLE_PLUGIN))
#define IS_XFCE_WINTITLE_PLUGIN_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), XFCE_TYPE_WINTITLE_PLUGIN))
#define XFCE_WINTITLE_PLUGIN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), XFCE_TYPE_WINTITLE_PLUGIN), WintitlePluginClass)

GType wintitle_plugin_get_type (void) G_GNUC_CONST;
void wintitle_plugin_register_type(XfcePanelTypeModule *typeModule);

#endif
