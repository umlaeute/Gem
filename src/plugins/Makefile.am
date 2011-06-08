AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

SUBDIRS = imageMAGICK imageTIFF imageJPEG imageSGI imageQT

noinst_LTLIBRARIES = libplugins.la

libplugins_la_CXXFLAGS =
libplugins_la_LIBADD   =

# RTE flags
libplugins_la_CXXFLAGS += @GEM_RTE_CFLAGS@
libplugins_la_LIBADD   += @GEM_RTE_LIBS@

libplugins_la_SOURCES= \
        film.cpp \
        image.cpp \
        imageloader.cpp \
        imagesaver.cpp \
        PluginFactory.cpp \
        record.cpp \
        video.cpp \
        videoThreaded.cpp \
        film.h \
        image.h \
        imageloader.h \
        imagesaver.h \
        PluginFactory.h \
        PluginFactoryTimple.h \
        record.h \
        video.h \
        videoThreaded.h
