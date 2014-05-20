SUBDIRS = fastjet .

if MONOLITHIC_PLUGINS
noinst_LTLIBRARIES = libD0RunIConePlugin.la
else
lib_LTLIBRARIES = libD0RunIConePlugin.la
endif
libD0RunIConePlugin_la_CXXFLAGS = ${AM_CXXFLAGS} -I$(srcdir) -I$(srcdir)/../../include
libD0RunIConePlugin_la_SOURCES = D0RunIBaseConePlugin.cc

EXTRA_DIST = makefile.static ConeClusterAlgo.hpp HepEntityI.h HepEntityIpre96.h inline_maths.h
