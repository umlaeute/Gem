# this file was generated automatically from Makefile.am.template
# rather than editing this file directly, you should instead edit the
# original Makefile.am.template and re-generate this file

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libGem.la

libGem_la_CXXFLAGS = -DHAVE_VERSION_H
libGem_la_LIBADD   =
libGem_la_LDFLAGS  =

# RTE flags
libGem_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
libGem_la_LIBADD   += @GEM_RTE_LIBS@
libGem_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

## version stuff
.PHONY: FORCE

FORCE:

Version.cpp: version_current.h FORCE

version_current.h: $(srcdir)/../version.h.in FORCE
	$(SHELL) $(srcdir)/../pkgversion.sh $< > $@


libGem_la_SOURCES=  \
	any.h \
	Dylib.cpp \
	Dylib.h \
	Exception.cpp \
	Exception.h \
	Files.cpp \
	Files.h \
	GLStack.cpp \
	GLStack.h \
	Loaders.cpp \
	Loaders.h \
	Properties.cpp \
	Properties.h \
	RTE.h \
	Settings.cpp \
	Settings.h \
	Setup.cpp \
	SynchedWorkerThread.cpp \
	SynchedWorkerThread.h \
	ThreadMutex.cpp \
	ThreadMutex.h \
	ThreadSemaphore.cpp \
	ThreadSemaphore.h \
	Version.cpp \
	Version.h \
	WorkerThread.cpp \
	WorkerThread.h
