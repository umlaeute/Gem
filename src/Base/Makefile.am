# this file was generated automatically from Makefile.am.template
# rather than editing this file directly, you should instead edit the
# original Makefile.am.template and re-generate this file
AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = libBase.la

libBase_la_CXXFLAGS = -DHAVE_VERSION_H
libBase_la_LIBADD   =
libBase_la_LDFLAGS  =

# RTE flags
libBase_la_CXXFLAGS += @GEM_RTE_CFLAGS@ @GEM_ARCH_CXXFLAGS@
libBase_la_LIBADD   += @GEM_RTE_LIBS@
libBase_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# X
libBase_la_CXXFLAGS += $(X_CFLAGS)
libBase_la_LIBADD   += $(X_PRE_LIBS) $(X_LIBS) $(X_EXTRA_LIBS)

# GLEW
libBase_la_CXXFLAGS += @GEM_LIB_GLEW_CFLAGS@
libBase_la_LIBADD   += @GEM_LIB_GLEW_LIBS@

# FTGL
libBase_la_CXXFLAGS += @GEM_LIB_FTGL_CFLAGS@
libBase_la_LIBADD   += @GEM_LIB_FTGL_LIBS@


## version stuff
.PHONY: FORCE

FORCE:

GemVersion.cpp: version.h FORCE

version.h: $(srcdir)/../version.h.in FORCE
	$(SHELL) $(srcdir)/../pkgversion.sh $< > $@




libBase_la_SOURCES=  \
    configDarwin.h \
    configLinux.h \
    configNT.h \
    CPPExtern.cpp \
    CPPExtern.h \
    GemBase.cpp \
    GemBase.h \
    GemCache.cpp \
    GemCache.h \
    GemConfig.h \
    GemContext.cpp \
    GemContextData.cpp \
    GemContextData.h \
    GemContext.h \
    GemDylib.cpp \
    GemDylib.h \
    GemEvent.cpp \
    GemEvent.h \
    GemException.cpp \
    GemException.h \
    GemExportDef.h \
    GemFiles.cpp \
    GemFiles.h \
    GemFuncUtil.cpp \
    GemFuncUtil.h \
    GemGL.h \
    GemGluObj.cpp \
    GemGluObj.h \
    GemGLUtil.cpp \
    GemGLUtil_generated.h \
    GemGLUtil.h \
    GemLoaders.cpp \
    GemLoaders.h \
    GemLoadObj.cpp \
    GemLoadObj.h \
    GemMan.cpp \
    GemMan.h \
    GemMath.h \
    GemModelData.cpp \
    GemModelData.h \
    GemPathBase.cpp \
    GemPathBase.h \
    GemPBuffer.cpp \
    GemPBuffer.h \
    GemPixConvertAltivec.cpp \
    GemPixConvert.h \
    GemPixConvertSSE2.cpp \
    GemPixDualObj.cpp \
    GemPixDualObj.h \
    GemPixImageLoad.cpp \
    GemPixImageLoad.h \
    GemPixImageSave.cpp \
    GemPixImageSave.h \
    GemPixObj.cpp \
    GemPixObj.h \
    GemPixPete.h \
    GemPixUtil.cpp \
    GemPixUtil.h \
    GemSettings.cpp \
    GemSettings.h \
    GemSetup.cpp \
    GemShape.cpp \
    GemShape.h \
    GemSIMD.cpp \
    GemSIMD.h \
    GemState.cpp \
    GemState.h \
    GemVector.cpp \
    GemVector.h \
    GemVersion.cpp \
    GemVersion.h \
    GemVertex.cpp \
    GemVertex.h \
    GemWinCreate.h \
    GemWinCreateMac.cpp \
    GemWinCreateNT.cpp \
    GemWinCreateXWin.cpp \
    GemWindow.cpp \
    GemWindow.h \
    glew.cpp \
    glew.h \
    GLStack.cpp \
    GLStack.h \
    glxew.h \
    Matrix.cpp \
    Matrix.h \
    Properties.cpp \
    Properties.h \
    TextBase.cpp \
    TextBase.h \
    wglew.h
