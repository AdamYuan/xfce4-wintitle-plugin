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
#ifndef WINTITLE_H
#define WINTITLE_H

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
#define XFCE_WINTITLE_PLUGIN_CLASS(class) \
	(G_TYPE_CHECK_CLASS_CAST((class), XFCE_TYPE_WINTITLE_PLUGIN, WintitlePluginClass))
#define IS_XFCE_WINTITLE_PLUGIN(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), XFCE_TYPE_WINTITLE_PLUGIN))
#define IS_XFCE_WINTITLE_PLUGIN_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE((class), XFCE_TYPE_WINTITLE_PLUGIN))
#define XFCE_WINTITLE_PLUGIN_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), XFCE_TYPE_WINTITLE_PLUGIN), WintitlePluginClass)

typedef struct _WintitlePluginClass WintitlePluginClass;
typedef struct _WintitlePlugin WintitlePlugin;
#endif
