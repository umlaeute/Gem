
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoPYLON.la

gem_videoPYLON_la_CXXFLAGS =
gem_videoPYLON_la_LDFLAGS  = -module -avoid-version -shared
gem_videoPYLON_la_LIBADD   =

# RTE
gem_videoPYLON_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gem_videoPYLON_la_LDFLAGS  += @GEM_RTE_LIBS@
#gem_videoPYLON_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
#gem_videoPYLON_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# Dependencies
gem_videoPYLON_la_CXXFLAGS += @GEM_PYLON_CXXFLAGS@
gem_videoPYLON_la_LIBADD   += @GEM_PYLON_LIBS@

gem_videoPYLON_la_SOURCES= videoPYLON.cpp videoPYLON.h CameraProperties.cpp StreamGrabberProperties.cpp

