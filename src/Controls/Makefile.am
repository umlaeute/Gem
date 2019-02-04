#####################################################################
# Gem/Controls: objectclasses that control rendering
#####################################################################

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libControls.la

libControls_la_CXXFLAGS =
libControls_la_LIBADD   =
libControls_la_LDFLAGS  =

# RTE flags
libControls_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libControls_la_LIBADD   += $(GEM_RTE_LIBS)
libControls_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libControls_la_SOURCES=  \
    gemframebuffer.cpp \
    gemframebuffer.h \
    gemcubeframebuffer.cpp \
    gemcubeframebuffer.h \
    gemhead.cpp \
    gemhead.h \
    gemlist.cpp \
    gemlist.h \
    gemlist_info.cpp \
    gemlist_info.h \
    gemlist_matrix.cpp \
    gemlist_matrix.h \
    gemmanager.cpp \
    gemmanager.h \
    gemreceive.cpp \
    gemreceive.h \
    render_trigger.cpp \
    render_trigger.h \
    $(empty)

if ENABLE_MULTICONTEXT
else !ENABLE_MULTICONTEXT
libControls_la_SOURCES+=  \
    gemkeyboard.cpp \
    gemkeyboard.h \
    gemkeyname.cpp \
    gemkeyname.h \
    gemmouse.cpp \
    gemmouse.h \
    gemwin.cpp \
    gemwin.h \
    $(empty)
endif !ENABLE_MULTICONTEXT


libControls_la_SOURCES+=  \
    modelfiler.cpp \
    modelfiler.h \
    $(empty)
