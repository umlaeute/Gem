#####################################################################
# Gem/Particles: objectlcasses for particle systems
#####################################################################

AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libParticles.la libPAPI.la

libParticles_la_CXXFLAGS =
libParticles_la_LIBADD   =
libParticles_la_LDFLAGS  =

libPAPI_la_CXXFLAGS =
libPAPI_la_LIBADD   =
libPAPI_la_LDFLAGS  =

# RTE flags
libParticles_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libParticles_la_LIBADD   += $(GEM_RTE_LIBS)
libParticles_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libParticles_la_LIBADD   += libPAPI.la

libPAPI_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
libPAPI_la_LIBADD   +=
libPAPI_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libPAPI_la_SOURCES=  \
    papi/papi.h \
    papi/actionapi.cpp \
    papi/actions.cpp \
    papi/general.h \
    papi/opengl.cpp \
    papi/system.cpp \
    papi/vector.h

libParticles_la_SOURCES=  \
    partlib_base.cpp \
    partlib_base.h \
    part_color.cpp \
    part_color.h \
    part_damp.cpp \
    part_damp.h \
    part_draw.cpp \
    part_draw.h \
    part_follow.cpp \
    part_follow.h \
    part_gravity.cpp \
    part_gravity.h \
    part_head.cpp \
    part_head.h \
    part_info.cpp \
    part_info.h \
    part_killold.cpp \
    part_killold.h \
    part_killslow.cpp \
    part_killslow.h \
    part_orbitpoint.cpp \
    part_orbitpoint.h \
    part_render.cpp \
    part_render.h \
    part_sink.cpp \
    part_sink.h \
    part_size.cpp \
    part_size.h \
    part_source.cpp \
    part_source.h \
    part_targetcolor.cpp \
    part_targetcolor.h \
    part_targetsize.cpp \
    part_targetsize.h \
    part_velcone.cpp \
    part_velcone.h \
    part_velocity.cpp \
    part_velocity.h \
    part_velsphere.cpp \
    part_velsphere.h \
    part_vertex.cpp \
    part_vertex.h

