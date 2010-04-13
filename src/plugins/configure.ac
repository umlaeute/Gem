#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.57)
AC_INIT(Gem-plugins,[],[zmoelnig@iem.at])

AC_CONFIG_SRCDIR([Makefile])

#### film decoding backends {
## AVI
if test -d "$srcdir/filmAVI"; then
  AC_CONFIG_SUBDIRS([filmAVI])
fi

## AVIPLAY
if test -d "$srcdir/filmAVIPLAY"; then
  AC_CONFIG_SUBDIRS([filmAVIPLAY])
fi

## Darwin
if test -d "$srcdir/filmDarwin"; then
  AC_CONFIG_SUBDIRS([filmDarwin])
fi

## DS
if test -d "$srcdir/filmDS"; then
  AC_CONFIG_SUBDIRS([filmDS])
fi

## GMERLIN
if test -d "$srcdir/filmGMERLIN"; then
  AC_CONFIG_SUBDIRS([filmGMERLIN])
fi

## MPEG1
if test -d "$srcdir/filmMPEG1"; then
  AC_CONFIG_SUBDIRS([filmMPEG1])
fi

## MPEG3
if test -d "$srcdir/filmMPEG3"; then
  AC_CONFIG_SUBDIRS([filmMPEG3])
fi

## QT
if test -d "$srcdir/filmQT"; then
  AC_CONFIG_SUBDIRS([filmQT])
fi

## QT4L
if test -d "$srcdir/filmQT4L"; then
  AC_CONFIG_SUBDIRS([filmQT4L])
fi
#### } film decoding backends

### QT
if test -d "$srcdir/recordQT"; then
  AC_CONFIG_SUBDIRS([recordQT])
fi

### QT4L
if test -d "$srcdir/recordQT4L"; then
  AC_CONFIG_SUBDIRS([recordQT4L])
fi

### V4L
if test -d "$srcdir/videoV4L"; then
  AC_CONFIG_SUBDIRS([videoV4L])
fi

### V4L2
if test -d "$srcdir/videoV4L2"; then
  AC_CONFIG_SUBDIRS([videoV4L2])
fi

### DV4L
if test -d "$srcdir/videoDV4L"; then
  AC_CONFIG_SUBDIRS([videoDV4L])
fi


AC_OUTPUT


