INCLUDES    = $(all_includes)
METASOURCES = AUTO

# Install this plugin in the KDE modules directory
kde_module_LTLIBRARIES = kcm_mlimecfg.la

kcm_mlimecfg_la_SOURCES = mlimecfg.cpp mlimecfg_base.ui mlimecfg_cl.cpp
kcm_mlimecfg_la_LIBADD = $(LIB_TDEUI)
kcm_mlimecfg_la_LDFLAGS = -module -avoid-version $(all_libraries) -no-undefined


xdg_apps_DATA = mlimecfg.desktop


messages: rc.cpp
	$(EXTRACTRC) `find -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kcmmlimecfg.pot
noinst_HEADERS = mlimecfg_cl.h
