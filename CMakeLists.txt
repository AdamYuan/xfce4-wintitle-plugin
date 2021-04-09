cmake_minimum_required(VERSION 3.9)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

project(xfce4-wintitle-plugin C)
set(VERSION 0.0.1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(XFCE4UI REQUIRED libxfce4ui-2)
pkg_check_modules(XFCE4PANEL REQUIRED libxfce4panel-2.0)
pkg_check_modules(XFCE4UTIL REQUIRED libxfce4util-1.0)
pkg_check_modules(XFCONF REQUIRED libxfconf-0)
pkg_check_modules(WNCK REQUIRED libwnck-3.0)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${XFCE4UI_INCLUDE_DIRS})
include_directories(${XFCE4PANEL_INCLUDE_DIRS})
include_directories(${XFCE4UTIL_INCLUDE_DIRS})
include_directories(${XFCONF_INCLUDE_DIRS})
include_directories(${WNCK_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${XFCE4UI_LIBRARY_DIRS})
link_directories(${XFCE4PANEL_LIBRARY_DIRS})
link_directories(${XFCE4UTIL_LIBRARY_DIRS})
link_directories(${XFCONF_LIBRARY_DIRS})
link_directories(${WNCK_LIBRARY_DIRS})

add_definitions(
	${GTK3_CFLAGS_OTHER} 
	-DWNCK_I_KNOW_THIS_IS_UNSTABLE=1
	-DPACKAGE_NAME="wintitle"
	)

add_library(wintitle SHARED 
	common/panel-debug.c 
	common/panel-utils.c 
	common/panel-xfconf.c

	src/wintitle.c 
	src/wintitle-dialogs.c
	)

target_link_libraries(wintitle ${GLIB_LIBRARIES})
target_link_libraries(wintitle ${GTK3_LIBRARIES})
target_link_libraries(wintitle ${XFCE4UI_LIBRARY_DIRS})
target_link_libraries(wintitle ${XFCE4PANEL_LIBRARY_DIRS})
target_link_libraries(wintitle ${XFCE4UTIL_LIBRARY_DIRS})
target_link_libraries(wintitle ${XFCONF_LIBRARY_DIRS})
target_link_libraries(wintitle ${WNCK_LIBRARY_DIRS})

INSTALL(TARGETS wintitle LIBRARY DESTINATION lib/xfce4/panel/plugins/)
INSTALL(FILES wintitle.desktop DESTINATION share/xfce4/panel/plugins/)
