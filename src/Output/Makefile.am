#####################################################################
# Gem/Output: objectclasses to handle windows
#####################################################################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src
include ../check-sources.mk

pkglib_LTLIBRARIES =

if MACOSX
pkglib_LTLIBRARIES += gemcocoawindow.la
pkglib_LTLIBRARIES += gemmacoswindow.la
endif

if HAVE_GLUT
pkglib_LTLIBRARIES += gemglutwindow.la
endif

if HAVE_GLX
pkglib_LTLIBRARIES += gemglxwindow.la
endif

#if HAVE_CARBON
if HAVE_FRAMEWORK_QUICKTIME
if HAVE_FRAMEWORK_CARBON
pkglib_LTLIBRARIES += gemmacwindow.la
endif
endif

if WINDOWS
pkglib_LTLIBRARIES += gemw32window.la
endif

if HAVE_LIB_SDL
pkglib_LTLIBRARIES += gemsdlwindow.la
endif

if HAVE_LIB_SDL2
pkglib_LTLIBRARIES += gemsdl2window.la
endif

if HAVE_LIB_LIBGLFW
pkglib_LTLIBRARIES += gemglfw2window.la
endif

if HAVE_LIB_GLFW3
pkglib_LTLIBRARIES += gemglfw3window.la
endif




COMMON_LDFLAGS  = -module -avoid-version -shared -shrext .$(GEM_RTE_EXTENSION)
if WINDOWS
COMMON_LDFLAGS += -no-undefined
endif

########### gemcocoawindow ###########
# some default flags
gemcocoawindow_la_CXXFLAGS =
gemcocoawindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemcocoawindow_la_LIBADD   =
# RTE flags
gemcocoawindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemcocoawindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemcocoawindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemcocoawindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemcocoawindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemcocoawindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemcocoawindow_la @MOREFLAGS@

# object specific libraries
gemcocoawindow_la_CXXFLAGS += $(GEM_FRAMEWORK_COCOA_CFLAGS)
gemcocoawindow_la_LIBADD   += $(GEM_FRAMEWORK_COCOA_LIBS)
gemcocoawindow_la_LDFLAGS  +=

## SOURCES
nodist_EXTRA_gemcocoawindow_la_SOURCES=gemcocoawindow.cpp
gemcocoawindow_la_SOURCES = \
  gemcocoawindow.mm \
  gemcocoawindow.h

########### gemmacoswindow ###########
# some default flags
gemmacoswindow_la_CXXFLAGS =
gemmacoswindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemmacoswindow_la_LIBADD   =
# RTE flags
gemmacoswindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemmacoswindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemmacoswindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemmacoswindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemmacoswindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemmacoswindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemmacoswindow_la @MOREFLAGS@

# object specific libraries
gemmacoswindow_la_CXXFLAGS += $(GEM_FRAMEWORK_COCOA_CFLAGS)
gemmacoswindow_la_LIBADD   += $(GEM_FRAMEWORK_COCOA_LIBS)
gemmacoswindow_la_LDFLAGS  +=

## SOURCES
nodist_EXTRA_gemmacoswindow_la_SOURCES=gemmacoswindow.cpp
gemmacoswindow_la_SOURCES = \
  gemmacoswindow.mm \
  gemmacoswindow.h


######################################
# ObjC++ nastiness {

.SUFFIXES=.mm

am_gemcocoawindow_la_OBJECTS = gemcocoawindow_la-gemcocoawindow.lo
gemcocoawindow_la-gemcocoawindow.lo: gemcocoawindow.mm
@am__fastdepCXX_TRUE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemcocoawindow_la_CXXFLAGS) $(CXXFLAGS) -MT gemcocoawindow_la-gemcocoawindow.lo -MD -MP -MF $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Tpo -c -o gemcocoawindow_la-gemcocoawindow.lo `test -f 'gemcocoawindow.mm' || echo '$(srcdir)/'`gemcocoawindow.mm
@am__fastdepCXX_TRUE@	mv -f $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Tpo $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Plo
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	source='gemcocoawindow.mm' object='gemcocoawindow_la-gemcocoawindow.lo' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemcocoawindow_la_CXXFLAGS) $(CXXFLAGS) -c -o gemcocoawindow_la-gemcocoawindow.lo `test -f 'gemcocoawindow.mm' || echo '$(srcdir)/'`gemcocoawindow.mm

am_gemmacoswindow_la_OBJECTS = gemmacoswindow_la-gemmacoswindow.lo
gemmacoswindow_la-gemmacoswindow.lo: gemmacoswindow.mm
@am__fastdepCXX_TRUE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemmacoswindow_la_CXXFLAGS) $(CXXFLAGS) -MT gemmacoswindow_la-gemmacoswindow.lo -MD -MP -MF $(DEPDIR)/gemmacoswindow_la-gemmacoswindow.Tpo -c -o gemmacoswindow_la-gemmacoswindow.lo `test -f 'gemmacoswindow.mm' || echo '$(srcdir)/'`gemmacoswindow.mm
@am__fastdepCXX_TRUE@	mv -f $(DEPDIR)/gemmacoswindow_la-gemmacoswindow.Tpo $(DEPDIR)/gemmacoswindow_la-gemmacoswindow.Plo
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	source='gemmacoswindow.mm' object='gemmacoswindow_la-gemmacoswindow.lo' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemmacoswindow_la_CXXFLAGS) $(CXXFLAGS) -c -o gemmacoswindow_la-gemmacoswindow.lo `test -f 'gemmacoswindow.mm' || echo '$(srcdir)/'`gemmacoswindow.mm

# } ObjC++ nastiness
######################################

########### gemglutwindow ###########
# some default flags
gemglutwindow_la_CXXFLAGS =
gemglutwindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemglutwindow_la_LIBADD   =
# RTE flags
gemglutwindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemglutwindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemglutwindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemglutwindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemglutwindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemglutwindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemglutwindow_la @MOREFLAGS@

# object specific libraries
gemglutwindow_la_CXXFLAGS += @GLUT_CFLAGS@
gemglutwindow_la_LIBADD   += @GLUT_LIBS@
gemglutwindow_la_LDFLAGS  += @GLUT_LDFLAGS@

## SOURCES
gemglutwindow_la_SOURCES = \
  gemglutwindow.cpp \
  gemglutwindow.h


########### gemglxwindow ###########
# some default flags
gemglxwindow_la_CXXFLAGS =
gemglxwindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemglxwindow_la_LIBADD   =
# RTE flags
gemglxwindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemglxwindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemglxwindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemglxwindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemglxwindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemglxwindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemglxwindow_la @MOREFLAGS@

# object specific libraries
gemglxwindow_la_CXXFLAGS += $(X_CFLAGS)
gemglxwindow_la_LIBADD   += $(X_PRE_LIBS) $(X_LIBS) $(X_EXTRA_LIBS)
gemglxwindow_la_LDFLAGS  +=

## SOURCES
gemglxwindow_la_SOURCES = \
  gemglxwindow.cpp \
  gemglxwindow.h


########### gemmacwindow ###########
# some default flags
gemmacwindow_la_CXXFLAGS =
gemmacwindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemmacwindow_la_LIBADD   =
# RTE flags
gemmacwindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemmacwindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemmacwindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemmacwindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemmacwindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemmacwindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemmacwindow_la @MOREFLAGS@

# object specific libraries
gemmacwindow_la_CXXFLAGS +=
gemmacwindow_la_LIBADD   +=
gemmacwindow_la_LDFLAGS  +=

## SOURCES
gemmacwindow_la_SOURCES = \
  gemmacwindow.cpp \
  gemmacwindow.h


########### gemsdlwindow ###########
# some default flags
gemsdlwindow_la_CXXFLAGS =
gemsdlwindow_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemsdlwindow_la_LIBADD   =
# RTE flags
gemsdlwindow_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemsdlwindow_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemsdlwindow_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemsdlwindow_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemsdlwindow_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemsdlwindow_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemsdlwindow_la @MOREFLAGS@

# object specific libraries
gemsdlwindow_la_CXXFLAGS += $(GEM_LIB_SDL_CFLAGS)
gemsdlwindow_la_LIBADD   += $(GEM_LIB_SDL_LIBS)
gemsdlwindow_la_LDFLAGS  +=

## SOURCES
gemsdlwindow_la_SOURCES = \
  gemsdlwindow.cpp \
  gemsdlwindow.h



########### gemsdl2window ###########
# some default flags
gemsdl2window_la_CXXFLAGS =
gemsdl2window_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemsdl2window_la_LIBADD   =
# RTE flags
gemsdl2window_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemsdl2window_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemsdl2window_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemsdl2window_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemsdl2window_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemsdl2window_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemsdl2window_la @MOREFLAGS@

# object specific libraries
gemsdl2window_la_CXXFLAGS += $(GEM_LIB_SDL2_CFLAGS)
gemsdl2window_la_LIBADD   += $(GEM_LIB_SDL2_LIBS)
gemsdl2window_la_LDFLAGS  +=

## SOURCES
gemsdl2window_la_SOURCES = \
  gemsdl2window.cpp \
  gemsdl2window.h



########### gemw32window ###########
# some default flags
gemw32window_la_CXXFLAGS =
gemw32window_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemw32window_la_LIBADD   =
# RTE flags
gemw32window_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemw32window_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemw32window_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemw32window_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemw32window_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemw32window_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemw32window_la @MOREFLAGS@

# object specific libraries
gemw32window_la_CXXFLAGS += @GL_CFLAGS@
gemw32window_la_LIBADD   += @GL_LIBS@
gemw32window_la_LDFLAGS  += @GL_LDFLAGS@
## QuickTime support
#gemw32window_la_CXXFLAGS += $(GEM_FRAMEWORK_QUICKTIME_CFLAGS)
#gemw32window_la_LIBADD   += $(GEM_FRAMEWORK_QUICKTIME_LIBS)
if WINDOWS
gemw32window_la_LIBADD   += -lgdi32
endif

## SOURCES
gemw32window_la_SOURCES = \
  gemw32window.cpp \
  gemw32window.h



########### gemglfw2window ###########
# some default flags
gemglfw2window_la_CXXFLAGS =
gemglfw2window_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemglfw2window_la_LIBADD   =
# RTE flags
gemglfw2window_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemglfw2window_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemglfw2window_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemglfw2window_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemglfw2window_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemglfw2window_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemglfw2window_la @MOREFLAGS@

# object specific libraries
gemglfw2window_la_CXXFLAGS += $(GEM_LIB_LIBGLFW_CFLAGS)
gemglfw2window_la_LIBADD   += $(GEM_LIB_LIBGLFW_LIBS)
gemglfw2window_la_LDFLAGS  +=

## SOURCES
gemglfw2window_la_SOURCES = \
  gemglfw2window.cpp \
  gemglfw2window.h


########### gemglfw3window ###########
# some default flags
gemglfw3window_la_CXXFLAGS =
gemglfw3window_la_LDFLAGS  = $(COMMON_LDFLAGS)
gemglfw3window_la_LIBADD   =
# RTE flags
gemglfw3window_la_CXXFLAGS += $(GEM_RTE_CFLAGS)
gemglfw3window_la_LIBADD   += $(GEM_RTE_LIBS)
# arch flags
gemglfw3window_la_CXXFLAGS += $(GEM_ARCH_CXXFLAGS)
gemglfw3window_la_LDFLAGS  += $(GEM_ARCH_LDFLAGS)
# flags for building Gem externals
gemglfw3window_la_CXXFLAGS += $(GEM_EXTERNAL_CFLAGS)
gemglfw3window_la_LIBADD   += -L$(top_builddir) $(GEM_EXTERNAL_LIBS)
# gemglfw3window_la @MOREFLAGS@

# object specific libraries
gemglfw3window_la_CXXFLAGS += $(GEM_LIB_GLFW3_CFLAGS)
gemglfw3window_la_LIBADD   += $(GEM_LIB_GLFW3_LIBS)
gemglfw3window_la_LDFLAGS  +=

## SOURCES
gemglfw3window_la_SOURCES = \
  gemglfw3window.cpp \
  gemglfw3window.h


# convenience symlinks for pkglib_LTLIBRARIES
# convenience symlinks for pkglib_LTLIBRARIES

## uha, this is ugly
#%.$(GEM_RTE_EXTENSION): %.la
#	rm -f $@
#	test -f .libs/$@ && $(LN_S) .libs/$@ $@ || true
#
#.PHONY: clean-conviencesymlink
#
#clean-conviencesymlink:
#	rm -f *.$(GEM_RTE_EXTENSION)
#
#
#all-local:: $(pkglib_LTLIBRARIES:.la=.$(GEM_RTE_EXTENSION))
#
#clean-local:: clean-conviencesymlink


# convenience links: symlink the plugin-binary into top_builddir
# include this makefile snippet in your plugin's Makefile.am to automatically
# create symlinks to all pkglib_LTLIBRARIES binaries into your top builddir


.PHONY: convenience-link clean-convenience-link

convenience-link: $(pkglib_LTLIBRARIES)
	  @for soname in `echo | $(EGREP) -h "^dlname=" $^ | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
            echo "$$soname: creating convenience link from $(abs_builddir)/.libs to $(top_builddir)"; \
	    rm -f $(top_builddir)/$$soname ; \
	    test -e $(abs_builddir)/.libs/$$soname && \
	    cd $(abs_top_builddir) && \
	    $(LN_S) $(abs_builddir)/.libs/$$soname $$soname || echo "create oops $$soname" && \
	    cd $(abs_builddir); \
	  done

clean-convenience-link:
	  @for soname in `echo | $(EGREP) -h "^dlname=" $(pkglib_LTLIBRARIES) | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
            echo "$$soname: cleaning convenience links"; \
            test -L $(top_builddir)/$$soname && rm -f $(top_builddir)/$$soname || echo "clean oops: $$soname"; \
	  done


all-local:: convenience-link

clean-local:: clean-convenience-link
