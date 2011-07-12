AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

SUBDIRS  = imageMAGICK imageTIFF imageJPEG imageSGI imageQT
SUBDIRS += filmAVI filmAVIPLAY filmDS filmDarwin filmGMERLIN filmMPEG1 filmMPEG3 filmQT filmQT4L
if FALSE
SUBDIRS += filmTEST
endif
SUBDIRS += recordQT recordQT4L recordV4L recordV4L2
SUBDIRS += videoDC1394 videoDS videoDV4L videoDarwin videoSGI videoUNICAP videoV4L videoV4L2 videoVFW

SUBDIRS += videoAVT videoHALCON videoPYLON

## filmTEST is not built by default, but we want to ship it
DIST_SUBDIRS = $(SUBDIRS) filmTEST


noinst_LTLIBRARIES = libplugins.la

libplugins_la_CXXFLAGS =
libplugins_la_LIBADD   =
libplugins_la_LDFLAGS  =

# RTE flags
libplugins_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
libplugins_la_LIBADD   += @GEM_RTE_LIBS@
libplugins_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

libplugins_la_SOURCES= \
        PluginFactory.cpp \
        PluginFactory.h \
        PluginFactoryTimple.h \
        imageloader.cpp \
        imageloader.h \
        imagesaver.cpp \
        imagesaver.h \
        image.cpp \
        image.h \
        film.cpp \
        film.h \
        record.cpp \
        record.h \
        video.cpp \
        video.h \
        videoGrabber.cpp \
        videoGrabber.h
