AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = libRTE.la

libRTE_la_CXXFLAGS =
libRTE_la_LIBADD   =

# RTE flags
libRTE_la_CXXFLAGS += @GEM_RTE_CFLAGS@
libRTE_la_LIBADD   += @GEM_RTE_LIBS@

libRTE_la_SOURCES= \
        Array.cpp \
        Array.h \
        MessageCallbacks.h
