AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = libBase.la

libBase_la_CXXFLAGS =
libBase_la_LIBADD   =

# RTE flags
libBase_la_CXXFLAGS += @GEM_RTE_CFLAGS@
libBase_la_LIBADD   += @GEM_RTE_LIBS@

# GLEW
libBase_la_CXXFLAGS += @GEM_LIB_GLEW_CFLAGS@
libBase_la_LIBADD   += @GEM_LIB_GLEW_LIBS@

# FTGL
libBase_la_CXXFLAGS += @GEM_LIB_FTGL_CFLAGS@
libBase_la_LIBADD   += @GEM_LIB_FTGL_LIBS@

# image loading
libBase_la_CXXFLAGS += @GEM_LIB_IMAGEMAGICK___CFLAGS@ \
	@GEM_LIB_JPEG_CFLAGS@ \
	@GEM_LIB_TIFF_CFLAGS@
libBase_la_LIBADD   += @GEM_LIB_IMAGEMAGICK___LIBS@ \
	@GEM_LIB_JPEG_LIBS@ \
	@GEM_LIB_TIFF_LIBS@





libBase_la_SOURCES= \
        CPPExtern.cpp \
        GemBase.cpp \
        GemCache.cpp \
        GemContext.cpp \
        GemContextData.cpp \
        GemDylib.cpp \
        GemEvent.cpp \
        GemException.cpp \
        GemFiles.cpp \
        GemFuncUtil.cpp \
        GemGluObj.cpp \
        GemGLUtil.cpp \
        GemLoaders.cpp \
        GemLoadObj.cpp \
        GemMan.cpp \
        GemModelData.cpp \
        GemPathBase.cpp \
        GemPBuffer.cpp \
        GemPixConvertAltivec.cpp \
        GemPixConvertSSE2.cpp \
        GemPixDualObj.cpp \
        GemPixImageLoad.cpp \
        GemPixImageSave.cpp \
        GemPixObj.cpp \
        GemPixUtil.cpp \
        GemSettings.cpp \
        GemSetup.cpp \
        GemShape.cpp \
        GemSIMD.cpp \
        GemState.cpp \
        GemVector.cpp \
        GemVersion.cpp \
        GemVertex.cpp \
        GemWinCreateMac.cpp \
        GemWinCreateNT.cpp \
        GemWinCreateXWin.cpp \
        GemWindow.cpp \
        glew.cpp \
        GLStack.cpp \
        Matrix.cpp \
        Properties.cpp \
        sgiimage.cpp \
        TextBase.cpp \
        ThreadMutex.cpp \
        configDarwin.h \
        config.h \
        configLinux.h \
        configNT.h \
        CPPExtern.h \
        GemBase.h \
        GemCache.h \
        GemConfig.h \
        GemContextData.h \
        GemContext.h \
        GemDylib.h \
        GemEvent.h \
        GemException.h \
        GemExportDef.h \
        GemFiles.h \
        GemFuncUtil.h \
        GemGL.h \
        GemGluObj.h \
        GemGLUtil_generated.h \
        GemGLUtil.h \
        GemLoaders.h \
        GemLoadObj.h \
        GemMan.h \
        GemMath.h \
        GemModelData.h \
        GemPathBase.h \
        GemPBuffer.h \
        GemPixConvert.h \
        GemPixDualObj.h \
        GemPixImageLoad.h \
        GemPixImageSave.h \
        GemPixObj.h \
        GemPixPete.h \
        GemPixUtil.h \
        GemSettings.h \
        GemShape.h \
        GemSIMD.h \
        GemState.h \
        GemVector.h \
        GemVersion.h \
        GemVertex.h \
        GemWinCreate.h \
        GemWindow.h \
        glew.h \
        GLStack.h \
        glxew.h \
        Matrix.h \
        Properties.h \
        sgiimage.h \
        TextBase.h \
        ThreadMutex.h \
        wglew.h
