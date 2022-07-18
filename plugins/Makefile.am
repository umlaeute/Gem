AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)/src

SUBDIRS  =

## some plugins are not built by default, but we want to ship them anyhow
## (so add them manually to DIST_SUBDIRS)
DIST_SUBDIRS =

# alphabetically ordered (by API)
SUBDIRS += ASSIMP2
SUBDIRS += ASSIMP3
SUBDIRS += AVI
SUBDIRS += AVIPLAY
SUBDIRS += DC1394
SUBDIRS += DV4L
SUBDIRS += GMERLIN
SUBDIRS += imageIO
SUBDIRS += imageMAGICK
SUBDIRS += JPEG
SUBDIRS += MPEG1
SUBDIRS += MPEG3
SUBDIRS += OBJ
SUBDIRS += QT4L
SUBDIRS += QuickTime
SUBDIRS += SGI
SUBDIRS += STB
if DISABLED
SUBDIRS += TEST
else
DIST_SUBDIRS += TEST
endif
SUBDIRS += TIFF
SUBDIRS += UNICAP
SUBDIRS += V4L
SUBDIRS += V4L2
SUBDIRS += VFW
SUBDIRS += VIDS
SUBDIRS += VLC
SUBDIRS += VNC

# specific plugins
SUBDIRS += filmAVF
SUBDIRS += filmDS
if DISABLED
SUBDIRS += filmDSATL
else
DIST_SUBDIRS += filmDSATL
endif
SUBDIRS += filmDarwin
SUBDIRS += videoAVF
SUBDIRS += videoDS
SUBDIRS += videoDarwin
SUBDIRS += videoSGI

# proprietary SDKs
SUBDIRS += AVT
SUBDIRS += DECKLINK
SUBDIRS += HALCON
SUBDIRS += NDI
SUBDIRS += PYLON


## obviously we want to ship all subdirs...
DIST_SUBDIRS += $(SUBDIRS)
