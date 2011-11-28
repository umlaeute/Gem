#####################################################################
# Gem/Output: objectclasses to handle windows
#####################################################################

AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4
AM_CPPFLAGS = -I$(top_srcdir)/src
include ../check-sources.mk

pkglib_LTLIBRARIES = 

#if HAVE_COCOA
if MACOSX
pkglib_LTLIBRARIES += gemcocoawindow.la
endif

if HAVE_GLUT
pkglib_LTLIBRARIES += gemglutwindow.la
endif

if HAVE_GLX
pkglib_LTLIBRARIES += gemglxwindow.la
endif

#if HAVE_CARBON
if DISABLED
pkglib_LTLIBRARIES += gemmacwindow.la
endif

if WINDOWS
pkglib_LTLIBRARIES += gemw32window.la
endif

########### gemcocoawindow ###########
# some default flags
gemcocoawindow_la_CXXFLAGS =
gemcocoawindow_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
gemcocoawindow_la_LIBADD   =
# RTE flags
gemcocoawindow_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gemcocoawindow_la_LIBADD   += @GEM_RTE_LIBS@
# arch flags
gemcocoawindow_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
gemcocoawindow_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# object specific libraries
gemcocoawindow_la_CXXFLAGS += 
gemcocoawindow_la_LIBADD   += 
gemcocoawindow_la_LDFLAGS  += 

## SOURCES
nodist_EXTRA_gemcocoawindow_la_SOURCES=gemcocoawindow.cpp
gemcocoawindow_la_SOURCES = \
  gemcocoawindow.mm \
  gemcocoawindow.h


######################################
# ObjC++ nastiness {

SUFFIXES=.mm

am_gemcocoawindow_la_OBJECTS = gemcocoawindow_la-gemcocoawindow.lo

gemcocoawindow_la-gemcocoawindow.lo: gemcocoawindow.mm
@am__fastdepCXX_TRUE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemcocoawindow_la_CXXFLAGS) $(CXXFLAGS) -MT gemcocoawindow_la-gemcocoawindow.lo -MD -MP -MF $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Tpo -c -o gemcocoawindow_la-gemcocoawindow.lo `test -f 'gemcocoawindow.mm' || echo '$(srcdir)/'`gemcocoawindow.mm
@am__fastdepCXX_TRUE@	mv -f $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Tpo $(DEPDIR)/gemcocoawindow_la-gemcocoawindow.Plo
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	source='gemcocoawindow.mm' object='gemcocoawindow_la-gemcocoawindow.lo' libtool=yes @AMDEPBACKSLASH@
@AMDEP_TRUE@@am__fastdepCXX_FALSE@	DEPDIR=$(DEPDIR) $(CXXDEPMODE) $(depcomp) @AMDEPBACKSLASH@
@am__fastdepCXX_FALSE@	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(gemcocoawindow_la_CXXFLAGS) $(CXXFLAGS) -c -o gemcocoawindow_la-gemcocoawindow.lo `test -f 'gemcocoawindow.mm' || echo '$(srcdir)/'`gemcocoawindow.mm

# } ObjC++ nastiness
######################################

########### gemglutwindow ###########
# some default flags
gemglutwindow_la_CXXFLAGS =
gemglutwindow_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
gemglutwindow_la_LIBADD   =
# RTE flags
gemglutwindow_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gemglutwindow_la_LIBADD   += @GEM_RTE_LIBS@
# arch flags
gemglutwindow_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
gemglutwindow_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# object specific libraries
gemglutwindow_la_CXXFLAGS += @GLUT_CFLAGS@
gemglutwindow_la_LIBADD   += @GLUT_LIBS@
gemglutwindow_la_LDFLAGS  += 

## SOURCES
gemglutwindow_la_SOURCES = \
  gemglutwindow.cpp \
  gemglutwindow.h


########### gemglxwindow ###########
# some default flags
gemglxwindow_la_CXXFLAGS =
gemglxwindow_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
gemglxwindow_la_LIBADD   =
# RTE flags
gemglxwindow_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gemglxwindow_la_LIBADD   += @GEM_RTE_LIBS@
# arch flags
gemglxwindow_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
gemglxwindow_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

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
gemmacwindow_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
gemmacwindow_la_LIBADD   =
# RTE flags
gemmacwindow_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gemmacwindow_la_LIBADD   += @GEM_RTE_LIBS@
# arch flags
gemmacwindow_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
gemmacwindow_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# object specific libraries
gemmacwindow_la_CXXFLAGS += 
gemmacwindow_la_LIBADD   += 
gemmacwindow_la_LDFLAGS  += 

## SOURCES
gemmacwindow_la_SOURCES = \
  gemmacwindow.cpp \
  gemmacwindow.h


########### gemw32window ###########
# some default flags
gemw32window_la_CXXFLAGS =
gemw32window_la_LDFLAGS  = -module -avoid-version -shared -shrext .@GEM_RTE_EXTENSION@
gemw32window_la_LIBADD   =
# RTE flags
gemw32window_la_CXXFLAGS += @GEM_RTE_CFLAGS@
gemw32window_la_LIBADD   += @GEM_RTE_LIBS@
# arch flags
gemw32window_la_CXXFLAGS += @GEM_ARCH_CXXFLAGS@
gemw32window_la_LDFLAGS  += @GEM_ARCH_LDFLAGS@

# object specific libraries
gemw32window_la_CXXFLAGS += 
gemw32window_la_LIBADD   += 
gemw32window_la_LDFLAGS  += 

## SOURCES
gemw32window_la_SOURCES = \
  gemw32window.cpp \
  gemw32window.h


# convenience symlinks for pkglib_LTLIBRARIES

## uha, this is ugly
#%.@GEM_RTE_EXTENSION@: %.la
#	rm -f $@
#	test -f .libs/$@ && $(LN_S) .libs/$@ $@ || true
#
#.PHONY: clean-conviencesymlink
#
#clean-conviencesymlink:
#	rm -f *.@GEM_RTE_EXTENSION@
#
#
#all-local:: $(pkglib_LTLIBRARIES:.la=.@GEM_RTE_EXTENSION@)
#
#clean-local:: clean-conviencesymlink


# convenience links: symlink the plugin-binary into top_builddir
# include this makefile snippet in your plugin's Makefile.am to automatically
# create symlinks to all pkglib_LTLIBRARIES binaries into your top builddir


.PHONY: convenience-link clean-convenience-link

convenience-link: $(pkglib_LTLIBRARIES)
	  for soname in `echo | $(EGREP) -h "^dlname=" $^ | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
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
