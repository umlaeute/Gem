#####################################################################
# Gem/Gem: core infrastructure of Gem
#####################################################################

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src -DGEM_INTERNAL $(GEM_CPPFLAGS)
include ../check-sources.mk

noinst_LTLIBRARIES = libGem.la

libGem_la_CXXFLAGS =
libGem_la_LIBADD   =
libGem_la_LDFLAGS  =

# RTE flags
libGem_la_CXXFLAGS += $(GEM_RTE_CFLAGS) $(GEM_ARCH_CXXFLAGS)
libGem_la_LIBADD   += $(GEM_RTE_LIBS)
libGem_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)

libGem_la_CFLAGS    = $(libGem_la_CXXFLAGS)

libGem_la_includedir = $(includedir)/Gem/Gem
libGem_la_include_HEADERS = \
	ExportDef.h \
	Version.h \
	RTE.h \
	State.h \
	Cache.h \
	Rectangle.h \
	Exception.h \
	Dylib.h \
	Files.h \
	Context.h \
	ContextData.h \
	Properties.h \
	Settings.h \
	Loaders.h \
	Manager.h \
	PBuffer.h \
	Event.h

libGem_la_include_HEADERS += \
	GemGL.h \
	GemGLconfig.h \
	GLStack.h

libGem_la_include_HEADERS += \
	Image.h \
	ImageIO.h \
	PixConvert.h

libGem_la_SOURCES =

libGem_la_SOURCES += \
	configDarwin.h \
	configLinux.h \
	configNT.h \
	GemConfig.h \
	GemGL.h \
	Cache.cpp \
	Cache.h \
	Context.cpp \
	Context.h \
	ContextData.cpp \
	ContextData.h \
	Dylib.cpp \
	Dylib.h \
	Event.cpp \
	Event.h \
	Exception.cpp \
	Exception.h \
	ExportDef.h \
	Files.cpp \
	Files.h \
	GLStack.cpp \
	GLStack.h \
	Image.cpp \
	Image.h \
	ImageLoad.cpp \
	ImageSave.cpp \
	ImageIO.h \
	PixConvert.h \
	PixConvertAltivec.cpp \
	PixConvertSSE2.cpp \
	Loaders.cpp \
	Loaders.h \
	Manager.cpp \
	Manager.h \
	PBuffer.cpp \
	PBuffer.h \
	Properties.cpp \
	Properties.h \
	Rectangle.cpp \
	Rectangle.h \
	RTE.h \
	Settings.cpp \
	Settings.h \
	Setup.cpp \
	State.cpp \
	State.h \
	VertexBuffer.cpp \
	VertexBuffer.h \
	Version.h

