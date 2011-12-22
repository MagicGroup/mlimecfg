include(MacroLibrary)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${QT_INCLUDES} ${KDE4_INCLUDES})

########### next target ###############

set(kcm_mlimecfg_SRCS
    mlimecfg.cpp
    mlimecfg_cl.cpp)

kde4_add_plugin(kcm_mlimecfg ${kcm_mlimecfg_SRCS})

target_link_libraries(kcm_mlimecfg ${KDE4_KIO_LIBS} ${QT_LIBRARY})

########### install files ###############

install(TARGETS kcm_mlimecfg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES mlimecfg.desktop DESTINATION ${SERVICES_INSTALL_DIR})
#kde4_install_icons(${ICON_INSTALL_DIR})
