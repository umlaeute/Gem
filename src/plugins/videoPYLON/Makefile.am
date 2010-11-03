
ACLOCAL_AMFLAGS = -I ../../m4 -I .
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_videoPYLON.la
gem_videoPYLON_la_SOURCES= videoPYLON.cpp videoPYLON.h CameraProperties.cpp StreamGrabberProperties.cpp
gem_videoPYLON_la_CXXFLAGS = @GEM_RTE_CFLAGS@ @GEM_PYLON_CXXFLAGS@
gem_videoPYLON_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ @GEM_PYLON_LIBS@


