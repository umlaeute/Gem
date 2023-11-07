#####################################################################
# Gem/Nongeos: objectclasses for 3D-objects that are not visible
#              (mainly lights)
#####################################################################

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libglad.la
libglad_la_includedir = $(includedir)/Gem/glad

libglad_la_CXXFLAGS =
libglad_la_LIBADD   =
libglad_la_LDFLAGS  =

# RTE flags
libglad_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libglad_la_LIBADD   += $(GEM_RTE_LIBS)
libglad_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libglad_la_include_HEADERS = \
    KHR/khrplatform.h \
    gl.h \
    $(empty)

libglad_la_SOURCES=  \
    KHR/khrplatform.h \
    gl.h \
    gl.c \
    $(empty)

if ENABLE_MULTICONTEXT
libglad_la_SOURCES+=  \
    gl_mx.h \
    $(empty)
libglad_la_include_HEADERS += \
    gl_mx.h \
    $(empty)
endif

if HAVE_GLX
libglad_la_SOURCES+=  \
    glx.h \
    glx.c \
    $(empty)
libglad_la_include_HEADERS += \
    glx.h \
    $(empty)
endif

if WINDOWS
libglad_la_SOURCES+=  \
    wgl.h \
    wgl.c \
    $(empty)
libglad_la_include_HEADERS += \
    wgl.h \
    $(empty)
endif
