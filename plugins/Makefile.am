AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

SUBDIRS  =

## image loading/saving plugins
SUBDIRS += imageMAGICK TIFF JPEG
SUBDIRS += imageSGI STB
SUBDIRS += imageQT imageIO


## film loading plugins
SUBDIRS += filmAVF AVI AVIPLAY filmDS filmDarwin GMERLIN MPEG1 MPEG3 filmQT filmQT4L
if DISABLED
SUBDIRS += filmDSATL
SUBDIRS += filmTEST
endif

## Pix output (continuous) plugins
SUBDIRS += recordQT recordQT4L

## video capture plugins
SUBDIRS += videoAVF
SUBDIRS += DC1394 videoDS DV4L videoDarwin videoSGI UNICAP
SUBDIRS += V4L V4L2 VFW VLC VNC
SUBDIRS += VIDS

## proprietary video capture plugins
SUBDIRS += AVT HALCON PYLON DECKLINK NDI
if DISABLED
SUBDIRS += videoTEST
endif

## asset loading plugins
SUBDIRS += modelOBJ modelASSIMP2 modelASSIMP3


## filmTEST is not built by default, but we want to ship it
DIST_SUBDIRS = $(SUBDIRS)
DIST_SUBDIRS += filmTEST filmDSATL
DIST_SUBDIRS += videoTEST
