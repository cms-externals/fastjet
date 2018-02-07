SUBDIRS = fastjet

if MONOLITHIC_PLUGINS
noinst_LTLIBRARIES = libPxConePlugin.la
else
lib_LTLIBRARIES = libPxConePlugin.la
endif
libPxConePlugin_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir) -I$(srcdir)/../../include
libPxConePlugin_la_SOURCES = pxcone.f PxConePlugin.cc

EXTRA_DIST = makefile.static pxcone.h

