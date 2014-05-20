SUBDIRS = siscone fastjet .

if MONOLITHIC_PLUGINS
noinst_LTLIBRARIES = libSISConePlugin.la
else
lib_LTLIBRARIES = libSISConePlugin.la
endif
libSISConePlugin_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir) -I$(srcdir)/siscone -I$(srcdir)/../../include
libSISConePlugin_la_SOURCES = SISConePlugin.cc SISConeSphericalPlugin.cc

EXTRA_DIST = makefile.static
