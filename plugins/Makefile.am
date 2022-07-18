AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

SUBDIRS  =

## image loading/saving plugins
SUBDIRS += imageMAGICK imageIO
SUBDIRS += TIFF JPEG SGI STB
SUBDIRS += QuickTime


## film loading plugins
SUBDIRS += AVI AVIPLAY GMERLIN MPEG1 MPEG3 QT4L
SUBDIRS += filmAVF filmDS filmDarwin
if DISABLED
SUBDIRS += filmDSATL
endif

## video capture plugins
SUBDIRS += DC1394 DV4L UNICAP
SUBDIRS += V4L V4L2 VFW VLC VNC
SUBDIRS += VIDS
SUBDIRS += videoAVF videoDS videoDarwin videoSGI

## proprietary video capture plugins
SUBDIRS += AVT HALCON PYLON DECKLINK NDI

## asset loading plugins
SUBDIRS += OBJ ASSIMP2 ASSIMP3

if DISABLED
SUBDIRS += TEST
endif

## filmTEST is not built by default, but we want to ship it
DIST_SUBDIRS = $(SUBDIRS)
DIST_SUBDIRS += TEST
DIST_SUBDIRS += filmDSATL
