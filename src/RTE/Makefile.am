AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src
include ../check-sources.mk

noinst_LTLIBRARIES = libRTE.la

libRTE_la_CXXFLAGS =
libRTE_la_LIBADD   =
libRTE_la_LDFLAGS  =

# RTE flags
libRTE_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
libRTE_la_LIBADD   += @GEM_RTE_LIBS@
libRTE_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

libRTE_la_SOURCES= \
        Array.cpp \
        Array.h \
        MessageCallbacks.h
