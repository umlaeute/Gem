AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

SUBDIRS  =

## image loading/saving plugins
SUBDIRS += imageMAGICK imageTIFF imageJPEG
SUBDIRS += imageSGI imageSTB
SUBDIRS += imageQT imageIO


## film loading plugins
SUBDIRS += filmAVF AVI filmAVIPLAY filmDS filmDarwin GMERLIN MPEG1 MPEG3 filmQT filmQT4L
if DISABLED
SUBDIRS += filmDSATL
SUBDIRS += filmTEST
endif

## Pix output (continuous) plugins
SUBDIRS += recordQT recordQT4L

## video capture plugins
SUBDIRS += videoAVF
SUBDIRS += videoDC1394 videoDS videoDV4L videoDarwin videoSGI videoUNICAP
SUBDIRS += V4L V4L2 videoVFW videoVLC videoVNC
SUBDIRS += videoVIDS

## proprietary video capture plugins
SUBDIRS += videoAVT videoHALCON videoPYLON DECKLINK NDI
if DISABLED
SUBDIRS += videoTEST
endif

## asset loading plugins
SUBDIRS += modelOBJ modelASSIMP2 modelASSIMP3


## filmTEST is not built by default, but we want to ship it
DIST_SUBDIRS = $(SUBDIRS)
DIST_SUBDIRS += filmTEST filmDSATL
DIST_SUBDIRS += videoTEST
