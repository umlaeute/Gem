#####################################################################
# Gem/Utils various utilities
#####################################################################

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libUtils.la

libUtils_la_CXXFLAGS =
libUtils_la_LIBADD   =
libUtils_la_LDFLAGS  =

# RTE flags
libUtils_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS) $(GEM_THREADS_CFLAGS)
libUtils_la_LIBADD   += $(GEM_RTE_LIBS) $(GEM_THREADS_LIBS)
libUtils_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)


# BiDirectional text output
libUtils_la_CXXFLAGS += $(GEM_LIB_FRIBIDI_CFLAGS)
libUtils_la_LIBADD   += $(GEM_LIB_FRIBIDI_LIBS)

if WINDOWS
libUtils_la_LIBADD   += -lws2_32
endif


libUtils_la_includedir = $(includedir)/Gem/Utils
libUtils_la_include_HEADERS = \
	any.h \
	Functions.h \
	GLUtil.h \
	GemMath.h \
	is_pointer.h \
	Matrix.h \
	nop.h \
	PixPete.h \
	SIMD.h \
	GemString.h \
	Vector.h

libUtils_la_include_HEADERS += \
	Thread.h \
	ThreadMutex.h \
	ThreadSemaphore.h \
	WorkerThread.h \
	SynchedWorkerThread.h

libUtils_la_SOURCES=  \
	any.h \
	Functions.cpp \
	Functions.h \
	GLUtil.cpp \
	GLUtil.h \
	GLUtil_define.cpp \
	GLUtil_define_generated.h \
	GemMath.h \
	is_pointer.h \
	Matrix.cpp \
	Matrix.h \
	PixPete.h \
	plist.cpp \
	plist.h \
	pstk.cpp \
	pstk.h \
	SIMD.cpp \
	SIMD.h \
	Thread.cpp \
	Thread.h \
	ThreadMutex.cpp \
	ThreadMutex.h \
	ThreadSemaphore.cpp \
	ThreadSemaphore.h \
	WorkerThread.cpp \
	WorkerThread.h \
	SynchedWorkerThread.cpp \
	SynchedWorkerThread.h \
	GemString.cpp \
	GemString.h \
	Vector.cpp \
	Vector.h

if MINGW
libUtils_la_SOURCES+=MSVCMinGW.cpp
endif
