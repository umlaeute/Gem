
ACLOCAL_AMFLAGS = -I ../../m4
AM_CPPFLAGS = -I../..

lib_LTLIBRARIES= gem_imageSGI.la
gem_imageSGI_la_SOURCES= imageSGI.cpp imageSGI.h sgiimage.cpp sgiimage.h
gem_imageSGI_la_CXXFLAGS = @GEM_RTE_CFLAGS@
gem_imageSGI_la_LDFLAGS = -module -avoid-version -shared @GEM_RTE_LIBS@ 

