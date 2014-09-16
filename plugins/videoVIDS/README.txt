videoVIDS
===


this is a plugin for the "videoInput" library by Theodore Watson.
The library gives us access to DirectShow video-grabbers
(so it's only useful on W32).
You can get it from
   https://github.com/ofTheo
For convenience, it has been included in the videoInput/ library
(it's only two files)

Compiling
====

should just work.

However, for unknown reasons `libtool` fails to properly link
with some relevant libraries (uuid, strmiids).

The hack currently applied in the Makefile, is to use

    gem_videoVIDS_la_LDFLAGS  += -lole32 -loleaut32  -Xlinker -lstrmiids -Xlinker -luuid

rather than 

    gem_videoVIDS_la_LIBADD   += -lole32 -loleaut32 -lstrmiids -luuid

This is very ugly (and probably buggy), as autotools documentation is very clear that
`LDFLAGS` should only be used for linker-flags and *not* for adding libraries
(for which `LIBADD` should be used when creating libs, which is what we do)
