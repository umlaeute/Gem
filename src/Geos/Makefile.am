AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

noinst_LTLIBRARIES = libGeos.la

LIBADD =

# RTE flags
CXXFLAGS += @GEM_RTE_CFLAGS@
LIBADD   += @GEM_RTE_LIBS@

# FTGL
CXXFLAGS += @GEM_LIB_FTGL_CFLAGS@
LIBADD   += @GEM_LIB_FTGL_LIBS@


libGeos_la_SOURCES= \
	circle.cpp colorSquare.cpp cone.cpp cube.cpp cuboid.cpp curve3d.cpp curve.cpp cylinder.cpp disk.cpp GemSplash.cpp imageVert.cpp mesh_line.cpp mesh_square.cpp model.cpp model_loader.cpp multimodel.cpp newWave.cpp polygon.cpp pqtorusknots.cpp primTri.cpp rectangle.cpp ripple.cpp rubber.cpp scopeXYZ~.cpp slideSquares.cpp sphere3d.cpp sphere.cpp square.cpp surface3d.cpp teapot.cpp text2d.cpp text3d.cpp textextruded.cpp textoutline.cpp torus.cpp trapezoid.cpp triangle.cpp tube.cpp circle.h colorSquare.h cone.h cube.h cuboid.h curve3d.h curve.h cylinder.h disk.h GemSplash.h imageVert.h mesh_line.h mesh_square.h model.h model_loader.h multimodel.h newWave.h polygon.h pqtorusknots.h primTri.h rectangle.h ripple.h rubber.h scopeXYZ~.h slideSquares.h sphere3d.h sphere.h square.h surface3d.h teapot.h TexFont.h text2d.h text3d.h textextruded.h textoutline.h torus.h trapezoid.h triangle.h tube.h 
