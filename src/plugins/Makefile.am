#####################################################################
# Gem/plugins: interfaces for various plugins
#####################################################################

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libplugins.la

libplugins_la_CXXFLAGS =
libplugins_la_LIBADD   =
libplugins_la_LDFLAGS  =

# RTE flags
libplugins_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS) $(GEM_THREADS_CFLAGS)
libplugins_la_LIBADD   += $(GEM_RTE_LIBS) $(GEM_THREADS_LIBS)
libplugins_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libplugins_la_SOURCES= \
        PluginFactory.cpp \
        PluginFactory.h \
        PluginFactoryTimple.h \
        imageloader.cpp \
        imageloader.h \
        imagesaver.cpp \
        imagesaver.h \
        film.cpp \
        film.h \
        modelloader.cpp \
        modelloader.h \
        record.cpp \
        record.h \
        video.cpp \
        video.h

libplugins_la_SOURCES+= \
        videoBase.cpp \
        videoBase.h

libplugins_la_includedir = $(includedir)/Gem/plugins
libplugins_la_include_HEADERS = \
	PluginFactory.h \
	PluginFactoryTimple.h \
	film.h \
	imageloader.h \
	imagesaver.h \
	record.h \
        video.h
