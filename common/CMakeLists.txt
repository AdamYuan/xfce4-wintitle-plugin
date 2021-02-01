cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 11)
project(common)
add_library(common panel-debug.c panel-utils.c panel-xfconf.c)
