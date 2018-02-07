SUBDIRS = fastjet .

if MONOLITHIC_PLUGINS
noinst_LTLIBRARIES = libCMSIterativeConePlugin.la
else
lib_LTLIBRARIES = libCMSIterativeConePlugin.la
endif
libCMSIterativeConePlugin_la_CXXFLAGS = $(AM_CXXFLAGS) -I$(srcdir) -I$(srcdir)/../../include
libCMSIterativeConePlugin_la_SOURCES = CMSIterativeConePlugin.cc
noinst_HEADERS = SortByEt.h

EXTRA_DIST = makefile.static
