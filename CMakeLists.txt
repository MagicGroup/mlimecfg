project(kcm_mlimecfg)
cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_subdirectory(src)
add_subdirectory(po)
